﻿//legahero  QQ:1395449850
#ifndef Q_HTTP_SERVER
#define Q_HTTP_SERVER

#define QHTTPSERVER_VERSION_MAJOR 0
#define QHTTPSERVER_VERSION_MINOR 1
#define QHTTPSERVER_VERSION_PATCH 0


#include <QObject>
#include <QtNetwork>

#include "qasyntcpserver.h"
/*
#include "qasynhttpsocket.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"
*/
#include "qhttpserverfwd.h"


class QHttpServer : public QAsynTcpServer
{
    Q_OBJECT
public:    
    QHttpServer(QObject *parent = 0,int numConnections=1000);

    virtual ~QHttpServer();

signals:
    void updateConnectStaSignal(bool sta, QString ip, quint16 port);
    void handleRequestSignal(QString ip, quint16 port, QString *path, QByteArray *body, QByteArray *data);
    void handleRequestCompleteSignal();

private Q_SLOTS:
    void handleRequest(QHttpRequest *request, QHttpResponse *response);   

protected slots:
    void sockDisConnectSlot(int handle,const QString & ip, quint16 port, QThread *th);//断开连接的用户信息

protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
};

#endif
