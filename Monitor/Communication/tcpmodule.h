#ifndef TCPMODULE_H
#define TCPMODULE_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include "datastruct.h"

class TcpModule: public QObject
{
    Q_OBJECT
public:
    explicit TcpModule(QObject *parent = nullptr);
    ~TcpModule();

signals:
    void recordDataStreamSignal(QString strTime, QString strFile, QString strFunc, QString strData,
                                QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);

public slots:
    void sendCmdSlot(const char *data, const int len);

private slots:
    void recvData();

private:
    void memberValueInit();
    unsigned short crc16(char* ptr, int count);

private:
    QThread *m_thread = nullptr;
    QTcpSocket *m_tcpSocket = nullptr;
    QByteArray m_recvDataArr;
};

#endif // TCPMODULE_H
