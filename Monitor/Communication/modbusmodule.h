#ifndef MODBUSMODULE_H
#define MODBUSMODULE_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QTimer>
#include <QMap>
#include <QMutex>

#include "datahandlemodule.h"
#include "datastruct.h"
#include "modbus/modbus.h"


class ModbusModule : public QObject
{
    Q_OBJECT
public:
    explicit ModbusModule(QObject *parent = nullptr);
    ~ModbusModule();
    DataHandleModule* m_dataHandle = nullptr;

public slots:
    void sendCmdSlot(const QString ip, const int cmdType, const int data);
    void clearDataArrSlot();
    void insertNewModbusSlot(const QString ip);
    void deleteDeviceModbusSlot(const QString ip);
    void startReadDataSlot();

signals:
    void recordDataStreamSignal(QString strTime, QString strFile, QString strFunc, QString strRecord, QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);
    void recordErrorSignal(QString strTime, QString strFile, QString strFunc, QString strError, QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);

private:
    void memberValueInit();
    modbus_t* createModbus(QString ip);
    void swap8ByteData(uint16_t * data, int index);
    void swap4ByteData(uint16_t * data, int index);
    void swap2ByteData(uint16_t * data, int index);

private slots:
    void readData();

private:
    QThread* m_thread = nullptr;
    modbus_t *m_modbus = nullptr;
    QMap<QString, modbus_t*> m_modbusMap;
    uint16_t *m_readData;
    uint16_t *m_sendData;

    bool m_startFlag;
    QMutex *m_pMutex;

    QTimer *m_timer;

    QByteArray m_recvDataArr;
};

#endif // MODBUSMODULE_H
