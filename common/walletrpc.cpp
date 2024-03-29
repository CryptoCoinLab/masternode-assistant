#include "walletrpc.h"

WalletRPC::WalletRPC(QString hostName, QString rpcUser, QString rpcPassword, QObject *parent)
    : QObject(parent)
{
    hostUrl = "http://" + hostName + ":" + "5554";
    userInfo2AuthInfo(rpcUser ,rpcPassword);
}

void WalletRPC::userInfo2AuthInfo(QString userName, QString passWord)
{
    //Authorization = "Basic YW5ndXM6YW5ndXM=";
    QString preAuth = "Basic ";
    //QString user_and_password = "user:password";
    QString user_and_password = userName + ":" + passWord;
    QByteArray  tmp(user_and_password.toUtf8());
    QString base64NameAndPassword(tmp.toBase64());
    Authorization = (preAuth + base64NameAndPassword).toUtf8();
    //qDebug()<< Authorization;
}

QJsonValue WalletRPC::execCmd(QByteArray array)
{
    QNetworkRequest request(hostUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
    request.setRawHeader("Authorization",Authorization);

    reply = qnam.post(request,array);

    QEventLoop eventLoop;
    connect(&qnam, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish

    QByteArray responseData(reply->readAll());

    return parseResponse(responseData);
}

QJsonValue WalletRPC::parseResponse(QByteArray &responseData)
{
    //qDebug() << responseData;
    QJsonValue value;
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(responseData, &jsonError);

    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject respObj = json.object();
            if (respObj.contains("error") && respObj.contains("result"))
            {
                if(!respObj.value("error").isObject())
                {
                    value = respObj.value("result");
                }
                else
                {
                    qDebug() << "parseResponse get result error!";
                    value = respObj.value("error");
                }
            }
        }
        else
        {
            qDebug()<<"is not object: " << QString(json.toJson());
        }
    }
    else
    {
        qDebug() << "request rpc error!!";
    }

    //qDebug() << value.toString();
    return value;
}

QJsonObject WalletRPC::getinfo()
{
    qDebug() << "getinfo";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"getinfo\", \"params\": []}");
    QJsonValue retData = execCmd(array);
    QJsonObject subObj;
    if (retData.isObject())
    {
        subObj = retData.toObject();
    }

    return subObj;
}

QString WalletRPC::masternodeGenkey()
{
    qDebug() << "masternodeGenkey";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"masternode\", \"params\": [\"genkey\"]}");
    QJsonValue retData = execCmd(array);
    return retData.toString();
}

QString WalletRPC::getblockcount()
{
    qDebug() << "getblockcount";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"getblockcount\", \"params\": []}");
    QJsonValue retData = execCmd(array);
    return QString::number(retData.toDouble(), 10, 0);
}

QString WalletRPC::getaccountaddress(QString label)
{
    qDebug() << "getaccountaddress";
    QString cmdInfo = "{\"jsonrpc\":\"1.0\", \"method\":\"getaccountaddress\", \"params\": [\"" + label + "\"]}";
    QByteArray array(cmdInfo.toUtf8());
    QJsonValue retData = execCmd(array);
    return retData.toString();
}

/*
void WalletRPC::masternodeStop()
{
    qDebug() << "masternodeStop";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"stop\", \"params\": []}");
    execCmd(array);
}
*/

QJsonObject WalletRPC::masternodeStatus()
{
    qDebug() << "masternodeStatus";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"masternode\", \"params\": [\"status\"]}");
    QJsonValue retData = execCmd(array);
    QJsonObject subObj;

    if (retData.isObject())
    {
        subObj = retData.toObject();
    }

    return subObj;
}

QJsonObject WalletRPC::masternodeOutputs()
{
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"masternode\", \"params\": [\"outputs\"]}");
    QJsonValue retData = execCmd(array);
    QJsonObject subObj;

    if (retData.isObject())
    {
        subObj = retData.toObject();
    }

    return subObj;
}

QJsonValue WalletRPC::masternodeDebug()
{
    qDebug() << "masternodeStatus";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"masternode\", \"params\": [\"debug\"]}");
    return execCmd(array);
}

QList<QJsonObject> WalletRPC::listTransations()
{
    qDebug()<< "list Transations...";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"listtransactions\", \"params\": [\"*\", 10]}");
    QJsonValue retData = execCmd(array);
    QList<QJsonObject> qlObjec;
    if (retData.isArray())
    {
       QJsonArray qa = retData.toArray();
       for (int i=0; i<qa.size(); i++)
       {
           qlObjec.append(qa[i].toObject());
       }
    }
    return qlObjec;
}
