#include "masternodedlg.h"
#include "ui_masternodedlg.h"
#include "cmasternodemodel.h"
#include "common/walletrpc.h"
#include "clocalsetting.h"
#include "findcolltxdlg.h"
#include "common/mntaskthread.h"
#include "mnmessagedialog.h"
#include <QMessageBox>
#include <QDateTime>

extern Mntaskthread *g_workThread;

MasternodeDlg::MasternodeDlg(QMainWindow *parent) :
    QMainWindow(parent), ui(new Ui::MasternodeDlg)

{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowSystemMenuHint
                         | Qt::WindowMinMaxButtonsHint);
    setWindowIcon(QIcon(QString(":/images/troublecheck.png")));
    setWindowTitle("配置主节点");

    ui->edtMnPort->setText("5555");
    QStringList als = CMasternodeModel::initance()->findAllAlias();
    ui->cboMasternodes->addItems(als);
    connect(ui->cboMasternodes, &QComboBox::currentTextChanged,
            this, &MasternodeDlg::cboMasternodesChange);
    connect(ui->edtMnName, &QLineEdit::editingFinished, this,
            &MasternodeDlg::chekMasternodeName);
}


MasternodeDlg::~MasternodeDlg()
{
    delete ui;
}
void MasternodeDlg::chekMasternodeName()
{
    QStringList als = CMasternodeModel::initance()->findAllAlias();
    QString qNewName = ui->edtMnName->text();
    if (als.contains(qNewName)
            && (qNewName!=ui->cboMasternodes->currentText()))
    {
        ui->edtMnName->setText(ui->cboMasternodes->currentText());
        ui->edtMnName->setFocus();
        ui->statusBar->showMessage("别名重复,请重新设置别名...", 5000);
    }
}

void MasternodeDlg::cboMasternodesChange(const QString &strAlias)
{
    QString qsIp = CMasternodeModel::initance()->queryIp(strAlias);
    if (qsIp != "")
    {
        QString qsHash = CMasternodeModel::initance()->queryHash(qsIp);
        if (qsHash != "")
        {
            CMasternode cmn =
                CMasternodeModel::initance()->selectMasternode(qsHash);
            if (cmn.m_mn_hash != "")
            {
                setMasternode(cmn);
                disableEditModel();
            }
        }
    }
    else
    {
        // new
    }
}

void MasternodeDlg::setMasternode(CMasternode cmn)
{
    m_cmn = cmn;
    if (cmn.m_mn_hash == "")
    {
        m_cmn.m_port = 5555;
        m_cmn.m_index = 0;
        m_cmn.m_ssh_user = "root";
        m_cmn.m_ssh_port = 22;
        m_cmn.m_alias = cmn.m_alias;
    }

    ui->edtMnName->setText(m_cmn.m_alias);
    ui->edtMnIp->setText(m_cmn.m_ip);
    ui->edtMnPort->setText(QString::number((m_cmn.m_port)));
    ui->edtMnPrivateKey->setText(m_cmn.m_mn_key);
    ui->edtMnCollateralTx->setText(m_cmn.m_mn_hash);
    ui->edtMnCollateralTxIndex->setText(QString::number((m_cmn.m_index)));
    ui->edtMnSshUser->setText(m_cmn.m_ssh_user);
    ui->edtMnSshPwd->setText(m_cmn.m_ssh_pwd);
    ui->edtMnSshPort->setText(QString::number((m_cmn.m_ssh_port)));

    // 选择当前显示的字段
    ui->cboMasternodes->setCurrentText(m_cmn.m_alias);
}

void MasternodeDlg::disableEditModel()
{
    ui->edtMnName->setEnabled(false);
    ui->edtMnIp->setEnabled(false);
    ui->edtMnPort->setEnabled(false);
    ui->edtMnPrivateKey->setEnabled(false);
    ui->edtMnCollateralTx->setEnabled(false);
    ui->edtMnCollateralTxIndex->setEnabled(false);
    ui->edtMnSshUser->setEnabled(false);
    ui->edtMnSshPwd->setEnabled(false);
    ui->edtMnSshPort->setEnabled(false);
    ui->edtMnCollateralAddress->setEnabled(false);

    ui->btnFindCollateral->setDisabled(true);
    ui->btnGenerateMNPrivateKey->setDisabled(true);
    ui->btnGetAddress->setDisabled(true);
    ui->btnSaveChange->setVisible(false);
}

