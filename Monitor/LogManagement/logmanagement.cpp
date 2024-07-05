#include "logmanagement.h"
#include "global.h"

#include <QDebug>

LogManagement *LogManagement::m_pInstance = nullptr;
LogManagement *LogManagement::getInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new LogManagement();
    }
    return m_pInstance;
}

void LogManagement::deleteInstance()
{
    delete m_pInstance;
}

LogManagement::LogManagement(QObject *parent) : QObject(parent)
{
    m_pThread = new QThread();
    this->moveToThread(m_pThread);
    m_pThread->start();
}

LogManagement::~LogManagement()
{
    m_pThread->quit();
    m_pThread->wait(1000);
    m_pThread->deleteLater();
}

void LogManagement::recordErrorSlot(QString strTime, QString strFile, QString strFunc, QString strError, QString strFolder1, QString strFolder2, QString strFolder3)
{
    if(!g_saveWorkLogFlag)
        return;
    QString strErrorFile = QString("./LOG/Error/");
    QString strContent = QString("%1,[%2],[%3],%4").arg(strTime).arg(strFile).arg(strFunc).arg(strError);
    mkFileAndWriteData(strErrorFile, strContent, strFolder1, strFolder2, strFolder3);
}

void LogManagement::recordEventSlot(QString strTime, QString strFile, QString strFunc, QString strEvent, QString strFolder1, QString strFolder2, QString strFolder3)
{
    QString strEventFile = QString("./LOG/Event/");
    QString strContent = QString("%1,[%2],[%3],%4").arg(strTime).arg(strFile).arg(strFunc).arg(strEvent);
    mkFileAndWriteData(strEventFile, strContent, strFolder1, strFolder2, strFolder3);
}

void LogManagement::recordDataStreamSlot(QString strTime, QString strFile, QString strFunc, QString strData, QString strFolder1, QString strFolder2, QString strFolder3)
{
    QString strEventFile = QString("./LOG/Data/");
    QString strContent = QString("%1,[%2],[%3],%4").arg(strTime).arg(strFile).arg(strFunc).arg(strData);
    mkFileAndWriteData(strEventFile, strContent, strFolder1, strFolder2, strFolder3);
}

void LogManagement::recordAlarmSlot(QString strContent, QString strFolder1, QString strFolder2, QString strFolder3)
{
    QString strAlarmFile = QString("./LOG/Alarm/");
    mkFileAndWriteData(strAlarmFile, strContent, strFolder1, strFolder2, strFolder3);
}

void LogManagement::recordOperatorSlot(QString strTime, QString strFile, QString strFunc, QString strOperator, QString strFolder1, QString strFolder2, QString strFolder3)
{
    if(!g_saveWorkLogFlag)
        return;
    QString strOperationFile = QString("./LOG/Operator/");
    QString strContent = QString("%1,[%2],[%3],%4").arg(strTime).arg(strFile).arg(strFunc).arg(strOperator);
    mkFileAndWriteData(strOperationFile, strContent, strFolder1, strFolder2, strFolder3);
}

void LogManagement::writeDataToFileSlot(QString strFilePath, QString strContent, bool bAppend, QString strArr)
{
    mkDirMultiLevel(strFilePath);
    bool bRet = writeDataToFile(strFilePath, strContent, bAppend);
    emit writeDataToFileRespSignal(strFilePath.split("/").last(), bRet, strArr);
}

void LogManagement::readDataFromFileSlot(QString strFilePath, QString strArr)
{
    QString strFileName = strFilePath.split("/").last();

    QFile dataFile(strFilePath);
    if(!dataFile.open(QIODevice::Text|QIODevice::ReadOnly))
    {
        emit readDataFromFileRespSignal(false, strFileName, NULL, strArr);
        return;
    }

    QByteArray baData = dataFile.readAll();
    dataFile.close();

    emit readDataFromFileRespSignal(true, strFileName, QString::fromLatin1(baData), strArr);
}

void LogManagement::mkFileAndWriteData(QString strFileType, QString strContent, QString strFolder1, QString strFolder2, QString strFolder3)
{
    QString strFilePath = strFileType;
    if(strFolder1 != NULL)
    {
        strFilePath += strFolder1;
        strFilePath += "/";

        if(strFolder2 != NULL)
        {
            strFilePath += strFolder2;
            strFilePath += "/";

            if(strFolder3 != NULL)
            {
                strFilePath += strFolder3;
                strFilePath += "/";
            }
        }
    }

    strFilePath += QDate::currentDate().toString("yyyy-MM-dd.txt");

    mkDirMultiLevel(strFilePath);
    writeDataToFile(strFilePath, strContent, true);
}

void LogManagement::mkDirMultiLevel(QString strFilePath)
{
    QDir dir;
    QString strDir; strDir.clear();
    QStringList listFolder = strFilePath.split("/");
    for(int i = 0; i < listFolder.size()-1; i++)
    {
        strDir += listFolder.at(i);
        if(dir.exists(strDir))
        {
            strDir += "/";
            continue;
        }

        dir.mkdir(strDir);
        strDir += "/";
    }
}

bool LogManagement::writeDataToFile(QString strFilePath, QString strContent, bool bAppend)
{
    QFile dataFile(strFilePath);
    if(bAppend)
    {
        if(!dataFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
        {
            qDebug() << __FUNCTION__ << "connot open file: " << strFilePath;
            return false;
        }
    }
    else
    {
        if(!dataFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug() << __FUNCTION__ << "connot open file: " << strFilePath;
            return false;
        }
    }

    dataFile.write(strContent.toLocal8Bit());
    dataFile.write("\n");
    dataFile.close();

    return true;
}
