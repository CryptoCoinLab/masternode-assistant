#ifndef MASTERNODEDLG_H
#define MASTERNODEDLG_H
#include <QMainWindow>
#include <QSharedPointer>

#include "cmasternode.h"
#include "common/mntaskthread.h"

namespace Ui {
class MasternodeDlg;
}

class MasternodeDlg: public QMainWindow
{
    Q_OBJECT
public:
    explicit MasternodeDlg(QMainWindow *parent = 0);
    ~MasternodeDlg();
    void disableEditModel();
    void editModel();
    void addModel();
    void setMasternode(CMasternode cmn);
private:
    QString genNewMnName(QStringList currentNames);
signals:
    void sigSshMsg(MnSshTask);
private slots:
    void on_pb_close_clicked();
    void on_btnNewMn_clicked();
    void on_btnEditMn_clicked();
    void on_btnGenerateMNPrivateKey_clicked();
    void on_btnGetAddress_clicked();
    void on_btnFindCollateral_clicked();
    void cboMasternodesChange(const QString &strAlias);
    void chekMasternodeName();
    void on_btnBroadcastMn_clicked();
    void on_connect_test_clicked();

    void recTestConnect(QSharedPointer<MnSshTask> p);
private:
    Ui::MasternodeDlg *ui;
    CMasternode m_cmn;
    int  m_model;
};

#endif // MASTERNODEDLG_H
