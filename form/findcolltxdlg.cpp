#include "findcolltxdlg.h"
#include "ui_findcolltxdlg.h"
#include "cmasternodemodel.h"
#include "common/walletrpc.h"
#include "clocalsetting.h"

#include <QMessageBox>

#define _DEV_TEST

FindCollateralTxDlg::FindCollateralTxDlg(QString qsAddress, QDialog *parent) :
    m_address(qsAddress), QDialog(parent),m_OnlyAddr(true),
    ui(new Ui::FindCollateralTxDlg)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(QString(":/images/msgbox_ask.png")));
    setWindowTitle("选择抵押交易");

    m_btnGroup.setExclusive(true);
    ui->rdbFixed->setChecked(true);
    m_btnGroup.addButton(ui->rdbFixed);
    m_btnGroup.addButton(ui->rdbUnused);
    m_btnGroup.addButton(ui->rdbUsed);

    connect(&m_btnGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onRadioChange(QAbstractButton*)));

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setResizeContentsPrecision(
                QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView *headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true);

    ui->tableWidget->setColumnWidth(0,460);
    ui->tableWidget->setColumnWidth(1,40);
    ui->tableWidget->setColumnWidth(2,40);

    ui->tableWidget->setStyleSheet(
        "QTableWidget::item:selected{background-color:#AEEEEE}");
    // 先获取数据库中的所有
    QMapMasterNode qmmn =
        CMasternodeModel::initance()->queryAllMasternodes();
    QMapMasterNode::iterator immn =  qmmn.begin();
    int rows = 0;
    CMasternode cmn;
    while (immn != qmmn.end() )
    {
        cmn =immn.value();
        if (cmn.m_mn_hash != immn.key())
        {
            continue;
        }
        if (cmn.m_mn_status == "UNUSED")
        {
            m_unused[immn.key()] = QString::number(cmn.m_index);
        }
        else
        {
            m_used[immn.key()] = QString::number(cmn.m_index);
        }
        immn++;
    }

    // 在获取新的
    WalletRPC walletRpc("127.0.0.1",
        CLocalSetting::initance()->local_rpc_user,
        CLocalSetting::initance()->local_rpc_pwd);

    QJsonObject qjsonOutput = walletRpc.masternodeOutputs();
    for (QJsonObject::Iterator it = qjsonOutput.begin();
         it!=qjsonOutput.end(); it++)
    {
        if (it.value().toString()!="0"
            && it.value().toString()!="1")
        {
            continue;
        }
        m_qhtoi[it.key()] = it.value().toString();
    }

    QList<QJsonObject> lastTransations = walletRpc.listTransations();

    QString address, txid;
    for (QList<QJsonObject>::iterator it = lastTransations.begin();
         it!=lastTransations.end(); it++)
    {
        if (!(*it)["address"].isNull() && !(*it)["txid"].isNull())
        {
           address = (*it)["address"].toString();
           txid = (*it)["txid"].toString();
           if (address == m_address)
           {
               rows = ui->tableWidget->rowCount();
               ui->tableWidget->insertRow(rows);
               ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(txid));
               ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(m_qhtoi[txid]));
               ui->tableWidget->setItem(rows, 2, new QTableWidgetItem("可用"));
               m_fix[txid] = m_qhtoi[txid];
           }
        }
    }
}

FindCollateralTxDlg::~FindCollateralTxDlg()
{
    delete ui;
}

void FindCollateralTxDlg::on_btnCancle_clicked()
{
    this->close();
}

void FindCollateralTxDlg::on_btnSubmit_clicked()
{
    QList<QTableWidgetItem*> qlSelectItems =
                    ui->tableWidget->selectedItems();
    if (qlSelectItems.size())
    {
        m_qhash = qlSelectItems.at(0)->text();
        m_qIndex = qlSelectItems.at(1)->text();
    }
    this->accept();
}

bool FindCollateralTxDlg::getValues(QString &qhash, QString &qIndex)
{
    if (m_qhash!="" && m_qIndex!="")
    {
        qhash = m_qhash;
        qIndex = m_qIndex;
    }
    else
    {
        return false;
    }
    return true;
}

void FindCollateralTxDlg::onRadioChange(QAbstractButton* radioBtn)
{
    QMap<QString, QString> qmhash;
    QString qsStatus = "";
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(radioBtn == ui->rdbFixed)
    {
        qmhash = m_fix;
        qsStatus = "匹配";
    }
    else if (radioBtn == ui->rdbUnused)
    {
        qmhash = m_unused;
        qsStatus = "可用";
    }
    else if (radioBtn == ui->rdbUsed)
    {
        qmhash = m_used;
        qsStatus = "已用";
    }

    int rows = 0;
    for (QMap<QString, QString>::iterator it = qmhash.begin();
         it!=qmhash.end(); it++)
    {
        rows = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rows);
        ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(it.key()));
        ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(it.value()));
        ui->tableWidget->setItem(rows, 2, new QTableWidgetItem(qsStatus));
    }
}

