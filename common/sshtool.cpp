#include "sshtool.h"
#include "sshproto.h"
#include <QFile>

SshTool::SshTool():mp_session(NULL)
{
}

SshTool::~SshTool()
{
    if(mp_session)
    {
        SshProto::initance()->ssh2_session_disconnect(mp_session,
                 SSH_DISCONNECT_BY_APPLICATION,
                 "Normal Shutdown, Thank you for playing","");
        SshProto::initance()->ssh2_session_free(mp_session);
    }

#ifdef WIN32
    closesocket(m_sock);
#else
    close(m_sock);
#endif
}

int SshTool::initSession(const QString &strIp,const QString &strUsr,
                const QString &strPwd)
{
    int rc = 0;
    if ((rc=SshProto::initance()->ssh2_init(0)) != 0)
    {
        qDebug("libssh2 initialization failed (%d)\n", rc);
        return rc;
    }

    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    m_sin.sin_family = AF_INET;
    m_sin.sin_port = htons(22);
    m_sin.sin_addr.s_addr = inet_addr(strIp.toStdString().c_str());

    if (connect(m_sock, (struct sockaddr*)(&m_sin),
                sizeof(struct sockaddr_in)) != 0)
    {
        rc = GetLastError();
        qDebug("failed to connect! error:%d", rc);
        return rc;
    }
    qDebug("success to connect!");

    mp_session = SshProto::initance()->ssh2_session_init(NULL, NULL, NULL, NULL);
    if(!mp_session)
    {
        qDebug("ssh init failed!");
        return -1;
    }
    SshProto::initance()->ssh2_session_set_timeout(mp_session, 60000);
    rc = SshProto::initance()->ssh2_session_handshake(mp_session, m_sock);
    if (rc != 0)
    {
        qDebug("failure establishing SSH session: %d\n", rc);
        return rc;
    }

    if ((rc=SshProto::initance()->ssh2_userauth(mp_session,strUsr.toStdString().c_str(),
                             strUsr.length(),strPwd.toStdString().c_str(),
                             strPwd.length(),NULL)) != 0)
    {
        qDebug("user name:%s\npasswd:%s",
               strUsr.toStdString().c_str(),
               strPwd.toStdString().c_str());
        qDebug("Authentication by password failed.");
        return rc;
    }
    else
    {
        qDebug("Authentication ok.");
    }
    return rc;
}

int SshTool::exec(QString strCmd, QString &qsRespone)
{
    int rc;
    int exitcode;
    char *exitsignal=(char *)"none";
    qsRespone = "";
    int bytecount = 0;

    LIBSSH2_CHANNEL* channel = NULL;
    while( (channel = SshProto::initance()->ssh2_channel_open(
                mp_session,"session",sizeof("session") - 1,
                LIBSSH2_CHANNEL_WINDOW_DEFAULT,
                LIBSSH2_CHANNEL_PACKET_DEFAULT, NULL, 0))==NULL
           &&SshProto::initance()->ssh2_session_laster_error_msg(mp_session,NULL,NULL,0)
           ==LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }
    if( channel == NULL )
    {
        return -1;
    }
    while((rc=SshProto::initance()->ssh2_channel_process_startup(
               channel,  "exec", sizeof("exec") - 1,
               strCmd.toStdString().c_str(),
               (unsigned int)strCmd.length()))
           == LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }

    if( rc != 0 )
    {
        goto shutdown;
    }

    for( ;; )
    {
        /* loop until we block */
        int rc;
        do
        {
            char buffer[0x4000] = {0};
            rc = SshProto::initance()->ssh2_channel_read( channel, 0, buffer, sizeof(buffer) );
            if( rc > 0 )
            {
                qsRespone.append(QString(buffer));
                bytecount += rc;
            }
            else
            {
                if( rc != LIBSSH2_ERROR_EAGAIN )
                {
                    /* no need to output this for the EAGAIN case */
                    qDebug("libssh2_channel_read returned %d\n", rc);
                }
            }
        }
        while( rc > 0 );
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(m_sock, mp_session);
        }
        else
        {
            break;
        }
    }

