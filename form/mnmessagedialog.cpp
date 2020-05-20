#include "mnmessagedialog.h"
#include "ui_mnmessagedialog.h"

MnMessageDialog::MnMessageDialog(QString qStrMessage, QWidget *parent) :
    m_qStrMessage(qStrMessage),QDialog(parent),ui(new Ui::MnMessageDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(QString(":/images/msgbox_ask.png")));
    setWindowTitle("提示");
    ui->textEdit->setText(m_qStrMessage);
}

MnMessageDialog::~MnMessageDialog()
{
    delete ui;
}

void MnMessageDialog::on_pushButton_clicked()
{
    this->accept();
}
