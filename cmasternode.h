#ifndef CMASTERNODE_H
#define CMASTERNODE_H
#include <QString>
#include <QDataStream>

class CMasternode
{
public:
    CMasternode();
    ~CMasternode();

    friend QDataStream& operator >>(QDataStream& in, CMasternode& data);
    friend QDataStream& operator <<(QDataStream& out, const CMasternode& data);
    static CMasternode DeSerializable(const QByteArray &datagram);
    static QByteArray Serializable(const CMasternode &cmn);

public:
    int m_num;
    int m_index;
    int m_port;
    QString m_alias;
    QString m_ip;
    /* 0 初始, 1 只有 hash, 2 没ssh,* 3 ssh已配置, 4 已上传，5，已启动*/
    int m_config_status;
    QString m_ssh_user;
    QString m_ssh_pwd;
    int m_ssh_port;
    QString m_remote_rpc_user;
    QString m_remote_rpc_pwd;
    QString m_remote_rpc_ip;
    QString m_local_rpc_user;
    QString m_local_rpc_pwd;
    QString m_local_rpc_ip;
    QString m_mn_address;
    QString m_mn_key;
    QString m_mn_hash;
    QString m_mn_status;
    QString m_safe_conf_path;
    QString m_mn_conf_path;
};

#endif // CMASTERNODE_H
