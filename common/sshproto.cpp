#include "sshproto.h"
#include <QDebug>

bool SshProto::isLoaded = false;

SshProto::SshProto()
    : m_mylib(("libssh2.dll"))
{
#ifdef WIN32
    WSADATA wsadata;
    int err;
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        qDebug("WSAStartup failed with error: %d\n", err);
        return;
    }
#elif
    // linux startup net .
#endif

    if(m_mylib.load())
    {
        ssh2_init = (plibssh2_init)m_mylib.resolve("libssh2_init");
        ssh2_exit = (plibssh2_exit)m_mylib.resolve("libssh2_exit");
        ssh2_free = (plibssh2_free)m_mylib.resolve("libssh2_free");
        ssh2_ssa = (plibssh2_session_supported_algs)m_mylib.resolve("libssh2_session_supported_algs");
        ssh2_session_init = (plibssh2_session_init_ex)m_mylib.resolve("libssh2_session_init_ex");
        ssh2_session_handshake = (plibssh2_session_handshake)m_mylib.resolve("libssh2_session_handshake");
        ssh2_hostkey_hash = (plibssh2_hostkey_hash)m_mylib.resolve("libssh2_hostkey_hash");
        ssh2_userauth = (plibssh2_userauth_password_ex)m_mylib.resolve("libssh2_userauth_password_ex");
        ssh2_scp_recv2 = (plibssh2_scp_recv2)m_mylib.resolve("libssh2_scp_recv2");
        ssh2_session_laster_error = (plibssh2_session_last_errno)m_mylib.resolve("libssh2_session_last_errno");
        ssh2_channel_read = (plibssh2_channel_read_ex)m_mylib.resolve("libssh2_channel_read_ex");
        ssh2_scp_send = (plibssh2_scp_send_ex)m_mylib.resolve("libssh2_scp_send_ex");
        ssh2_channel_write = (plibssh2_channel_write_ex)m_mylib.resolve("libssh2_channel_write_ex");
        ssh2_session_laster_error_msg = (plibssh2_session_last_error)m_mylib.resolve("libssh2_session_last_error");
        ssh2_channel_send_eof =  (plibssh2_channel_send_eof)m_mylib.resolve("libssh2_channel_send_eof"); ;
        ssh2_channel_eof = (plibssh2_channel_eof)m_mylib.resolve("libssh2_channel_eof"); ;
        ssh2_channel_wait_eof =  (plibssh2_channel_wait_eof)m_mylib.resolve("libssh2_channel_wait_eof"); ;
        ssh2_channel_close = (plibssh2_channel_close)m_mylib.resolve("libssh2_channel_close"); ;
        ssh2_channel_wait_closed = (plibssh2_channel_wait_closed)m_mylib.resolve("libssh2_channel_wait_closed"); ;
        ssh2_channel_free = (plibssh2_channel_free)m_mylib.resolve("libssh2_channel_free"); ;
        ssh2_session_disconnect = (plibssh2_session_disconnect_ex)m_mylib.resolve("libssh2_session_disconnect_ex"); ;
        ssh2_session_free = (plibssh2_session_free)m_mylib.resolve("libssh2_session_free");
        ssh2_session_set_timeout = (plibssh2_session_set_timeout)m_mylib.resolve("libssh2_session_set_timeout");

        ssh2_channel_open = (plibssh2_channel_open_ex)m_mylib.resolve("libssh2_channel_open_ex");
        ssh2_channel_process_startup = (plibssh2_channel_process_startup)m_mylib.resolve("libssh2_channel_process_startup");
        ssh2_channel_get_exit_status = (plibssh2_channel_get_exit_status)m_mylib.resolve("libssh2_channel_get_exit_status");
        ssh2_channel_get_exit_signal = (plibssh2_channel_get_exit_signal)m_mylib.resolve("libssh2_channel_get_exit_signal");
        ssh2_session_block_directions = (plibssh2_session_block_directions)m_mylib.resolve("libssh2_session_block_directions");
        isLoaded = true;
    }
}

SshProto::~SshProto()
{
    if(m_mylib.isLoaded())
    {
        m_mylib.unload();
        isLoaded = false;
    }
}

SshProto* SshProto::initance()
{
    static SshProto sshproto;
    if (isLoaded)
    {
        return &sshproto;
    }
    else
    {
        return NULL;
    }
}
