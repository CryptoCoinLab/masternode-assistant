#ifndef CMASTERNODEMODEL_H
#define CMASTERNODEMODEL_H

#include <QObject>
#include <QMap>
#include "cmasternode.h"
#include "common/database.h"

typedef QMap<QString, CMasternode> QMapMasterNode;
typedef QMap<QString, QString> QMapKeyToValuse;

class CMasternodeModel : public QObject
{
     Q_OBJECT
protected:
    CMasternodeModel(QObject *parent = 0);
    ~CMasternodeModel();
    void setNewMnValue(const CMasternode& cmn);
    void delMnValue(const CMasternode& cmn);
public:
    static CMasternodeModel* initance();
    int insertMasternode(const CMasternode& cmn);
    int deleteMasternode(const QString& qsHash);
    int updateMasternode(const CMasternode& cmn);
    CMasternode selectMasternode(const QString& qsHash);
    const QMapMasterNode& queryAllMasternodes();

    QString queryIp(const QString& qsAlias);
    QString queryHash(const QString& qsIp);
    bool findAlias(const QString& qsIp);
    QStringList findAllAlias();
signals:
    void sigMasternodeInsert(CMasternode& cmn);
    void sigMasternodeDelete(CMasternode& cmn);
    void sigMasternodeUpdate(CMasternode& cmn);
private:
    QMapMasterNode qmapMns;
    QMapKeyToValuse qmapIpToHsh;
    QMapKeyToValuse qmapAliasToIp;
    QStringList qlistAlias;
    static CMasternodeModel p;
    static Database *ptrdb;
};

#define pMnDb CMasternodeModel::initance()

#endif // CMASTERNODEMODEL_H
