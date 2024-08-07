﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "global.h"
#include "datastruct.h"

#include "catalogwidget.h"
#include "catalogwidget2.h"
#include "iconwidget.h"

#include "LogManagement/logmanagement.h"
#include "ConfigManagement/configmanagement.h"
#include "DataBase/mydatabase.h"
#include "UserManagement/usermanagement.h"

#include "Model/multicurvesplot.h"
#include "Model/itemdelegate.h"
#include "Model/commontablemodel.h"
#include "Model/MyTab/SapidTab/linetab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateCurrentUserInfo();

signals:
    void recordErrorSignal(QString strTime, QString strFile, QString strFunc, QString strError, QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);
    void recordEventSignal(QString strTime, QString strFile, QString strFunc, QString strEvent, QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);
    void recordOperatorSignal(QString strTime, QString strFile, QString strFunc, QString strOperator, QString strFolder1 = NULL, QString strFolder2 = NULL, QString strFolder3 = NULL);

    void databaseInitSignal(QString type);

public slots:
    void updateMsg(const QString &fileName, const QString &functionName, EN_MessageType msgType, const QString &msg);
    void databaseInitCompleteSlot(bool sta);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void catalogBtnClicked();

    void on_pushButton_messageTipsTest_clicked();

private:
    void mainInit();
    void memberValueInit();
    void uiInit();

    void signalSlotInit();
    void configFileInit();
    void databaseInit();
    void workLogFileInit();
    void plotInit();
    void tableInit();
    void btnIndicatorInit();
    void pushButtonInit();

    void catalogWidgetInit();
    void iconWidgetInit();

    void styleSheetInit();
    void updateStyleSheet();

    void commonTableInit();
    void functionBtnClicked();
    void slideBtnClicked();
    void slideBtn2Clicked();

    void selectCurveSlot(int sta, int level, int pos);
    void showCurveSlot(bool checked);
    void onHeaderDoubleClickedSlot(int index);

private:
    Ui::MainWindow *ui;
    QByteArray m_qssByteArray;

    MyDataBase *m_myDatabase;

    ConfigManagement *m_configManagement;
    LogManagement *m_logManagement;

    MultiCurvesPlot *m_commonDataPlot;
    MultiCurvesPlot *m_logDataPlot;

    CommonTableModel *m_commonTableModel;
    QList<TESTDATA> m_commonTableData;
    QMap<QString, QCheckBox *> m_curvesMap;

    LineTab *m_lineTab;
    QButtonGroup *m_slideBtnGroup;

    LineTab *m_lineTab2;
    QButtonGroup *m_slideBtnGroup2;

    LineTab *m_lineTab3;
    QButtonGroup *m_catalogBtnGroup;
    QList<QPushButton *> m_functionBtnList;
};
#endif // MAINWINDOW_H
