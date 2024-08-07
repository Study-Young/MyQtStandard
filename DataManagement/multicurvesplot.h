﻿#ifndef MULTICURVESPLOT_H
#define MULTICURVESPLOT_H

#include <QObject>
#include "qcustomplot.h"
#include <QMouseEvent>
#include <QMap>
#include <QDebug>

typedef struct _CurveData
{
    QVector<double> timeKeyVec; //时间
    QVector<double> cntKeyVec;  //点数
    QVector<double> valVec;     //y
}CurveData;//单个曲线的数据

struct stZoomRangeLevel{
    double dXLower;
    double dXUpper;
    double dYLower;
    double dYUpper;
};

class MultiCurvesPlot : public QCustomPlot
{
    Q_OBJECT
public:
    MultiCurvesPlot(int _curvesCnt, QWidget *parent = 0);
    ~MultiCurvesPlot(){/*qDebug() << "delete MultiCurvesPlot";*/}

    void showCurves(QList<uint16_t> idxList);

    void setGraphVisible(int idx, bool visible);
    void setDataShowMode(int mode);
    void setShowPause(bool pause);
    void setXAxisShowMode(int mode);

    void setXAxisRange(double range);
    void setXAxisType(int type);
    void setYAxisRange(double lower, double upper);
    void setYAxisType(int type);
    void setYAxisAuto(bool flag);
    void setYAxisName(QString name);
    void setPause(bool enable);
    void setUpdateTimes(int cnt);
    int  getUpdateTimes();
    void setStartTime(QDateTime startTime);
    void setCalDataPlotProperty();

    void addData(const int idx, double time, double cnt, double value);
    void addData(const int idx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values);
    void setData(const int idx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values);
    void clearAllData();
    void clearIndexData(const int idx);

    void setCurveColor(int idx, int colorIdx);

    void updateMyPlot();
    void restoreBeforePlot();

signals:
    void updatePauseStaSignal(bool flag);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots: 
    void slotRect();
    void slotContexMenu();

private:
    void mainInit();
    void connectSignalsSlots();
    void memberValueInit();
    void uiInit();

    void contextMenuInit();
    void curvesDataInit();
    void setMyStyleSheet();
    void paintAttributeInit();
    void legendInit();

private:
    int m_curvesCnt;//支持的曲线数目
    QVector<CurveData> allCurvesData;//所有曲线的数据,allCurvesData[n]中又含有三个QVector，也即第n条曲线的x时间、x点数、y值

    QMap<int, QCPGraph*> curveIdx2graphPtr;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
    QVector<QColor> getColor;//记录所有曲线的颜色
    QVector<QString> getName;  //记录所有曲线的名称（图例）
    QList<uint16_t> idxList;//当前正在显示的曲线的编号

    bool slotRectFlag;
    bool pauseFlag;
    int dataShowMode;

    double xAxisShowRange = 1000;
    int xAxisType = 0;  //X轴类型，0：实际时间，1：点数
    bool yAxisAuto = true;
    int updateTimes = 0;
    double m_startTime = 0.0;

    enum DATASHOWMODE{FixedWindowMode = 0, PushMode};

    QAction *m_pActionShowAll;
    QAction *m_pActionShowPart;
    QAction *m_pActionPause;
    QAction *m_pActionContinue;
    QAction *m_pActionRestoreBefore;    // 显示还原至上级框选
    QAction *m_pTimeMode;
    QAction *m_pPointNumMode;
    QAction *m_pActionClear;

    QList<stZoomRangeLevel> m_listZoomLevel;

    QStringList colorListStr = {"#960000", "#009600", "#ff9600", "#ff0000",
                                "#ffff00", "#ff00ff", "#00ff00", "#ffffff",
                                "#960000", "#ff0000", "#ff9600", "#ffff00",
                                "#009600", "#00ff00", "#960096", "#ff00ff",
                                "#b22222", "#ff4757", "#00308f", "#0047ab",
                                "#cc8400", "#e1ad01", "#158466", "#20c997"};
};

#endif // MULTICURVESPLOT_H
