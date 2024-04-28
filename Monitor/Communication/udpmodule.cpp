#include "udpmodule.h"
#include "QDateTime"

#define THIS_FILE_NAME "udpmodule.cpp"

UdpModule::UdpModule(QObject *parent)
    : QObject(parent)
{
    memberValueInit();
    m_thread->start();
}

UdpModule::~UdpModule()
{
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
}

void UdpModule::memberValueInit()
{
    m_udpSocket = new QUdpSocket(this);

    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_udpSocket->moveToThread(m_thread);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpModule::recvData);
    connect(m_thread, &QThread::finished, m_udpSocket, &QObject::deleteLater);
}

void UdpModule::sendCmdSlot(const QString ip, const quint16 port, const QString data)
{
    m_udpSocket->writeDatagram(data.toUtf8(), QHostAddress(ip), port);
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    emit recordDataStreamSignal(strTime, THIS_FILE_NAME, __FUNCTION__, data);
}

void UdpModule::recvData()
{
    QHostAddress devIp;
    quint16 devPort;

    m_recvDataArr.resize(m_udpSocket->pendingDatagramSize());
    int len = m_udpSocket->readDatagram(m_recvDataArr.data(), m_recvDataArr.size(), &devIp, &devPort);

    quint32 ipTemp = devIp.toIPv4Address();
    devIp = QHostAddress(ipTemp);
    QString ipStr = devIp.toString();
    QString recvDataStr = QString(m_recvDataArr).simplified();

    m_recvDataArr.clear();
}
