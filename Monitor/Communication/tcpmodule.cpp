#include "tcpmodule.h"
#include <QDateTime>

TcpModule::TcpModule(QObject *parent)
    : QObject(parent)
{
    memberValueInit();
    m_thread->start();
}

TcpModule::~TcpModule()
{
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
}

void TcpModule::memberValueInit()
{
    m_tcpSocket = new QTcpSocket(this);

    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_tcpSocket->moveToThread(m_thread);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpModule::recvData);
    connect(m_thread, &QThread::finished, m_tcpSocket, &QObject::deleteLater);
}

void TcpModule::sendCmdSlot(const char* data, const int len)
{
    m_tcpSocket->write(data, len);
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    emit recordDataStreamSignal(strTime, __FILE__, __FUNCTION__, QString(data));
}

void TcpModule::recvData()
{
    m_recvDataArr = m_tcpSocket->readAll();

    m_recvDataArr.clear();
}

unsigned short TcpModule::crc16(char* ptr, int count)
{
    int crc, i;

    crc = 0;

    while (--count >= 0)
    {
        crc = (crc ^ (((int)*ptr++) << 8));
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = ((crc << 1) ^ 0x1021);
            else
                crc = crc << 1;
        }
    }

    return (unsigned short)(crc & 0xFFFF);
}