void MasternodeDlg::addModel()
{
    m_model = 0;
    m_cmn = CMasternode();
    m_cmn.m_port = 5555;
    m_cmn.m_index = 0;
    m_cmn.m_ssh_user = "root";
    m_cmn.m_ssh_port = 22;

    QStringList als = CMasternodeModel::initance()->findAllAlias();
    QString qln = genNewMnName(als);
    m_cmn.m_alias = qln;

    ui->cboMasternodes->setCurrentText(m_cmn.m_alias);
    ui->edtMnName->setText(m_cmn.m_alias);
    ui->edtMnIp->setText(m_cmn.m_ip);
    ui->edtMnPort->setText(QString::number(m_cmn.m_port));
    ui->edtMnPrivateKey->setText(m_cmn.m_mn_key);
    ui->edtMnCollateralTx->setText(m_cmn.m_mn_hash);
    ui->edtMnCollateralTxIndex->setText(QString::number(m_cmn.m_index));
    ui->edtMnSshUser->setText(m_cmn.m_ssh_user);
    ui->edtMnSshPwd->setText(m_cmn.m_ssh_pwd);
    ui->edtMnSshPort->setText(QString::number(m_cmn.m_ssh_port));

    ui->edtMnName->setEnabled(true);
    ui->edtMnIp->setEnabled(true);
    ui->edtMnPort->setEnabled(true);
    ui->edtMnPrivateKey->setEnabled(true);
    ui->edtMnCollateralTx->setEnabled(true);
    ui->edtMnCollateralTxIndex->setEnabled(true);
    ui->edtMnSshUser->setEnabled(true);
    ui->edtMnSshPwd->setEnabled(true);
    ui->edtMnSshPort->setEnabled(true);
    ui->edtMnCollateralAddress->setEnabled(true);

    ui->btnFindCollateral->setDisabled(false);
    ui->btnGenerateMNPrivateKey->setDisabled(false);
    ui->btnGetAddress->setDisabled(false);
    ui->btnSaveChange->setVisible(false);
}

void MasternodeDlg::editModel()
{
    m_model = 1;
    QStringList als = CMasternodeModel::initance()->findAllAlias();
    if (!als.contains(ui->edtMnName->text()))
    {
        // 还没添加成功，状态都是可以改的
        return;
    }

    ui->edtMnName->setEnabled(true);
    ui->edtMnIp->setEnabled(true);
    ui->edtMnPort->setEnabled(true);
    ui->edtMnPrivateKey->setEnabled(true);
    ui->edtMnCollateralTx->setEnabled(true);
    ui->edtMnCollateralTxIndex->setEnabled(true);
    ui->edtMnSshUser->setEnabled(true);
    ui->edtMnSshPwd->setEnabled(true);
    ui->edtMnSshPort->setEnabled(true);
    ui->edtMnCollateralAddress->setEnabled(true);

    ui->btnFindCollateral->setDisabled(true);
    ui->btnGenerateMNPrivateKey->setDisabled(true);
    ui->btnGetAddress->setDisabled(true);
    ui->btnSaveChange->setVisible(true);
}

void MasternodeDlg::on_pb_close_clicked()
{
    this->close();
}

void MasternodeDlg::on_btnNewMn_clicked()
{
     addModel();
}

void MasternodeDlg::on_btnEditMn_clicked()
{
    editModel();
}


QString MasternodeDlg::genNewMnName(QStringList currentNames)
{
    QString qln = "mn";
    for (int i=1; i<1000; i++)
    {
        qln += QString::number(i);
        if (currentNames.contains(qln, Qt::CaseInsensitive))
        {
            qln.remove(2,qln.length()-2);
            continue;
        }
        break;
    }

    if (qln != "mn")
    {
        if (ui->cboMasternodes->findText(qln) < 0)
        {
            ui->cboMasternodes->addItem(qln);
        }
        else
        {
            qDebug()<< "found..."<<qln;
        }
    }
    else
    {
        QDateTime time = QDateTime::currentDateTime();
        qln  = "mn_" + time.toString("yyyy-MM-dd[hh:mm:ss]");
        ui->cboMasternodes->addItem(qln);
    }
    return qln;
}

