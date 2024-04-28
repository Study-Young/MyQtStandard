#include "myhttpmodule.h"

MyHttpModule::MyHttpModule(QObject *parent):
    HttpModule(parent),
    m_isServing(false)
{

}

bool MyHttpModule::isServing() const
{
    return m_isServing;
}

void MyHttpModule::initHttp()
{
}

void MyHttpModule::sendHeartBreak()
{
    QString urlStr = QString("ims/heartbeat/update");
    QJsonObject heartbeatObj;
    heartbeatObj.insert("clientId", "clock");
    heartbeatObj.insert("line", 1);
    postRequest(urlStr, heartbeatObj);
}

QByteArray MyHttpModule::recvServerData(QString &path, QByteArray &body)
{
    bool paramFlag = false;
    if(path.contains("ims/alarmCode/query"))
    {
        int pageNo = -1;
        int pageSize = -1;
        QJsonParseError err;
        QJsonDocument jsonRecv = QJsonDocument::fromJson(body, &err);
        if(!jsonRecv.isNull())
        {
            QJsonObject object = jsonRecv.object();
            if(object.contains("pageNo"))
            {
                QJsonValue value = object.value("pageNo");
                if(value.isDouble())
                    pageNo = value.toVariant().toInt();
            }
            if(object.contains("pageSize"))
            {
                QJsonValue value = object.value("pageSize");
                if(value.isDouble())
                    pageSize = value.toVariant().toInt();
            }
            paramFlag = true;
        }
        return sendAlarmCode(paramFlag, pageNo, pageSize);
    }
}

void MyHttpModule::handleData(QString urlStr, QByteArray data)
{
    if(urlStr.contains("ims/heartbeat/update"))
    {
        ;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonRecv = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug()<<__FUNCTION__<<jsonError.errorString();
        return;
    }
    if(!jsonRecv.isNull())
    {
        int retCode;
        QString retMessage;
        QJsonObject object = jsonRecv.object();
        if(object.contains("retCode"))
        {
            QJsonValue value = object.value("retCode");
            if(value.isDouble())
                retCode = value.toVariant().toInt();
        }
        if(object.contains("retMessage"))
        {
            QJsonValue value = object.value("retMessage");
            if(value.isString())
                retMessage = value.toVariant().toString();
        }
    }
}

void MyHttpModule::startServer(QString localIp, quint16 localPort, QString aimIp, quint16 aimPort, quint32 heartIntv, quint32 confirmIntv)
{
    m_isServing = true;
    setBindServer(localIp, localPort, aimIp, aimPort);
    HttpModule::startServer();
}

void MyHttpModule::stopServer()
{
    m_isServing = false;
    HttpModule::stopServer();
}

QByteArray MyHttpModule::sendAlarmCode(bool paramFlag, int pageNo, int pageSize)
{
    QJsonObject resultObject;
    int alarmSize = 10;
    resultObject.insert("total", alarmSize);

    QJsonArray dataList;

    resultObject.insert("pageNo", 1);
    resultObject.insert("pageSize", alarmSize);

    for(int i=0; i<alarmSize; i++)
    {
        QJsonObject alarmMsgObj;
        alarmMsgObj.insert("code", QString::number(i+1));
        alarmMsgObj.insert("suggestion", "设备排查");
        dataList.append(QJsonValue(alarmMsgObj));
    }

    resultObject.insert("dataList", QJsonValue(dataList));

    QJsonObject infoObject;
    infoObject.insert("result", QJsonValue(resultObject));
    infoObject.insert("retMessage", "请求成功");
    infoObject.insert("retCode", 0);

    QJsonDocument document;
    document.setObject(infoObject);
    QByteArray askArray = document.toJson(QJsonDocument::Compact);

    return askArray;
}

void MyHttpModule::sendNewAlarm()
{
    QJsonObject resultObject;
    int alarmSize = 10;
    resultObject.insert("rows", alarmSize);

    QJsonArray dataList;
    for(int i=0; i<alarmSize; i++)
    {
        QJsonObject alarmMsgObj;

        quint32 curAlarmId = 1;
        alarmMsgObj.insert("alarmId", 50);
        alarmMsgObj.insert("code", QString::number(curAlarmId));

        dataList.append(QJsonValue(alarmMsgObj));
    }

    resultObject.insert("dataList", QJsonValue(dataList));

    QString urlStr = QString("ims/alarm/update");
    postRequest(urlStr, resultObject);
}

