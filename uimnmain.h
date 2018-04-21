#ifndef UIMNMAIN_H
#define UIMNMAIN_H

#include <QDialog>
#include <QTableWidgetItem>
#include <map>
#include "frametitlebar.h"
#include "cmasternode.h"
#include "mnwizard.h"
#include "changedialog.h"
#include "cstartthread.h"
#include "common/database.h"
#include "common/mntaskthread.h"


// @new
#include "cmasternodemodel.h"
#include "form/masternodePanel.h"
#include "form/masternodedlg.h"

using namespace std;

namespace Ui {
class UIMnMain;
}

class UIMnMain : public QDialog
{
    Q_OBJECT
    typedef enum {E_INFO=0, E_MESSAGE, E_ERROR} E_MESSAGE_LEVEL;
protected:
    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);
public:
    explicit UIMnMain(QWidget *parent = 0);
    ~UIMnMain();
signals:
    void sigMasternodeSynOver();

private:
    void insertTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(int nIndex);
    void initSetting();
    void initDatabase();
    void initPanelWidget();

private:
    Ui::UIMnMain *ui;
    bool max;
    QRect location;
    MnWizard mnwizard;
    MasternodeDlg mndlg;
    ChangeDialog changedialog;
    QString current_ip;
    CMasternode current_mn;
    QTimer *timer;
    QTimer *hlth_timer;
    CStartThread mns;
    Database mydb;
    bool isGenLocalSafeByProgram;
    int nTotalBlock;
    int nCurrentBlock;
    QList<QWidget *> frms;
    CMasternode selectMn;

private slots:
    void initForm();
    void buttonClick();
    void recvMnInfo(const CMasternode &cmn);
    void recvChangeMnInfo(const CMasternode &cmn);
    void show_masternode(const CMasternode &cmn);
    void GetMasternodeInfo();
    void CheckMasternodeFunctioning();
    void ShowMasternodeStatus();
    void mnSetupComplete();
    void doSynOver();
    void selectMasternodeList(QListWidgetItem *selectNode);
    void showProcessMessage(const QString &msg,
                            E_MESSAGE_LEVEL ml=E_INFO);
    void ShowMnToMessageBox(const CMasternode &cmn);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pb_add_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_pb_remove_clicked();
    void on_pb_upload_clicked();
    void on_pb_start_clicked();
    void on_pb_rechange_clicked();
    void on_pb_stop_clicked();
    void on_pb_add_new_clicked();
};

#endif // UUIMAIN_H
