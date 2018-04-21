#ifndef MNMESSAGEDIALOG_H
#define MNMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class MnMessageDialog;
}

class MnMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MnMessageDialog(QString qStrMessage, QWidget *parent = 0);
    ~MnMessageDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MnMessageDialog *ui;
    QString m_qStrMessage;
};

#endif // MNMESSAGEDIALOG_H
