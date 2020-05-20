#include "cmasternode.h"
#include <QBuffer>

CMasternode::CMasternode()
    :m_num(0), m_index(0), m_port(0),m_config_status(0)
    ,m_ssh_port(0)
{
}

CMasternode::~CMasternode()
{
}

QDataStream& operator>>(QDataStream& in, CMasternode& data)
{
    in>>data.m_num>>data.m_alias>>data.m_ip>>
            data.m_port>>data.m_mn_status>>data.m_ssh_user>>
            data.m_ssh_pwd>>data.m_remote_rpc_user>>data.m_remote_rpc_pwd>>
            data.m_remote_rpc_ip>>data.m_local_rpc_user>>
            data.m_local_rpc_pwd>>data.m_local_rpc_ip>>
            data.m_mn_key>>data.m_mn_hash>>data.m_index>>
            data.m_safe_conf_path>>data.m_mn_conf_path;
     return in;
}

QDataStream& operator<<(QDataStream& out, const CMasternode& data)
{
    out<<data.m_num<<data.m_alias<<data.m_ip<<
            data.m_port<<data.m_mn_status<<data.m_ssh_user<<
            data.m_ssh_pwd<<data.m_remote_rpc_user<<data.m_remote_rpc_pwd<<
            data.m_remote_rpc_ip<<data.m_local_rpc_user<<
            data.m_local_rpc_pwd<<data.m_local_rpc_ip<<
            data.m_mn_key<<data.m_mn_hash<<data.m_index<<
            data.m_safe_conf_path<<data.m_mn_conf_path;
     return out;
}

CMasternode CMasternode::DeSerializable(const QByteArray &datagram)
{
    QByteArray tmp_array = datagram;
    QBuffer buffer(&tmp_array);
    buffer.open(QIODevice::ReadOnly);

    QDataStream in(&buffer);
    CMasternode cmn;
    in>>cmn;
    buffer.close();
    return cmn;
}

QByteArray CMasternode::Serializable(const CMasternode &cmn)
{
    QByteArray tmp_array;
    QBuffer buffer(&tmp_array);
    buffer.open(QIODevice::WriteOnly);

    QDataStream out(&buffer);
    out<<cmn;
    buffer.close();
    return tmp_array;
}

