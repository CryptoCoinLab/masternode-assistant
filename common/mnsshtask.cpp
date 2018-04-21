#include "mnsshtask.h"
#include "sshtool.h"

MnSshTask::MnSshTask(QObject *parent)
    :QObject(parent), m_callback(NULL), m_param(NULL), m_pSender(NULL)
{
}

MnSshTask::MnSshTask(const MnSshTask& mnst)
{
    m_qsCmds = mnst.m_qsCmds;
    m_qsRetValues = mnst.m_qsRetValues;
    m_strIp = mnst.m_strIp;
    m_strUser = mnst.m_strUser;
    m_strPwd = mnst.m_strPwd;
    m_type = mnst.m_type;
    m_pSender = mnst.m_pSender;
}

MnSshTask& MnSshTask::operator=(const MnSshTask& mnst)
{
    if (this == &mnst)
    {
        return *this;
    }
    m_qsCmds = mnst.m_qsCmds;
    m_qsRetValues = mnst.m_qsRetValues;
    m_strIp = mnst.m_strIp;
    m_strUser = mnst.m_strUser;
    m_strPwd = mnst.m_strPwd;
    m_type = mnst.m_type;
    m_pSender = mnst.m_pSender;
    return *this;
}

void MnSshTask::bindFunc(qSshFunc func, QObject * paramer)
{
    m_callback = func;
    m_param = paramer;
}

MnSshTask::~MnSshTask()
{
}

MnsshtaskRunnable::MnsshtaskRunnable(QSharedPointer<MnSshTask> pSshTask, QString qsMsg)
{
    this->qsMsg = qsMsg;
    this->mnSshTask = pSshTask;
}

MnsshtaskRunnable::~MnsshtaskRunnable()
{
}

void MnsshtaskRunnable::run()
{
    if (mnSshTask == NULL)
        return;
    try
    {
        qDebug() << "Current is :" << QThread::currentThread();
        SshTool ssh;
        int ret = 0;
        ret = ssh.initSession(mnSshTask->m_strIp, mnSshTask->m_strUser, mnSshTask->m_strPwd);
        if (ret != 0)
        {
            mnSshTask->m_qsRetValues = "INITSESSION_ERROR";
            emit (mnSshTask->sigSshMsg(mnSshTask));
        }
        else
        {
            if (mnSshTask->m_type == "EXEC")
            {
                for (int i=0; i<mnSshTask->m_qsCmds.size(); i++)
                {
                    ret = ssh.exec(mnSshTask->m_qsCmds.at(i), mnSshTask->m_qsRetValues);
                    if (ret != 0)
                    {
                        if (ret == LIBSSH2_ERROR_TIMEOUT)
                        {
                            mnSshTask->m_qsRetValues = "SSH_EXEC_TIMEOUT";
                        }
                        else
                        {
                            mnSshTask->m_qsRetValues = "SSH_EXEC_ERROR";
                        }
                        emit (mnSshTask->sigSshMsg(mnSshTask));
                    }
                    else
                    {
                        emit (mnSshTask->sigSshMsg(mnSshTask));
                        if (mnSshTask->m_callback != NULL)
                        {
                            (*mnSshTask->m_callback)(mnSshTask->m_param);
                        }
                    }
                }
            }
        }
    }
    catch(...)
    {
        qDebug()<< "throw errror from ssh.";
    }
}

// 注册一下，否则connect 会失败
int metatype_id = qRegisterMetaType<QSharedPointer<MnSshTask> >("QSharedPointer<MnSshTask>");
