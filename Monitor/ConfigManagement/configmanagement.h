#ifndef INIHELPEMENT_H
#define INIHELPEMENT_H

#include <QObject>

class ConfigManagement : public QObject
{
    Q_OBJECT

public:
    static ConfigManagement *getInstance();
    ~ConfigManagement();

    void setDatabaseIp(QString ip);
    QString getDatabaseIp();

    void setDatabaseUserName(QString name);
    QString getDatabaseUserName();

    void setDatabasePassword(QString password);
    QString getDatabasePassword();

    void setSaveWorkLogFlag(int flag);
    bool getSaveWorkLogFlag();

private:
    explicit ConfigManagement(QObject *parent = nullptr);

    void mainInit();
    void connectSignalSlot();
    void memberValueInit();
    void writeNewCaliFile(QString fileName);

private:
    static ConfigManagement* m_pInstance;
    QString m_fileName;
};

#endif // INIHELPEMENT_H
