#ifndef LOGMANAGEMENT_H
#define LOGMANAGEMENT_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QDate>


class LogManagement : public QObject
{
    Q_OBJECT
public:
    static LogManagement *getInstance();
    static void deleteInstance();

signals:
    void writeDataToFileRespSignal(QString strFile, bool bOk, QString strArr);
    void readDataFromFileRespSignal(bool bReadFlag, QString strFileName, QString strData, QString strArr);

public slots:
    void recordErrorSlot(QString strTime, QString strFile, QString strFunc, QString strError, QString strFolder1, QString strFolder2, QString strFolder3);
    void recordEventSlot(QString strTime, QString strFile, QString strFunc, QString strEvent, QString strFolder1, QString strFolder2, QString strFolder3);
    void recordDataStreamSlot(QString strTime, QString strFile, QString strFunc, QString strData, QString strFolder1, QString strFolder2, QString strFolder3);
    void recordAlarmSlot(QString strContent, QString strFolder1, QString strFolder2, QString strFolder3);
    void recordOperatorSlot(QString strTime, QString strFile, QString strFunc, QString strOperator, QString strFolder1, QString strFolder2, QString strFolder3);
    void writeDataToFileSlot(QString strFilePath, QString strContent, bool bAppend, QString strArr);
    void readDataFromFileSlot(QString strFilePath, QString strArr);

private:
    explicit LogManagement(QObject *parent = nullptr);
    ~LogManagement();

    void mkFileAndWriteData(QString strFileType, QString strContent, QString strFolder1, QString strFolder2, QString strFolder3);
    void mkDirMultiLevel(QString strFilePath);
    bool writeDataToFile(QString strFilePath, QString strContent, bool bAppend);

private:
    static LogManagement* m_pInstance;
    QThread* m_pThread;

};

#endif // LOGMANAGEMENT_H
