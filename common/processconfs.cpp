#include "processconfs.h"
#include <QFile>

int ProcessSafeConf::read(SafeConfEntity& safeConf, const QString &qsPath)
{
    QFile file(qsPath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadWrite))
        {
            QByteArray safe_conf_line =  file.readLine();
            while(safe_conf_line.size())
            {
                if (safe_conf_line.at(0) == '#')
                {
                    safe_conf_line =  file.readLine();
                    continue;
                }
                if (safeConf.qsDaemon == "")
                {
                    safeConf.qsDaemon = keyToValues("daemon", safe_conf_line);
                    if (safeConf.qsDaemon!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsListen == "")
                {
                    safeConf.qsListen = keyToValues("listen", safe_conf_line);
                    if (safeConf.qsListen!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsLogtimestamps == "")
                {
                    safeConf.qsLogtimestamps = keyToValues("logtimestamps", safe_conf_line);
                    if (safeConf.qsLogtimestamps!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsMaxconnections == "")
                {
                    safeConf.qsMaxconnections = keyToValues("maxconnections", safe_conf_line);
                    if (safeConf.qsMaxconnections!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsRpcallowip == "")
                {
                    safeConf.qsRpcallowip = keyToValues("rpcallowip", safe_conf_line);
                    if (safeConf.qsRpcallowip!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsRpcpassword == "")
                {
                    safeConf.qsRpcpassword = keyToValues("rpcpassword", safe_conf_line);
                    if (safeConf.qsRpcpassword!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsRpcuser == "")
                {
                    safeConf.qsRpcuser = keyToValues("rpcuser", safe_conf_line);
                    if (safeConf.qsRpcuser!="")
                    {
                        continue;
                    }
                }
                if (safeConf.qsServer == "")
                {
                    safeConf.qsServer = keyToValues("server", safe_conf_line);
                    if (safeConf.qsServer!="")
                    {
                        continue;
                    }
                }
                safe_conf_line =  file.readLine();
            }
            file.close();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int ProcessSafeConf::write(const SafeConfEntity& safeConf, const QString &qsPath)
{
    int ret = 0;
    QFile file(qsPath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadWrite))
        {
            QString qStrLine = QString("listen=") + safeConf.qsListen + QString("\r\n");
            qStrLine += (QString("server=") + safeConf.qsServer + "\r\n");
            qStrLine += (QString("daemon=") + safeConf.qsDaemon + "\r\n");
            qStrLine += (QString("logtimestamps=") + safeConf.qsLogtimestamps + "\r\n");
            qStrLine += (QString("maxconnections=") + safeConf.qsMaxconnections + "\r\n");
            qStrLine += (QString("rpcallowip=") + safeConf.qsRpcallowip + "\r\n");
            qStrLine += (QString("rpcuser=") + safeConf.qsRpcuser + "\r\n");
            qStrLine += (QString("rpcpassword=") + safeConf.qsRpcpassword + "\r\n");
            file.write(qStrLine.toStdString().c_str());
            file.close();
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

QString ProcessSafeConf::keyToValues(const QString &qStrkey, const QString &qStrLine)
{
    QString qStrValuse = "";
    if (qStrLine.startsWith(qStrkey))
    {
        int begin = qStrLine.indexOf("=",0);
        int end = qStrLine.indexOf("\r",0);
        if (begin < --end)
        {
            qStrValuse = qStrLine.mid(++begin,--end);
            qStrValuse.replace("\r\n", "\0");
        }
    }
    return qStrValuse;
}

int ProcessMasternodeConf::query(QList<CMasternode> &qlMns, const QString &qsPaht)
{

    return 0;
}
int ProcessMasternodeConf::add(const QList<CMasternode> &qlMns, const QString &qsPaht)
{

    return 0;
}
int ProcessMasternodeConf::add(const CMasternode &cmn, const QString &qsPaht)
{

    return 0;
}
int ProcessMasternodeConf::reset(const CMasternode &oldMn, const CMasternode &newMn)
{

    return 0;
}
int ProcessMasternodeConf::del(const CMasternode &cMn)
{

    return 0;
}
