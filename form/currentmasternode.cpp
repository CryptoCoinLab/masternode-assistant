#include "currentmasternode.h"
#include "ui_currentmasternode.h"
#include <QFontDatabase>
#include <QDebug>
CurrentMasternode::CurrentMasternode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentMasternode)
{
    ui->setupUi(this);
    //引入图形字体
    int fontId = QFontDatabase::addApplicationFont(":/images/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(15);

    /*
    ui->label_alias->setFont(iconFont);
    ui->label_alias->setText(QChar(0xf136));

    ui->label_ip->setFont(iconFont);
    ui->label_ip->setText(QChar(0xf109));

    ui->label_port->setFont(iconFont);
    ui->label_port->setText(QChar(0xf1dc));
    */

}

CurrentMasternode::~CurrentMasternode()
{
    delete ui;
}

void CurrentMasternode::setValues(const CMasternode& cmn)
{
    m_cmn = cmn;
    if (m_cmn.m_mn_hash == "")
    {
        ui->lineEdit_alias->setText("");
        ui->lineEdit_ip->setText("");
        ui->lineEdit_port->setText("");
        ui->lineEdit_prikey->setText("");
        ui->lineEdit_addr->setText("");
        ui->lineEdit_hash->setText("");
        ui->lineEdit_id->setText("");
        ui->lineEdit_root->setText("");
        ui->lineEdit_pwd->setText("");
        ui->lineEdit_status->setText("");
    }
    else
    {
        ui->lineEdit_alias->setText(cmn.m_alias);
        ui->lineEdit_ip->setText(cmn.m_ip);
        ui->lineEdit_port->setText(QString::number(cmn.m_port));
        ui->lineEdit_prikey->setText(cmn.m_mn_key);
        ui->lineEdit_addr->setText("SAFE Address");
        ui->lineEdit_hash->setText(cmn.m_mn_hash);
        ui->lineEdit_id->setText(QString::number(cmn.m_index));
        ui->lineEdit_root->setText(cmn.m_ssh_user);
        ui->lineEdit_pwd->setText(cmn.m_ssh_pwd);
        ui->lineEdit_status->setText(cmn.m_mn_status);
    }
}

void CurrentMasternode::on_pb_edit_clicked()
{
    m_mndlg.setMasternode(m_cmn);
    m_mndlg.editModel();
    m_mndlg.show();
}
