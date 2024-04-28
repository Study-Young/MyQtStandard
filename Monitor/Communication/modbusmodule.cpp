#include "modbusmodule.h"
#include "global.h"
#include <QtEndian>
#include <QDateTime>
#include <QEventLoop>

#define THIS_FILE_NAME "modbusmodule.cpp"

ModbusModule::ModbusModule(QObject *parent)
    : QObject(parent)
{
    memberValueInit();
    m_thread->start();
}

ModbusModule::~ModbusModule()
{
    QList<modbus_t *> modbusObjList = m_modbusMap.values();
    for(int i=0; i<modbusObjList.size(); i++)
    {
        modbus_t *mb = modbusObjList.at(i);
        modbus_close(mb);
        modbus_free(mb);
    }
    delete []m_readData;
    delete []m_sendData;
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
}

void ModbusModule::memberValueInit()
{
    m_readData = new uint16_t[1250];
    memset(m_readData, 0, 1250*sizeof(uint16_t));

    m_sendData = new uint16_t[125];
    memset(m_sendData, 0, 125*sizeof(uint16_t));

    m_dataHandle = new DataHandleModule(this);   

    m_pMutex = new QMutex();

    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_dataHandle->moveToThread(m_thread);

    m_timer = new QTimer();
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &ModbusModule::readData);
//    m_timer->start(1000);
    m_timer->moveToThread(m_thread);

    connect(m_thread, &QThread::finished, m_dataHandle, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_timer, &QObject::deleteLater);
}

modbus_t* ModbusModule::createModbus(QString ip)
{
    int Address =  1;      //站号
    modbus_t *mb = modbus_new_tcp(ip.toLatin1().constData(), 502);
    modbus_set_slave(mb, Address);  //从机地址
    modbus_connect(mb); //

    //2.设置通讯超时时间
    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒，注意：经测试，如果没有成功建立tcp连接，则该设置无效。
    modbus_set_response_timeout(mb,t.tv_sec,t.tv_usec);
    return mb;
}

void ModbusModule::insertNewModbusSlot(const QString ip)
{
    m_pMutex->lock();
    m_modbusMap.insert(ip, createModbus(ip));
    m_pMutex->unlock();
}

void ModbusModule::deleteDeviceModbusSlot(const QString ip)
{
    m_pMutex->lock();
    m_modbusMap.remove(ip);
    m_pMutex->unlock();
}

void ModbusModule::sendCmdSlot(const QString ip, const int cmdType, const int data)
{
    modbus_t *mb = NULL;

    m_pMutex->lock();
    mb = m_modbusMap.value(ip);
    m_pMutex->unlock();

    if(mb == NULL)
        return;
    int wreg = -1;
    if(cmdType == 1)
    {
        m_sendData[0] = data >> 16;
        m_sendData[1] = data & 0xffff;
        wreg = modbus_write_registers(mb,3,2,m_sendData);
    }
    else if(cmdType == 2)
    {
        m_sendData[0] = data >> 16;
        m_sendData[1] = data & 0xffff;
        wreg = modbus_write_registers(mb,5,2,m_sendData);
    }

    if(wreg  == -1){
        qDebug()<<"写入失败!";
    }
    else {  //写入成功返回0
        qDebug()<<"写入成功!";
    }
}

void ModbusModule::clearDataArrSlot()
{
    m_recvDataArr.clear();
}

void ModbusModule::startReadDataSlot()
{
    m_timer->start(1000);
}

void ModbusModule::readData()
{
    QList<modbus_t *> modbusObjList = m_modbusMap.values();
    for(int i=0; i<modbusObjList.size(); i++)
    {
        modbus_t *mb = modbusObjList.at(i);
        QString ip = m_modbusMap.key(mb);

        int modbusLen = 1250;
        memset(m_readData, 0, modbusLen*sizeof(uint16_t));
        int startAddress = 0;
        int readLength = 0;
        int regs = 0;

        startAddress = 1;
        readLength = 1244;
        int cycleCnt = modbusLen/125;
        for(int j=0; j<cycleCnt; j++)
        {
            if(j < (cycleCnt-1))
                regs += modbus_read_registers(mb, startAddress+125*j, 125, m_readData+125*j);
            else
                regs += modbus_read_registers(mb, startAddress+125*j, readLength-125*j, m_readData+125*j);

            QEventLoop loop;
            QTimer::singleShot(20, &loop, SLOT(quit()));
            loop.exec();
        }


        if(regs == readLength)
        {
            for(int m=0; m<5; m++)
            {
                swap2ByteData(m_readData, 73+m);
                swap2ByteData(m_readData, 78+m);
                swap2ByteData(m_readData, 83+m);
                swap2ByteData(m_readData, 88+m);
                swap2ByteData(m_readData, 93+m);
                swap2ByteData(m_readData, 98+m);
                if(m<4)
                {
                    swap4ByteData(m_readData, 0+m*2);
                }
                if(m<3)
                {
                    swap4ByteData(m_readData, 24+m*2);
                }
                if(m<2)
                {
                    swap8ByteData(m_readData, 8+m*4);
                    swap4ByteData(m_readData, 36+m*2);
                }
            } 
        }
        else
        {
            QString strRecord;
            if(regs > 0)
            {
                strRecord = QString("[ReadRegister from (%1) ERROR recvLength %2]").arg(ip).arg(regs);
            }
            else
            {
                m_modbusMap.insert(ip, createModbus(ip));
                strRecord = QString("[ReadRegister from (%1) ERROR %2]").arg(ip).arg(QString::fromUtf8(modbus_strerror(errno)));
            }
            QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            emit recordErrorSignal(strTime, THIS_FILE_NAME, __FUNCTION__, strRecord);
        }

        if(g_saveOriDataFlag == 1)
        {
            QString strRlt;
            QString strRecord;
            strRlt.clear();
            for(int j=0; j<regs; j++)
            {
                strRlt.append(QString("0x%1 ").arg(m_readData[j], 4, 16, QLatin1Char('0')));
            }
            if(regs == readLength)
            {
                strRecord = QString("[ReadRegister from (%1)] : %2").arg(ip).arg(strRlt);
            }
            QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            emit recordDataStreamSignal(strTime, THIS_FILE_NAME, __FUNCTION__, strRecord);
        }

        QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();
    }
}

void ModbusModule::swap8ByteData(uint16_t *data, int index)
{
    std::swap(data[index], data[index+3]);
    std::swap(data[index+1], data[index+2]);
}

void ModbusModule::swap4ByteData(uint16_t *data, int index)
{
    std::swap(data[index], data[index+1]);
}

void ModbusModule::swap2ByteData(uint16_t *data, int index)
{
    data[index] = qToBigEndian(data[index]);
}

