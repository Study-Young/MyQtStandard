#include "database.h"
#include <QDebug>

#define CUR_FILE_NAME ("database.cpp")

DataBase::DataBase(QObject *parent) :
    QObject(parent)
{
    mainInit();
    connectSignalSlot();

    m_thread->start();
}

DataBase::~DataBase()
{
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();

    m_dataBase->close();
    delete m_dataBase;
}

void DataBase::mainInit()
{
    memberValueInit();
}

void DataBase::connectSignalSlot()
{
}

void DataBase::memberValueInit()
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
}

void DataBase::databaseInitSlot(QString type)
{
    bool databaseSta = false;

    m_dataBase = new QSqlDatabase();
    m_userBase = new QSqlDatabase();

    if(type == "MYSQL")
    {
        *m_dataBase = QSqlDatabase::addDatabase("QMYSQL", "m_dataBase");
        *m_userBase = QSqlDatabase::addDatabase("QMYSQL", "m_userBase");

        m_dataBase->setHostName("127.0.0.1");
        m_dataBase->setPort(3307);
        m_dataBase->setUserName("root");
        m_dataBase->setPassword("123456");
        m_dataBase->setDatabaseName("DataManagement");

        m_userBase->setHostName("127.0.0.1");
        m_userBase->setPort(3307);
        m_userBase->setUserName("root");
        m_userBase->setPassword("123456");
        m_userBase->setDatabaseName("UserManagement");
    }
    else if(type == "SQLITE")
    {
        *m_dataBase = QSqlDatabase::addDatabase("QSQLITE", "m_dataBase");
        *m_userBase = QSqlDatabase::addDatabase("QSQLITE", "m_userBase");

        m_dataBase->setDatabaseName("DataManagement.db");
        m_userBase->setDatabaseName("UserManagement.db");
    }

    if (!m_dataBase->open())
    {
        databaseSta = false;
        emit updateMsgSignal(__FILE__, __FUNCTION__, MESSAGE_ERROR, QString("数据库打开失败，%1").arg(m_dataBase->lastError().text()));
    }
    else
    {
        databaseSta = true;
        emit updateMsgSignal(__FILE__, __FUNCTION__, MESSAGE_EVENT, "数据库打开成功");
    }

    emit databaseInitCompleteSignal(databaseSta);
    if(!databaseSta)
        return;

    emit tableInitSignal();
}

bool DataBase::isTableExist(QString tableName)
{
    if(m_dataBase->tables().contains(tableName))
    {
        return true;
    }

    return false;
}

bool DataBase::execSql(QString &sqlStr, const QString &msg)
{
    QSqlQuery query(*m_dataBase);

    query.prepare(sqlStr);

    bool flag = true;
    if(query.exec())
    {
        if(!msg.isEmpty())
            emit updateMsgSignal(__FILE__, __FUNCTION__, MESSAGE_EVENT, QString("%1成功").arg(msg));
    }
    else
    {
        if(!msg.isEmpty())
            emit updateMsgSignal(__FILE__, __FUNCTION__, MESSAGE_ERROR, QString("%1失败，%2").arg(msg).arg(query.lastError().text()));
        flag = false;
    }

    query.exec("COMMIT");
    return flag;

}

bool DataBase::execSqlList(QList<QString> &sqlList, const QString &msg)
{
    QSqlQuery query(*m_dataBase);
    query.exec("BEGIN");

    bool flag = true;
    for(auto sql:sqlList)
    {
        query.prepare(sql);
        if(!query.exec())
        {
            if(!msg.isEmpty())
                emit updateMsgSignal(__FILE__, __FUNCTION__, MESSAGE_ERROR, QString("%1 error %2").arg(__func__).arg(query.lastError().text()));
            flag = false;
        }
    }

    query.exec("COMMIT");
    return flag;
}
