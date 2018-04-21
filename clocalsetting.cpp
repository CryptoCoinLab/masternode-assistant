#include "clocalsetting.h"
#include "common/cregistry.h"
#include "localrpcdialog.h"
#include "cmasternode.h"
#include "cmasternodemodel.h"
#include "common/walletrpc.h"
#include "common/processconfs.h"

#include <QMessageBox>

CLocalSetting::CLocalSetting()
{
    M_STATUS["UNLOAD"] = UNLOAD;
    M_STATUS["UPLOADED"] = UPLOADED;
    M_STATUS["STARTING"] = STARTING;
    M_STATUS["STARTED"] = STARTED;

    //@todo read rpc from /safe.conf
    new_safe_conf_files_path = "./safeconf/";
    local_script_path = "./script/";
    install_script = "auto_install_safe.py";
    start_script = "masternode_start.sh";
    restart_script = "masternode_restart.sh";
    stop_script = "masternode_stop.sh";
    reset_rpc_script = "reset_rpc.py";

    if(!CRegistry::readDataDir(safe_conf_path))
    {

        QMessageBox::warning(NULL,QString("提示"),
                     QString("未能从注册表中获取钱包路径，请查看是否正确安装钱包。"));
        exit(0);
    }

    QString localSafeConfFile = safe_conf_path + "\\safe.conf";
    SafeConfEntity safeEntity;
    ProcessSafeConf::read(safeEntity, localSafeConfFile);
    local_rpc_user = safeEntity.qsRpcuser;
    local_rpc_pwd = safeEntity.qsRpcpassword;

    masternode_conf_file = safe_conf_path + "/masternode.conf";
    QFile mnfile(masternode_conf_file);
    if(mnfile.exists())
    {
        if(mnfile.open(QIODevice::ReadWrite))
        {
            CMasternode cmn;
            while(true)
            {
                QByteArray mn_conf_line =  mnfile.readLine();
                if(mn_conf_line.size()>0)
                {
                    if (mn_conf_line.at(0) == '#')
                    {
                        continue;
                    }

                    QList<QByteArray> qList = mn_conf_line.split(' ');
                    if(qList.size() > 4)
                    {
                        mn_old_info[qList[3]] = qList[4]; // 保存hash与索引
                        mn_alias<<qList[0];  // 保存别名
                        cmn.m_alias = qList[0];
                        QList<QByteArray> qIpAndPort = qList[1].split(':');
                        if (qIpAndPort.size() > 1)
                        {
                            cmn.m_ip = qIpAndPort[0];
                            cmn.m_port = qIpAndPort[1].toInt();
                        }
                        else
                        {
                            cmn.m_ip = "0.0.0.0";
                            cmn.m_port = 5555;
                        }
                        cmn.m_mn_key = qList[2];
                        cmn.m_mn_hash = qList[3];
                        cmn.m_index = qList[4].toInt();
                        cmn.m_config_status = 2;
                        pMnDb->insertMasternode(cmn);
                    }
                }
                else
                {
                    break;
                }
            }
            mnfile.close();

        }
        else
        {
            /*
            QMessageBox::information(this,tr("提示"),
                         tr("打开masternode.conf配置文件失败，请检查改文件是否被占用。"));
            */
        }
    }
    else
    {
        /*
        QMessageBox::information(this,tr("提示"),
                     tr("未找到masternode.conf文件。"));
        */
    }

    // 获取Masternode outputs的输出
    WalletRPC walletRpc("127.0.0.1", local_rpc_user, local_rpc_pwd);
    QJsonObject qjsonOutput = walletRpc.masternodeOutputs();

    for (QJsonObject::Iterator it = qjsonOutput.begin();
         it!=qjsonOutput.end(); it++)
    {
        qDebug()<<"key:"<<it.key();
        qDebug()<<"value:"<<it.value().toString();
        if (it.value().toString()!="0"
                && it.value().toString()!="1")
        {
            continue;
        }
        if (mn_old_info.find(it.key()) == mn_old_info.end())
        {
            qDebug()<<"not in mn.conf"<<it.key();
            if (pMnDb->selectMasternode(it.key()).m_mn_hash != "")
            {
                // masternode.conf没有，DB里面有，以前配置过，被用户删了
                // 怎么处理？
                // 1 提示用户是否恢复，如果恢复，从数据库读取数据写入masternode.conf
            }
            else
            {
                CMasternode cmn;
                cmn.m_mn_hash = it.key();
                cmn.m_config_status = 1;
                pMnDb->insertMasternode(cmn);
            }
        }
    }

}

CLocalSetting* CLocalSetting::initance()
{
    static CLocalSetting p;
    return &p;
}

CLocalSetting *local_setting;
QString g_current_ip;
QString g_current_tx_hash;
QString MNSTATUS[] = {"UNLOAD","UPLOADED","STARTING","STARTED"};
QMap<QString, E_STATUS> M_STATUS;


