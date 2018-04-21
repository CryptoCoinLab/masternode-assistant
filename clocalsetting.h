#ifndef CLOCALSETTING_H
#define CLOCALSETTING_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QThread>

//using namespace std;

class CLocalSetting
{
protected:
    CLocalSetting();
public:
    static CLocalSetting* initance();
public:
    QString safe_conf_path;
    QString safe_conf_file;
    QString new_safe_conf_files_path;
    QString masternode_conf_path;
    QString masternode_conf_file;
    QString local_rpc_user;
    QString local_rpc_pwd;
    QString remote_rpc_user;
    QString remote_rpc_pwd;
    QString remote_rpc_ip;
    QString local_script_path;
    QString remote_script_path;
    QString install_script;
    QString start_script;
    QString restart_script;
    QString stop_script;
    QString reset_rpc_script;

    QMap<QString, QString> mn_old_info;
    QMap<QString, QString> mn_new_info;
    QStringList  mn_alias;  // 保存别名，防止别名冲突在钱包中不显示

private:
    static CLocalSetting p;
};

typedef enum {UNLOAD=0,UPLOADED,STARTING,STARTED} E_STATUS;

extern CLocalSetting *local_setting;
extern QString g_current_ip;
extern QString g_current_tx_hash;
extern QString MNSTATUS[];
extern QMap<QString, E_STATUS> M_STATUS;

#endif // CLOCALSETTING_H
