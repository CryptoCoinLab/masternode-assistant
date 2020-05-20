#include "cmasternodemodel.h"

Database* CMasternodeModel::ptrdb;

CMasternodeModel::CMasternodeModel(QObject *parent)
    : QObject(parent)
{
    try
    {
        ptrdb = new Database();
        if (ptrdb!=NULL)
        {
            QMap<QString, QByteArray> qmDatas = ptrdb->queryData();
            for(QMap<QString, QByteArray>::iterator
                qit=qmDatas.begin();
                qit!=qmDatas.end(); qit++)
            {
                CMasternode cmn = CMasternode::DeSerializable(qit.value());
                setNewMnValue(cmn);
            }
        }
    }
    catch(...)
    {
        throw "CMasternodeModel  init error";
    }
}
CMasternodeModel::~CMasternodeModel()
{
    if (ptrdb != NULL)
    {
        delete ptrdb;
    }
}

CMasternodeModel* CMasternodeModel::initance()
{
    static CMasternodeModel p;
    return &p;
}

int CMasternodeModel::insertMasternode(const CMasternode& cmn)
{
    if (ptrdb != NULL)
    {
        if (!ptrdb->addMn(cmn.m_mn_hash,
                         CMasternode::Serializable(cmn)))
        {
            qDebug()<< "insert error.";
        }
        setNewMnValue(cmn);
    }
    return 0;
}

int CMasternodeModel::deleteMasternode(const QString& qsHash)
{
    if(qmapMns.find(qsHash)!=qmapMns.end())
    {
        delMnValue(qmapMns[qsHash]);
    }
    if (ptrdb->delMn(qsHash))
    {
        return 1;
    }
    return 0;
}

int CMasternodeModel::updateMasternode(const CMasternode& cmn)
{
    if (ptrdb->updateMn(cmn.m_mn_hash,
                        CMasternode::Serializable(cmn)))
    {
        setNewMnValue(cmn);
        return 0;
    }
    else
    {
        return -1;
    }
}

CMasternode CMasternodeModel::selectMasternode(const QString& qsHash)
{
    CMasternode cmn;
    if (qmapMns.find(qsHash)!=qmapMns.end())
    {
        cmn = qmapMns[qsHash];
    }
    else
    {
        QByteArray qsData = ptrdb->queryData(qsHash);
        if (qsData != "")
        {
            cmn = CMasternode::DeSerializable(qsData);
        }
    }
    return cmn;
}

const QMapMasterNode& CMasternodeModel::queryAllMasternodes()
{
    return qmapMns;
}

void CMasternodeModel::setNewMnValue(const CMasternode &cmn)
{
    qmapMns[cmn.m_mn_hash] = cmn;
    if (cmn.m_ip != "")
    {
        qmapIpToHsh[cmn.m_ip] = cmn.m_mn_hash;
    }
    if (cmn.m_alias != "")
    {
        qmapAliasToIp[cmn.m_alias] = cmn.m_ip;
        qlistAlias<<cmn.m_alias;
    }
}

void CMasternodeModel::delMnValue(const CMasternode &cmn)
{
    QMapKeyToValuse::iterator pa2i
            = qmapAliasToIp.find(cmn.m_alias);
    if (pa2i!= qmapAliasToIp.end())
    {
        qmapAliasToIp.erase(pa2i);
    }
    QMapKeyToValuse::iterator pi2h
            = qmapIpToHsh.find(cmn.m_ip);
    if (pi2h != qmapIpToHsh.end())
    {
        qmapIpToHsh.erase(pi2h);
    }
    QMapMasterNode::iterator ph2m
            = qmapMns.find(cmn.m_mn_hash);
    if (ph2m != qmapMns.end())
    {
        qmapMns.erase(ph2m);
    }
    // 删除别名
    qlistAlias.removeAll(cmn.m_alias);
}

QString CMasternodeModel::queryIp(const QString& qsAlias)
{
    return qmapAliasToIp[qsAlias];
}

QString CMasternodeModel::queryHash(const QString& qsIp)
{
    return qmapIpToHsh[qsIp];
}

bool CMasternodeModel::findAlias(const QString& qsAlias)
{
    if (qlistAlias.contains(qsAlias))
    {
        return true;
    }
    return false;
}

QStringList CMasternodeModel::findAllAlias()
{
    return qlistAlias;
}