void MasternodeDlg::on_btnGenerateMNPrivateKey_clicked()
{
    WalletRPC walletRpc("127.0.0.1",
        CLocalSetting::initance()->local_rpc_user,
        CLocalSetting::initance()->local_rpc_pwd);

    QString qsGenKey = walletRpc.masternodeGenkey();
    if (qsGenKey != "")
    {
        ui->edtMnPrivateKey->setText(qsGenKey);
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),
                     tr("请检查SAFE钱包程序是否已经打开。"));
        return;
    }
}

void MasternodeDlg::on_btnGetAddress_clicked()
{
    WalletRPC walletRpc("127.0.0.1",
        CLocalSetting::initance()->local_rpc_user,
        CLocalSetting::initance()->local_rpc_pwd);

    QString qsAdress = walletRpc.getaccountaddress(ui->edtMnName->text());
    if(qsAdress != "")
    {
        ui->edtMnCollateralAddress->setText(qsAdress);
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),
            tr("调用RPC获取地址失败，请检查SAFE钱包程序是否已经打开。"));
        return;
    }
}

void MasternodeDlg::on_btnFindCollateral_clicked()
{
    FindCollateralTxDlg fctd(ui->edtMnCollateralAddress->text());
    fctd.exec();
    QString qHash, qIndex;
    if(fctd.getValues(qHash, qIndex))
    {
        ui->edtMnCollateralTx->setText(qHash);
        ui->edtMnCollateralTxIndex->setText(qIndex);
    }
}

