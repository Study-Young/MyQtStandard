#include "mydatabase.h"
#include <QDebug>

MyDataBase::MyDataBase(QObject *parent)
    :DataBase(parent)
{
    connectSignalSlot();
}

MyDataBase::~MyDataBase()
{

}

void MyDataBase::connectSignalSlot()
{
    connect(this, &DataBase::tableInitSignal, this, &MyDataBase::tableInitSlot);
}

void MyDataBase::tableInitSlot()
{
    userTableInit();
}

void MyDataBase::userTableInit()
{
    QString createTableSql;

    if(!isTableExist("UserInfo"))
    {
        createTableSql = "CREATE TABLE UserInfo ("
                         "UserId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                         "Name TEXT NOT NULL,"
                         "Password TEXT NOT NULL,"
                         "CreateTime DATETIME DEFAULT CURRENT_TIMESTAMP)";

        execSql(createTableSql, QString("用户信息表创建"));
    }
}

void MyDataBase::checkUserNamePasswordSlot(QString name, QString password, bool flag)
{
    int userId = 0;
    QString queryPassword = "";
    QString userName = "";
    QSqlQuery query(*m_dataBase);
    QString sqlStr = QString("SELECT UserId, Password FROM UserInfo WHERE Name=\"%1\"").arg(name);
    query.prepare(sqlStr);
    m_checkUserNamePasswordSta = false;

    if(query.exec())
    {
        if(query.next())
        {
            userId = query.value(0).toInt();
            queryPassword = query.value(1).toString();

            if(queryPassword == password)
            {
                if(flag)
                {
                    userName = name;
                }
            }
            else
            {
                emit checkUserNamePasswordCompleteSignal(2);
            }
        }
        else
        {
            emit checkUserNamePasswordCompleteSignal(0);
        }
    }
}

void MyDataBase::modifyPasswordSlot(QString name, QString oldPassword, QString newPassword)
{
    checkUserNamePasswordSlot(name, oldPassword, false);
    if(!m_checkUserNamePasswordSta)
        return;

    QString sqlStr = QString("UPDATE UserInfo set Password=\"%1\" WHERE Name=\"%2\"").arg(newPassword).arg(name);
    if(execSql(sqlStr, ""))
        emit modifyPasswordCompleteSignal(true);
    else
        emit modifyPasswordCompleteSignal(false);
}

void MyDataBase::addNewUserSlot(QString name, QString password)
{
    QSqlQuery query(*m_dataBase);

    QString sqlStr;
    sqlStr = QString("SELECT COUNT(*) FROM UserInfo WHERE Name=\"%1\"").arg(name);
    query.prepare(sqlStr);
    if(query.exec())
    {
        if(query.next())
        {
            if(query.value(0).toInt() != 0)
            {
                emit addNewUserCompleteSignal(0);
                return;
            }
        }
    }
    else
    {
        emit addNewUserCompleteSignal(-1);
    }

    sqlStr = QString("INSERT INTO UserInfo (Name, PassWord) VALUES (\"%1\", \"%2\")").arg(name).arg(password);
    if(execSql(sqlStr, ""))
        emit addNewUserCompleteSignal(1);
    else
        emit addNewUserCompleteSignal(-1);
}

void MyDataBase::initRoleInfoTableSlot()
{
    QSqlQuery query(*m_dataBase);

    QList<QString> nameList;
    QList<int> occupationList;
    QList<int> sexList;
    QList<int> levelList;
    QList<int> rebirthLevelList;
    QList<int> curExpList;
    QList<QString> lastEnterList;

    QString sqlStr = QString("SELECT * FROM RoleInfo");
    query.prepare(sqlStr);
    if(query.exec())
    {
        while(query.next())
        {
            nameList.append(query.value(2).toString());
            occupationList.append(query.value(3).toInt());
            sexList.append(query.value(4).toInt());
            levelList.append(query.value(6).toInt());
            rebirthLevelList.append(query.value(7).toInt());
            curExpList.append(query.value(8).toInt());
            lastEnterList.append(query.value(9).toString());
        }
    }
    emit updateRoleInfoTableSignal(nameList, occupationList, sexList, levelList, rebirthLevelList, curExpList, lastEnterList);
}