shutdown:
    exitcode = 127;
    while( (rc=SshProto::initance()->ssh2_channel_close(channel))
           == LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }
    if( rc == 0 )
    {
        exitcode = SshProto::initance()->ssh2_channel_get_exit_status(channel);
        SshProto::initance()->ssh2_channel_get_exit_signal(
                    channel, &exitsignal,NULL, NULL, NULL,
                    NULL, NULL);
    }
    if (exitsignal)
    {
        qDebug("\nGot signal: %s\n", exitsignal);
    }
    else
    {
        qDebug("\nEXIT: %d bytecount: %d\n", exitcode, bytecount);
    }

    SshProto::initance()->ssh2_channel_free(channel);
    channel = NULL;
    return rc;
}

int SshTool::scpToRemote(const QString &strDest, const QString &strSrc)
{
    if (!mp_session)
    {
        return -1;
    }
    FILE *local;
    struct stat fileinfo;
    local = fopen(strSrc.toStdString().c_str(), "rb");
    if (!local)
    {
        qDebug("Can't open local file\n");
        return -1;
    }
    stat(strSrc.toStdString().c_str(), &fileinfo);

    LIBSSH2_CHANNEL* channel = SshProto::initance()->ssh2_scp_send(mp_session,
                         strDest.toStdString().c_str(),
                         fileinfo.st_mode & 0777,
                         (unsigned long)fileinfo.st_size,0,0);
    if (!channel)
    {
        char *errmsg;
        int errlen;
        int err = SshProto::initance()->ssh2_session_laster_error_msg(mp_session, &errmsg, &errlen,0);
        qDebug("Unable to open a session: (%d) %s\n", err, errmsg);
        fclose(local);
        return err;
    }

    int rc = 0;
    char mem[1024];
    size_t nread;
    char *ptr;

    do
    {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0)
        {
            /* end of file */
            break;
        }
        ptr = mem;
        do
        {
            /* write the same data over and over, until error or completion */
            rc = SshProto::initance()->ssh2_channel_write(channel, 0, ptr, nread);
            if (rc < 0)
            {
                qDebug("ERROR %d\n", rc);
                break;
            }
            else
            {
                /* rc indicates how many bytes were written this time */
                ptr += rc;
                nread -= rc;
            }
        } while (nread);
    } while (1);

    SshProto::initance()->ssh2_channel_send_eof(channel);
    SshProto::initance()->ssh2_channel_wait_eof(channel);
    SshProto::initance()->ssh2_channel_wait_closed(channel);
    SshProto::initance()->ssh2_channel_free(channel);
    channel = NULL;

    fclose(local);
    return 0;
}

int SshTool::scpToLocal(const QString &strDest, const QString &strSrc)
{
    libssh2_struct_stat_size got = 0;
    libssh2_struct_stat fileinfo;

    if (!mp_session)
    {
        return -1;
    }
    LIBSSH2_CHANNEL* channel = SshProto::initance()->ssh2_scp_recv2(
                mp_session, strSrc.toStdString().c_str(), &fileinfo);
    if (!channel)
    {
        int err = SshProto::initance()->ssh2_session_laster_error(mp_session);
        qDebug("Unable to open a session: %d\n", err);
        return err;
    }
    int rc = 0;
    QFile destFile(strDest);
    if (!destFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug("Unable to open a dest file.");
        SshProto::initance()->ssh2_channel_free(channel);
        return -1;
    }

    while(got < fileinfo.st_size)
    {
        char mem[1024]={0};
        int amount = sizeof(mem);
        if((fileinfo.st_size - got) < amount)
        {
            amount = (int)(fileinfo.st_size - got);
            qDebug("amount is %d", amount);
        }
        rc = SshProto::initance()->ssh2_channel_read(channel, 0, mem, amount);
        if(rc > 0)
        {
            destFile.write(mem);
        }
        else if(rc < 0)
        {
            qDebug("libssh2_channel_read() failed: %d\n", rc);
            break;
        }
        got += rc;
    }
    SshProto::initance()->ssh2_channel_free(channel);
    destFile.close();
    return 0;
}

int SshTool::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    FD_ZERO(&fd);
    FD_SET(socket_fd, &fd);

    int dir;
    /* now make sure we wait in the correct direction */
    dir = SshProto::initance()->ssh2_session_block_directions(session);
    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
    {
        readfd = &fd;
    }
    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
    {
        writefd = &fd;
    }
    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
    return rc;
}
