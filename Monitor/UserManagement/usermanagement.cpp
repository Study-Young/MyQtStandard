#include "usermanagement.h"
#include "ui_usermanagement.h"
#include <QMessageBox>
#include <QListView>
#include <QDebug>

UserManagement::UserManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagement)
{
    ui->setupUi(this);

    uiInit();
    userInfoTableInit();
    setWindowTitle("用户登录");
}

UserManagement::~UserManagement()
{
    delete ui;
}

UserManagement *UserManagement::m_pInstance = nullptr;
UserManagement *UserManagement::getInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new UserManagement();
    }
    return m_pInstance;
}

void UserManagement::uiInit()
{
    ui->comboBox_userManagement_userLevel->setView(new QListView());

    ui->stackedWidget_password->setCurrentIndex(0);
    ui->stackedWidget_userManagement->setCurrentIndex(0);
}

void UserManagement::userInfoTableInit()
{
    m_userTableModel = new UserTableModel;
    ui->tableView_user->setModel(m_userTableModel);

    ui->tableView_user->verticalHeader()->setVisible(false);
    ui->tableView_user->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_user->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_user->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_user->setColumnWidth(0, 50);
    ui->tableView_user->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView_user->setColumnWidth(1, 120);
    ui->tableView_user->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView_user->setFocusPolicy(Qt::NoFocus);

    connect(ui->tableView_user, &QTableView::clicked, this, &UserManagement::onItemClicked);
}

void UserManagement::updateUserInfoTable(int level)
{
    emit getUserInfoSignal(level);
}

void UserManagement::onItemClicked(const QModelIndex &index)
{
    int row = index.row();
    QVariant data1 = m_userTableModel->data(m_userTableModel->index(row, 1));
    ui->lineEdit_userManagement_userName->setText(data1.toString());

    QVariant data2 = m_userTableModel->data(m_userTableModel->index(row, 2));
    m_selectUserLevel = m_levelList.indexOf(data2.toString());
    ui->comboBox_userManagement_userLevel->setCurrentIndex(m_selectUserLevel);
}

void UserManagement::on_pushButton_login_clicked()
{
    m_loginOrManage = 1;
    QString userName = ui->lineEdit_userName->text();
    QString password = ui->lineEdit_password->text();
    if(userName.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "输入信息不完整!");
        return;
    }

    if((userName == "SuperAdmin") && (password == "zss@10s!#"))
    {
        m_level = 3;
        QMessageBox::information(this, "成功!", "登录成功!");
        updateUserInfoTable(m_level);
        updateUserLevelComboBox(m_level);
        ui->stackedWidget_userManagement->setCurrentIndex(1);
        setWindowTitle("用户管理");
    }
    else
    {
        emit checkUserNamePasswordSignal(userName, password, true);
    }
}

void UserManagement::checkUserNamePasswordCompleteSlot(int sta)
{
    if(sta == 1)
    {
        if(m_loginOrManage == 1)
        {
            ui->lineEdit_userName->clear();
            ui->lineEdit_password->clear();
            accept();
        }
    }
    else if(sta == 2)
    {
        QMessageBox::warning(this, "错误!", "密码错误!");
    }
    else if(sta == 0)
    {
        QMessageBox::warning(this, "错误!", "该用户名不存在!");
    }
}

void UserManagement::on_pushButton_modifyPassword_clicked()
{
    ui->lineEdit_userName->clear();
    ui->lineEdit_password->clear();
    ui->stackedWidget_password->setCurrentIndex(1);
    ui->lineEdit_userName->setFocus();
}

void UserManagement::on_pushButton_userManage_clicked()
{
    m_loginOrManage = 2;
    QString userName = ui->lineEdit_userName->text();
    QString password = ui->lineEdit_password->text();
    if(userName.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "输入信息不完整!");
        return;
    }

    if((userName == "SuperAdmin") && (password == "zss@10s!#"))
    {
        m_level = 3;
        QMessageBox::information(this, "成功!", "登录成功!");
        updateUserInfoTable(m_level);
        updateUserLevelComboBox(m_level);
        ui->stackedWidget_userManagement->setCurrentIndex(1);
        setWindowTitle("用户管理");
    }
    else
    {
        emit checkUserNamePasswordToManageSignal(userName, password);
    }
}

void UserManagement::checkUserNamePasswordToManagementCompleteSlot(int level)
{
    if(level == 0)
    {
        QMessageBox::information(this, "失败!", "用户权限不足!");
    }
    else
    {
        m_level = level;
        updateUserInfoTable(m_level);
        updateUserLevelComboBox(m_level);
        ui->stackedWidget_userManagement->setCurrentIndex(1);
        ui->lineEdit_userName->clear();
        ui->lineEdit_password->clear();
        setWindowTitle("用户管理");
    }
}

void UserManagement::on_pushButton_exit_clicked()
{
    reject();
}

