#include "httpmodule.h"

HttpModule::HttpModule(QObject *parent)
{
    m_httpServer = new QHttpServer(this);
    m_httpClient = new QTcpSocket(this);

    connect(m_httpServer, &QHttpServer::updateConnectStaSignal, this, &HttpModule::updateConnectStaSlot);
    connect(m_httpServer, &QHttpServer::handleRequestSignal, this, &HttpModule::handleRequestSlot);
    connect(this, &HttpModule::handleRequestCompleteSignal, m_httpServer, &QHttpServer::handleRequestCompleteSignal);
}

HttpModule::~HttpModule()
{
    m_httpServer->deleteLater();
    m_httpClient->deleteLater();
}

void HttpModule::setBindServer(QString localIp, quint16 localPort, QString aimIp, quint16 aimPort)
{
    m_localIp = localIp;
    m_localPort = localPort;
    m_aimIp = aimIp;
    m_aimPort = aimPort;
}

bool HttpModule::serverIsListening() const
{
    return m_httpServer->isListening();
}

void HttpModule::startServer()
{
    initHttp();
    if(!serverIsListening())
        m_httpServer->listen(QHostAddress(m_localIp), m_localPort);
}

void HttpModule::stopServer()
{
    m_httpServer->close();
}

void HttpModule::restartServer()
{
    stopServer();
    startServer();
}

void HttpModule::updateConnectStaSlot(bool sta, QString ip, quint16 port)
{
    qDebug()<<ip<<port<<sta;
}

void HttpModule::handleRequestSlot(QString ip, quint16 port, QString *path, QByteArray *body, QByteArray *data)
{
    *data = recvServerData(*path, *body);

    emit handleRequestCompleteSignal();
}

void HttpModule::postRequest(QString urlStr, QJsonObject dataObj)
{
    QNetworkRequest request;
    QNetworkAccessManager accessManager;
    connect(&accessManager, &QNetworkAccessManager::finished, this, &HttpModule::receiveReplyData);
    QString curUrlStr = QString("http://%1:%2/%3").arg(m_aimIp).arg(m_aimPort).arg(urlStr);
    request.setUrl(QUrl(curUrlStr));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charaset=utf-8"));

    QJsonDocument document;
    document.setObject(dataObj);
    QByteArray dataArr = document.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = accessManager.post(request, dataArr);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void HttpModule::getRequest(QString urlStr)
{
    QNetworkRequest request;
    QNetworkAccessManager accessManager;
    connect(&accessManager, &QNetworkAccessManager::finished, this, &HttpModule::receiveReplyData);
    QString curUrlStr = QString("http://%1:%2/%3").arg(m_aimIp).arg(m_aimPort).arg(urlStr);
    request.setUrl(QUrl(curUrlStr));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charaset=utf-8"));

    QNetworkReply *reply = accessManager.get(request);
}

void HttpModule::receiveReplyData(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<QString("http请求出错：%1").arg(reply->errorString());
    }
    else
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(statusCode == 200)
        {
            QString urlStr = reply->url().toString();
            QByteArray replyContent = reply->readAll();
            handleData(urlStr, replyContent);
        }
        else
        {
            qDebug()<<__FUNCTION__<<"status code error:"<<statusCode;
        }
    }
    reply->deleteLater();
}
