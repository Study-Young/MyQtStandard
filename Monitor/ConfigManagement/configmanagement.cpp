#include "configmanagement.h"
#include <QSettings>
#include <QMessageBox>
#include <QFile>

ConfigManagement *ConfigManagement::m_pInstance = nullptr;
ConfigManagement *ConfigManagement::getInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new ConfigManagement();
    }
    return m_pInstance;
}

void ConfigManagement::deleteInstance()
{
    delete m_pInstance;
}

ConfigManagement::ConfigManagement(QObject *parent) : QObject(parent)
{
    mainInit();
    connectSignalSlot();
}

ConfigManagement::~ConfigManagement()
{

}

void ConfigManagement::mainInit()
{
    memberValueInit();
}

void ConfigManagement::connectSignalSlot()
{

}

void ConfigManagement::memberValueInit()
{
    m_fileName = "paramConfig.ini";
    QFile file(m_fileName);
    if(!file.exists())
    {
        writeNewCaliFile(m_fileName);
    }
}

void ConfigManagement::writeNewCaliFile(QString fileName)
{
    QSettings config(fileName, QSettings::IniFormat);
    config.setValue(QString("DataBase/IPAddress"), "192.168.90.15");
    config.setValue(QString("DataBase/UserName"), "root");
    config.setValue(QString("DataBase/Password"), "zjsaisi");
    config.setValue(QString("LogSave/WorkLogFlag"), 1);
}

void ConfigManagement::setDatabaseIp(QString ip)
{
    QSettings config(m_fileName, QSettings::IniFormat);
    config.setValue(QString("DataBase/IPAddress"), ip);
}

QString ConfigManagement::getDatabaseIp()
{
    QSettings config(m_fileName, QSettings::IniFormat);
    return config.value(QString("DataBase/IPAddress"), "192.168.90.15").toString();
}

void ConfigManagement::setDatabaseUserName(QString name)
{
    QSettings config(m_fileName, QSettings::IniFormat);
    config.setValue(QString("DataBase/UserName"), name);
}

QString ConfigManagement::getDatabaseUserName()
{
    QSettings config(m_fileName, QSettings::IniFormat);
    return config.value(QString("DataBase/UserName"), "root").toString();
}

void ConfigManagement::setDatabasePassword(QString password)
{
    QSettings config(m_fileName, QSettings::IniFormat);
    config.setValue(QString("DataBase/Password"), password);
}

QString ConfigManagement::getDatabasePassword()
{
    QSettings config(m_fileName, QSettings::IniFormat);
    return config.value(QString("DataBase/Password"), "zjsaisi").toString();
}

void ConfigManagement::setSaveWorkLogFlag(int flag)
{
    QSettings config(m_fileName, QSettings::IniFormat);
    config.setValue(QString("LogSave/WorkLogFlag"), flag);
}

bool ConfigManagement::getSaveWorkLogFlag()
{
    QSettings config(m_fileName, QSettings::IniFormat);
    return config.value(QString("LogSave/WorkLogFlag"), 1).toBool();
}
