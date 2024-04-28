/*
 * Copyright 2011-2014 Nikhil Marathe <nsm.nikhil@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QTcpSocket>
#include <QVariant>
#include <QEventLoop>
#include <QDebug>

#include "threadhandle.h"
#include "qasynhttpsocket.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpserver.h"

//QHash<int, QString> STATUS_CODES;

QHttpServer::QHttpServer(QObject *parent,int numConnections) :
    QAsynTcpServer(parent,numConnections)
{    

}

QHttpServer::~QHttpServer()
{

}

void QHttpServer::incomingConnection(qintptr socketDescriptor)
{
    //继承重写此函数后，QQAsynTcpServer默认的判断最大连接数失效，自己实现
    if (m_ClientList->size() > maxPendingConnections())
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);        
        tcp.disconnectFromHost();
        return;
    }

    auto th = ThreadHandle::getClass().getThread();
    QAsynHttpSocket* tcpTemp = new QAsynHttpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();
    emit updateConnectStaSignal(true, ip, port);

    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，线程管理计数减1,此槽必须实现
    connect(tcpTemp,SIGNAL(sockDisConnect(const int ,const QString &,const quint16, QThread *)),
            this,SLOT(sockDisConnectSlot(const int ,const QString &,const quint16, QThread *)));

    //必须在QAsynHttpSocket的线程中执行
    connect(tcpTemp, SIGNAL(newRequest(QHttpRequest *, QHttpResponse *)), this,
            SLOT(handleRequest(QHttpRequest *, QHttpResponse *)), Qt::DirectConnection);

    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    m_ClientList->insert(socketDescriptor,tcpTemp);//插入到连接信息中
}

//释放线程资源
void QHttpServer::sockDisConnectSlot(int handle, const QString & ip, quint16 port, QThread * th)
{
    emit updateConnectStaSignal(false, ip, port);
    m_ClientList->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了,释放数量
}

//处理新的http 请求，这里处理业务
void QHttpServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    QString path = req->path();
    QByteArray body = req->body();
    QByteArray data;
    emit handleRequestSignal(req->remoteAddress(), req->remotePort(), &path, &body, &data);

    QEventLoop loop;
    connect(this, &QHttpServer::handleRequestCompleteSignal, &loop , &QEventLoop::quit);
    loop.exec();

    resp->setHeader("Content-Type", "text/html");
    resp->setHeader("Content-Length", QString::number(data.length()));
    resp->writeHead(200);
    resp->end(data);
    resp->flush();

    req->deleteLater();
    resp->deleteLater();
}

