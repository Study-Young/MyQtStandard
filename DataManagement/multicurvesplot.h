#ifndef MULTICURVESPLOT_H
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

class MultiCurvesPlot : public QCustomPlot
{
    Q_OBJECT
public:
    MultiCurvesPlot(int _curvesCnt, QWidget *parent = 0);
    ~MultiCurvesPlot(){/*qDebug() << "delete MultiCurvesPlot";*/}

    void setCurveName(int idx, const QString newName);
    void setCurvesName(QVector<QString> _nameVec);
    void showCurves(QList<uint16_t> _idxList);
    void setGraphVisible(int channelNum, bool visible);

    void setXAxis(double range);
    void setXAxisType(int type);
    void setYAxis(double lower, double upper);
    void setYAxisType(int type);
    void setYAxisAuto(bool flag);
    void setUpdateTimes(int cnt);
    void setDataShowMode(int mode);
    void setPause(bool enable);

    void addData(int curveIdx, double time, double cnt, double y);
    void setData(int curveIdx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values);
    void clearAllData();

    void setTracerEnable(bool enable);//是否使能游标
    void setScatterPointEnable(bool enable);//是否使能散点

signals:
    void updatePauseStaSignal(bool flag);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private slots: 
    void slotRect();

private:
    void mainInit();
    void connectSignalsSlots();
    void memberValueInit();
    void uiInit();
    void setStyleSheet();
    void setMouseTraceItem();
    void paintAttributeInit();
    void controlsInit();
    void curvesDataInit();
    void getTracerItemCurValue();

private:
    MultiCurvesPlot();
    QVector<CurveData> allCurvesData;//所有曲线的数据,allCurvesData[n]中又含有三个QVector，也即第n条曲线的x时间、x点数、y值
    int curvesCnt;//支持的曲线数目
    QMap<int, QCPGraph*> curveIdx2graphPtr;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
    QVector<QColor> getColor;//记录所有曲线的颜色
    QVector<QString> getName;  //记录所有曲线的名称（图例）
    QList<uint16_t> idxList;//当前正在显示的曲线的编号
    bool pauseFlag;
    int dataShowMode;
    int traceItemMode;
    double mouseXPos;

    /*游标相关组件*/
    QCPItemTracer *tracer;          //游标
    QCPGraph *traceGraph;           //游标要吸附哪个graph
    QCPItemText *tracerXText;       //用于实时显示游标X值
    QCPItemText *tracerYText;       //用于实时显示游标Y值
    bool tracerEnable;//是否使能游标

    /*记忆轴的范围组件*/
    QCPRange Xscope;//记录X轴的范围
    QCPRange Yscope;//记录Y轴的范围

    double xAxisShowRange = 1000;
    int xAxisType = 0;  //X轴类型，0：实际时间，1：点数
    bool yAxisAuto = true;
    int updateTimes = 0;

    QStringList colorListStr = {"#ff0000", "#ffff00", "#00ff00", "#0000ff",
                                "#00ffff", "#ff00ff", "#a6937c", "#0075a9",
                                "#8957a1", "#b28850", "#8a8000", "#7e0043",
                                "#601986", "#226059", "#246e07", "#ab8915",
                                "#ef88bb", "#ff6262", "#b4f9f2", "#afa7ff",
                                "#75afff", "#835000", "#ccdb64", "#cd0777"};
    enum TRACEITEMMODE{FixedTraceItem = 0, MouseTraceItem};
    enum DATASHOWMODE{FixedWindowMode = 0, PushMode};
};

#endif // MULTICURVESPLOT_H
