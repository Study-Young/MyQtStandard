#ifndef MYHTTPMODULE_H
#define MYHTTPMODULE_H

#include "httpmodule.h"

class MyHttpModule : public HttpModule
{
    Q_OBJECT
public:
    MyHttpModule(QObject *parent = 0);

    virtual bool isServing() const;

protected:
    virtual void initHttp();
    virtual QByteArray recvServerData(QString &path, QByteArray &body);
    virtual void handleData(QString urlStr, QByteArray data);

public slots:
    void startServer(QString localIp, quint16 localPort, QString aimIp, quint16 aimPort, quint32 heartIntv, quint32 confirmIntv);
    void stopServer();

private slots:
    void sendHeartBreak();

private:
    QByteArray sendAlarmCode(bool paramFlag, int pageNo, int pageSize);

    void sendNewAlarm();

private:
    bool m_isServing;
};

#endif // MYHTTPMODULE_H
