#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QDialog>
#include "usertablemodel.h"

namespace Ui {
class UserManagement;
}

class UserManagement : public QDialog
{
    Q_OBJECT

public:
    static UserManagement* getInstance();
    ~UserManagement();

public slots:
    void checkUserNamePasswordCompleteSlot(int sta);
    void modifyPasswordCompleteSlot(bool flag);
    void checkUserNamePasswordToManagementCompleteSlot(int level);
    void modifyUserInfoCompleteSlot(int sta);
    void addNewUserCompleteSlot(int sta);
    void delUserCompleteSlot(int sta);
    void getUserInfoCompleteSlot(QList<QString> userNameList, QList<int> levelList);

signals:
    void checkUserNamePasswordSignal(QString userName, QString password, bool flag);
    void modifyPasswordSignal(QString userName, QString oldPassword, QString newPassword);
    void checkUserNamePasswordToManageSignal(QString userName, QString password);
    void modifyUserInfoSignal(QString userName, QString password, int level);
    void addNewUserSignal(QString userName, QString password, int level);
    void delUserSignal(QString userName);
    void getUserInfoSignal(int level);

private slots:
    void onItemClicked(const QModelIndex& index);

    void on_pushButton_login_clicked();
    void on_pushButton_modifyPassword_clicked();
    void on_pushButton_userManage_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_modifyConfirm_clicked();
    void on_pushButton_modifyCancel_clicked();
    void on_pushButton_userManagement_modify_clicked();
    void on_pushButton_userManagement_addUser_clicked();
    void on_pushButton_userManagement_delUser_clicked();
    void on_pushButton_userManagement_back_clicked();

private:
    explicit UserManagement(QWidget *parent = nullptr);
    void uiInit();
    void userInfoTableInit();

    void updateUserInfoTable(int level);
    void updateUserLevelComboBox(int level);

private:
    Ui::UserManagement *ui;

    static UserManagement* m_pInstance;

    UserTableModel *m_userTableModel;
    QList<USERDATA> m_userTableData;
    int m_level;
    int m_loginOrManage;
    int m_selectUserLevel;

    const QStringList m_levelList = {"测试", "管理员", "研发"};
};

#endif // USERMANAGEMENT_H
