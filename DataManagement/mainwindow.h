#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QTableView>
#include <QHeaderView>
#include <QTextStream>

#include <QDebug>

#include "databasetablemodel.h"
#include "caldatatablemodel.h"
#include "multicurvesplot.h"
#include "datastruct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_query_clicked();
    void on_pushButton_read_clicked();
    void on_pushButton_downLoad_clicked();
    void on_pushButton_firstPage_clicked();
    void on_pushButton_pageUp_clicked();
    void on_pushButton_pageDown_clicked();
    void on_pushButton_endPage_clicked();
    void on_pushButton_toThePage_clicked();
    void on_pushButton_rowCountsSet_clicked();
    void headerViewOnClicked(int colIndex);
    void on_pushButton_adev_clicked();
    void on_pushButton_tdev_clicked();
    void on_pushButton_showAll_clicked();
    void on_pushButton_clear_clicked();

    void on_pushButton_xAxisType_clicked();
    void on_pushButton_plot_clicked();
    void on_pushButton_resetTimeRange_clicked();

private:
    void mainIint();
    void memberValueInit();
    void uiInit();
    void connectSignalSlot();
    void styleSheetInit();
    void updateStyleSheet();
    void updateLogInfo(QString info);

    void dataBaseInit();
    void dataBaseTableInit();                   //创建窗口
    void updateDataBaseTableStatus();       //刷新状态
    void currentPageEdit(QString pageStr);
    void plotInit();
    void calDataTableInit();

    void initDataPlotTable();
    void updateReadDataPlot();
    void updateCalDataPlot(int type);
    void updateCalDataTable();

    void calculateFreqAllan(const QVector<double>& valueVct);
    void calculateTimeIntervalFreqAllan(const double freq, const int interval, const int allanDataIdx);
    void calculateTdev(const QVector<double>& values);
    void calculateTimeIntervalTdev(const QVector<double> &values, const int interval, const int tdevDataIdx);

    QString getDataType(int index);
    QString getPlotYAxisName(int index);
    int getPlotYAxisType(int index);

private:
    Ui::MainWindow *ui;
    QByteArray qssByteArray;
    QSqlDatabase *m_pDataBase;
    DataBaseTableModel *m_pModel;
    QTableView *m_pTableView;           //数据表

    const QString m_databaseName = "RF4600.db";
    QString m_selectDataTableName;
    QString m_selectDeviceInfoTableName;

    QString m_selectDeviceDataStr;

    QString m_dataStartTime;
    QString m_dataEndTime;
    int m_startPoint;
    int m_stopPoint;
    QStringList m_dateTimeStr;
    QVector<double> m_dateTimes;
    QVector<double> m_dataKeys;
    QVector<double> m_readDatas;

    MultiCurvesPlot *m_oriDataPlot;
    MultiCurvesPlot *m_calDataPlot;
    CalDataTableModel *m_calDataTableModel;
    QTableView *m_calDataTableView;

    QVector<double> m_calIndex;
    QVector<double> m_calValues;
    FreqAllanStruct m_allanData[CAL_INTERVAL_CNT];
    TdevStruct m_tdevData[CAL_INTERVAL_CNT];
};
#endif // MAINWINDOW_H