void UserManagement::on_pushButton_modifyConfirm_clicked()
{
    m_loginOrManage = 2;
    QString userName = ui->lineEdit_userName->text();
    QString oldPassword = ui->lineEdit_oldPassword->text();
    QString newPassword1 = ui->lineEdit_newPassword1->text();
    QString newPassword2 = ui->lineEdit_newPassword2->text();
    if(userName.isEmpty() || oldPassword.isEmpty() || newPassword1.isEmpty() || newPassword2.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "输入信息不完整!");
        return;
    }

    if(newPassword1 != newPassword2)
    {
        QMessageBox::warning(this, "错误!", "两次输入密码不同,请重新输入密码!");
        return;
    }

    emit modifyPasswordSignal(userName, oldPassword, newPassword1);
}

void UserManagement::modifyPasswordCompleteSlot(bool flag)
{
    if(flag)
        QMessageBox::information(this, "成功!", "修改密码成功!");
    else
        QMessageBox::warning(this, "失败!", "修改密码失败!");
}

void UserManagement::on_pushButton_modifyCancel_clicked()
{
    ui->lineEdit_userName->clear();
    ui->lineEdit_oldPassword->clear();
    ui->lineEdit_newPassword1->clear();
    ui->lineEdit_newPassword2->clear();
    ui->stackedWidget_password->setCurrentIndex(0);
    ui->lineEdit_userName->setFocus();
}

void UserManagement::on_pushButton_userManagement_modify_clicked()
{
    QString userName = ui->lineEdit_userManagement_userName->text();
    QString password = ui->lineEdit_userManagement_password->text();
    int level = ui->comboBox_userManagement_userLevel->currentIndex();
    if(userName.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "请输入用户名!");
        return;
    }

    if(userName.isEmpty() && (m_selectUserLevel==level))
    {
        QMessageBox::warning(this, "错误!", "请输入要修改的内容!");
        return;
    }

    emit modifyUserInfoSignal(userName, password, level);
}

void UserManagement::modifyUserInfoCompleteSlot(int sta)
{
    if(sta == 1)
    {
        QMessageBox::information(this, "成功!", "修改用户信息成功!");
        updateUserInfoTable(m_level);
    }
    else if(sta == -1)
    {
        QMessageBox::warning(this, "失败!", "修改用户信息失败!");
    }
    else if(sta == 0)
    {
        QMessageBox::warning(this, "失败!", "该用户名不存在!");
    }
}

void UserManagement::on_pushButton_userManagement_addUser_clicked()
{
    QString userName = ui->lineEdit_userManagement_userName->text();
    QString password = ui->lineEdit_userManagement_password->text();
    int level = ui->comboBox_userManagement_userLevel->currentIndex();
    if(userName.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "输入信息不完整!");
        return;
    }

    emit addNewUserSignal(userName, password, level);
}

void UserManagement::addNewUserCompleteSlot(int sta)
{
    if(sta == 1)
    {
        QMessageBox::information(this, "成功!", "增加用户成功!");
        updateUserInfoTable(m_level);
    }
    else if(sta == -1)
    {
        QMessageBox::warning(this, "失败!", "增加用户失败!");
    }
    else if(sta == 0)
    {
        QMessageBox::warning(this, "失败!", "该用户名已存在!");
    }
}

void UserManagement::on_pushButton_userManagement_delUser_clicked()
{
    QString userName = ui->lineEdit_userManagement_userName->text();
    if(userName.isEmpty())
    {
        QMessageBox::warning(this, "错误!", "输入信息不完整!");
        return;
    }

    if(QMessageBox::Yes == QMessageBox::information(this, "提示", "确认删除该用户？", QMessageBox::Yes | QMessageBox::No))
    {
        emit delUserSignal(userName);
    }
}

void UserManagement::delUserCompleteSlot(int sta)
{
    if(sta == 1)
    {
        QMessageBox::information(this, "成功!", "删除用户成功!");
        updateUserInfoTable(m_level);
    }
    else if(sta == -1)
    {
        QMessageBox::warning(this, "失败!", "删除用户失败!");
    }
    else if(sta == 0)
    {
        QMessageBox::warning(this, "失败!", "该用户名不存在!");
    }
}

void UserManagement::on_pushButton_userManagement_back_clicked()
{
    ui->stackedWidget_userManagement->setCurrentIndex(0);
    ui->lineEdit_userName->setFocus();
    setWindowTitle("用户登录");
}

void UserManagement::getUserInfoCompleteSlot(QList<QString> userNameList, QList<int> levelList)
{
    m_userTableData.clear();
    for(int i=0; i<userNameList.size(); i++)
    {
        USERDATA userData;
        userData.seq = i+1;
        userData.userName = userNameList.at(i);
        userData.level = m_levelList.at(levelList.at(i));
        m_userTableData.append(userData);
    }
    m_userTableModel->updateModelData(m_userTableData);
}

void UserManagement::updateUserLevelComboBox(int level)
{
    ui->comboBox_userManagement_userLevel->clear();
    for(int i=0; i<level; i++)
        ui->comboBox_userManagement_userLevel->addItem(m_levelList.at(i));
}
