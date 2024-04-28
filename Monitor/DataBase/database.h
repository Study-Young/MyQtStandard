#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QThread>

#include "global.h"

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

signals:
    void updateMsgSignal(const QString& fileName, const QString& functionName, EN_MessageType msgType, const QString& msg);
    void databaseInitCompleteSignal(bool flag);
    void tableInitSignal();

public slots:
    void databaseInitSlot(QString type);

protected:
    void mainInit();
    void connectSignalSlot();
    void memberValueInit();

    bool isTableExist(QString tableName);
    bool execSql(QString &sqlStr, const QString &msg);
    bool execSqlList(QList<QString> &sqlList, const QString &msg);

protected:
    QThread *m_thread;
    QSqlDatabase *m_dataBase;
    QSqlDatabase *m_userBase;
};

#endif // DATABASE_H
