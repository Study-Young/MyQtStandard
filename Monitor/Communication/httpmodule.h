#ifndef HTTPMODULE_H
#define HTTPMODULE_H

#include "HttpServer/qhttpserver.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QEventLoop>
#include <QTcpServer>
#include <QTcpSocket>

class HttpModule: public QObject
{
    Q_OBJECT
public:
    HttpModule(QObject *parent = 0);
    ~HttpModule();

signals:
    void handleRequestCompleteSignal();

public slots:
    void updateConnectStaSlot(bool sta, QString ip, quint16 port);
    void handleRequestSlot(QString ip, quint16 port, QString *path, QByteArray *body, QByteArray *data);

protected:
    void setBindServer(QString localIp, quint16 localPort, QString aimIp, quint16 aimPort);
    bool serverIsListening() const;
    void startServer();
    void stopServer();
    void restartServer();

    virtual void initHttp() = 0;
    virtual QByteArray recvServerData(QString &path, QByteArray &body) = 0;
    virtual void handleData(QString urlStr, QByteArray dataArr) = 0;

    void postRequest(QString urlStr, QJsonObject dataObj);
    void getRequest(QString urlStr);
    void receiveReplyData(QNetworkReply *reply);

private:
    QString m_localIp;
    quint16 m_localPort;
    QString m_aimIp;
    quint16 m_aimPort;

    QHttpServer *m_httpServer;
    QTcpSocket *m_httpClient;
};

#endif // HTTPMODULE_H
