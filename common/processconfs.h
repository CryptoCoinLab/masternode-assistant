#include <QString>
#include "cregistry.h"
#include "cmasternode.h"

class SafeConfEntity
{
public:
    QString qsListen;           /*exp: listen=1*/
    QString qsServer;           /*exp: server=1*/
    QString qsDaemon;           /*exp: daemon=1*/
    QString qsLogtimestamps;    /*exp: logtimestamps=1*/
    QString qsMaxconnections;   /*exp: maxconnections=256*/
    QString qsRpcallowip;    /*exp: rpcallowip=0.0.0.0/0*/
    QString qsRpcuser;          /*exp: rpcuser=bxj*/
    QString qsRpcpassword;      /*exp: rpcpassword=123456*/
};

class ProcessSafeConf
{
public:
    static int read(SafeConfEntity& safeConf, const QString &qsPath);
    static int write(const SafeConfEntity& safeConf, const QString &qsPath);
private:
    static QString keyToValues(const QString &qStrkey, const QString &qStrLine);
};

class ProcessMasternodeConf
{
public:
    static int query(QList<CMasternode> &qlMns, const QString &qsPaht);
    static int add(const QList<CMasternode> &qlMns, const QString &qsPaht);
    static int add(const CMasternode &cmn, const QString &qsPaht);
    static int reset(const CMasternode &oldMn, const CMasternode &newMn);
    static int del(const CMasternode &cMn);
};
