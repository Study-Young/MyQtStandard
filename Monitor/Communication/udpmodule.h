#ifndef UDPMODULE_H
#define UDPMODULE_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

class UdpModule: public QObject
{
    Q_OBJECT
public:
    explicit UdpModule(QObject *parent = nullptr);
    ~UdpModule();

signals:
    void recordDataStreamSignal(QString strTime, QString strFile, QString strFunc, QString strData,
                                QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);

public slots:
    void sendCmdSlot(const QString ip, const quint16 port, const QString data);

private slots:
    void recvData();

private:
    void memberValueInit();

private:
    QThread *m_thread = nullptr;
    QUdpSocket *m_udpSocket = nullptr;
    QByteArray m_recvDataArr;
};

#endif // UDPMODULE_H
