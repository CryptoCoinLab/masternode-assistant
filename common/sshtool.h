#ifndef SSHTOOL_H
#define SSHTOOL_H
#include <QObject>
#include <QStringList>
#include "include/libssh2.h"
#include "include/scp.h"

class SshTool
{
public:
    SshTool();
    ~SshTool();
    int initSession(const QString &strIp,const QString &strUsr,
                    const QString &strPwd);
    int exec(QString strCmd, QString &strRetValuse);
    int scpToRemote(const QString &strDest, const QString &strSrc);
    int scpToLocal(const QString &strDest, const QString &strSrc);
private:
    int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
private:
    int m_sock;
    LIBSSH2_SESSION* mp_session;
    struct sockaddr_in m_sin;
};

#endif // SSHTOOL_H
