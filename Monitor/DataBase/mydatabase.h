#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include "database.h"

class MyDataBase : public DataBase
{
    Q_OBJECT
public:
    static MyDataBase *getInstance();
    static void deleteInstance();

signals:
    void checkUserNamePasswordCompleteSignal(int sta);
    void modifyPasswordCompleteSignal(bool flag);
    void addNewUserCompleteSignal(int sta);

    void updateRoleInfoTableSignal(QList<QString> nameList, QList<int> occupationList, QList<int> sexList,
                                   QList<int> levelList, QList<int> rebirthLevelList, QList<int> curExpList,
                                   QList<QString> lastEnterList);

public slots:
    void tableInitSlot();

    void checkUserNamePasswordSlot(QString name, QString password, bool flag);
    void modifyPasswordSlot(QString name, QString oldPassword, QString newPassword);
    void addNewUserSlot(QString name, QString password);

    void initRoleInfoTableSlot();

private:
    explicit MyDataBase(QObject *parent = nullptr);
    ~MyDataBase();

    void connectSignalSlot();
    void userTableInit();

private:
    static MyDataBase* m_pInstance;
    bool m_checkUserNamePasswordSta;
};

#endif // MYDATABASE_H