void MasternodeDlg::on_btnBroadcastMn_clicked()
{
    // 启动节点
    CMasternode cmn;
    QByteArray blocksafeconf;
    QByteArray blockmnconf;

    //
    //g_workThread->insertTask(QRunnable task);

    /*
    QByteArray mnalias = field("mnalias").toByteArray();

    QByteArray host = field("host").toByteArray();
    QByteArray port = field("port").toByteArray();
    QByteArray user = field("user").toByteArray();
    QByteArray pwd = field("pwd").toByteArray();
    QByteArray rpcuser = field("rpcUser").toByteArray();
    QByteArray rpcpwd = field("rpcPwd").toByteArray();
    QByteArray rpcip=  field("rpcIp").toByteArray();

    QByteArray mnkey= field("masternodekey").toByteArray();
    QByteArray mnclltrlh = field("collateralhash").toByteArray();
    QByteArray mnclltrlindex = field("collateralindex").toByteArray();
    QByteArray safeconf = field("safeconf").toByteArray();
    QByteArray mnconf = field("masternodeconf").toByteArray();

    cmn.m_alias = QString(mnalias);
    cmn.m_ip = QString(host);
    cmn.m_port = port.toShort();
    cmn.m_ssh_user = QString(user);
    cmn.m_ssh_pwd = QString(pwd);
    cmn.m_remote_rpc_user = QString(rpcuser);
    cmn.m_remote_rpc_pwd = QString(rpcpwd);
    cmn.m_remote_rpc_ip = cmn.m_ip;

    cmn.m_mn_key = QString(mnkey);
    cmn.m_mn_hash = QString(mnclltrlh);
    cmn.m_index = mnclltrlindex.toShort();
    cmn.m_safe_conf_path = QString(safeconf);
    cmn.m_mn_conf_path = QString(mnconf);

    emit sigMasternodeAdd(cmn);

    blocksafeconf += "rpcuser=" + rpcuser + "\n";
    blocksafeconf += "rpcpassword=" + rpcpwd + "\n";
    blocksafeconf += "rpcallowip=0.0.0.0/0\n";
    blocksafeconf += "listen=1\n"\
                       "server=1\n"\
                       "daemon=1\n"\
                       "logtimestamps=1\n"\
                       "maxconnections=256\n"\
                       "masternode=1\n";
    blocksafeconf += "masternodeprivkey="+mnkey+"\n";
    blocksafeconf += "externalip="+host+":"+QString(port)+"\n";

    QDir qDit;
    if (!qDit.exists("./safeconf"))
    {
        qDit.mkdir("./safeconf");
    }

    QFile headerFile("./safeconf/" + QString(host));

    if (!headerFile.open(QFile::WriteOnly | QFile::Text ))
    {
        QMessageBox::warning(this,tr("提示"),
                     tr("保存文件时出现错误，请查看safeconf目录否被占用"));
        return;
    }

    headerFile.write(blocksafeconf);
    blocksafeconf.clear();
    headerFile.close();

    QString qMnconf = "";
    if(CRegistry::readDataDir(qMnconf))
    {
         QDir qDitMn;
         if (qDitMn.exists(qMnconf+"\\masternode.conf"))
         {
             QFile mnFile(qMnconf+"\\masternode.conf");
             if (!mnFile.open(QFile::Text | QFile::ReadWrite))
             {
                 QMessageBox::warning(this,tr("提示"),
                              tr("请查看masternode.conf是否存在，且未被其他程序占用。"));
                 return;
             }

             QString qbData = mnFile.readAll();

             // 如果配置文件没有回车或者换行作为结尾，你们给他加一个。
             if (qbData.size() > 0)
             {
                 if (qbData.at(qbData.size()-1) != '\n'
                         && qbData.at(qbData.size()-1) != '\r' )
                 {
                     blockmnconf += "\n";
                 }
             }

             blockmnconf += QString(mnalias) + QString(" ") + QString(host)
                     + QString(":") + QString(port) + QString(" ");
             blockmnconf += QString(mnkey) + QString(" ") + QString(mnclltrlh)
                     + QString(" ") + QString(mnclltrlindex) + QString("\n");

             if (qbData.contains(mnclltrlh))
             {
                 qbData.replace(QRegExp("[^\\n]?\\S+\\s+\\S+:5555\\s+\\S+\\s+"
                                        +mnclltrlh+"\\s+\\d+[\\n]?"),QString(blockmnconf));
                 mnFile.resize(0);
                 mnFile.write(qbData.toLocal8Bit());
             }
             else
             {
                 mnFile.seek(mnFile.size());
                 mnFile.write(blockmnconf);
             }

             mnFile.close();
             local_setting->mn_alias<<mnalias;  // 将别名放入别名列表，防止连续添加出现相同的别名
         }
         else
         {
             //qDebug()<<"can not find masternode.conf...";
             QMessageBox::warning(this,tr("提示"),
                          tr("请查看masternode.conf是否存在,如果不存在请重启钱包，或者手动添加。"));
         }
    }
    else
    {
        qDebug()<<"Can not find masternode.conf...";
        QMessageBox::warning(this,tr("提示"),
                     tr("未能从注册表中获取masternode.conf文件的位置。"\
                        "检查是否正确安装SAFE钱包。"));
    }

    */
}

// @智能指针解决, New不需要释放内存。
void MasternodeDlg::on_connect_test_clicked()
{
    MnSshTask *sshTask = new MnSshTask();
    QSharedPointer<MnSshTask> mnSshTask(sshTask);
    mnSshTask->m_strIp = "192.168.12.132";
    mnSshTask->m_strUser = "root";
    mnSshTask->m_strPwd = "`1q`1q`1q";
    mnSshTask->m_type = "EXEC";
    mnSshTask->m_qsCmds.append("uname -a");

    connect(mnSshTask.data(), &MnSshTask::sigSshMsg, this, &MasternodeDlg::recTestConnect);
    g_workThread->insertTask(new MnsshtaskRunnable(mnSshTask));
}

void MasternodeDlg::recTestConnect(QSharedPointer<MnSshTask> pRecTask)
{
    qDebug() << "debug recTestConnect: "<< pRecTask->m_qsRetValues;
    MnMessageDialog md(pRecTask->m_qsRetValues);
    md.exec();
}
