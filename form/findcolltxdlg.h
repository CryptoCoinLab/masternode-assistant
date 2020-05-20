#ifndef FINDCOLLTXDLG_H
#define FINDCOLLTXDLG_H

#include <QDialog>
#include <QMap>
#include <QButtonGroup>

namespace Ui {
class FindCollateralTxDlg;
}

class FindCollateralTxDlg : public QDialog
{
    Q_OBJECT
public:
    explicit FindCollateralTxDlg(QString qsAddress, QDialog *parent = 0);
    ~FindCollateralTxDlg();
    bool getValues(QString &qhash, QString &qIndex);

private slots:
    void on_btnCancle_clicked();
    void on_btnSubmit_clicked();
    void onRadioChange(QAbstractButton*);

private:
    Ui::FindCollateralTxDlg *ui;
    QString m_qhash;
    QString m_qIndex;
    QString m_address;
    QMap<QString, QString> m_qhtoi;
    QMap<QString, QString> m_fix;
    QMap<QString, QString> m_unused;
    QMap<QString, QString> m_used;
    bool m_OnlyAddr;
    QButtonGroup m_btnGroup;
};

#endif // FINDCOLLTXDLG_H
