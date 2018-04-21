#ifndef MNSSHTASK_H
#define MNSSHTASK_H
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QSharedPointer>
#include <QMetaType>

typedef void (*qSshFunc)(QObject*);

class MnSshTask : public QObject
{
    Q_OBJECT

public:
    MnSshTask(QObject *parent = 0);
    MnSshTask(const MnSshTask& mnst);
    MnSshTask& operator=(const MnSshTask& mnst);
    void bindFunc(qSshFunc func, QObject* paramer);
    ~MnSshTask();
signals:
    void sigSshMsg(QSharedPointer<MnSshTask> pTsk);
public:
    QStringList m_qsCmds;
    QString m_qsRetValues;
    QString m_strIp;
    QString m_strUser;
    QString m_strPwd;
    QString m_type;
    qSshFunc m_callback;
    QObject *m_param;
    QObject *m_pSender;
};

class MnsshtaskRunnable : public QRunnable
{
public:
    MnsshtaskRunnable(QSharedPointer<MnSshTask> pSshTask, QString qsMsg="");
    ~MnsshtaskRunnable();
    void run();
private:
    QString qsMsg;
    QSharedPointer<MnSshTask> mnSshTask;
};

#endif // MNSSHTASK_H
