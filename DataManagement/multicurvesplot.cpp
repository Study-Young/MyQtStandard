#include "multicurvesplot.h"
#include <QColorDialog>
#include <QDebug>

#pragma execution_character_set("utf-8")

MultiCurvesPlot::MultiCurvesPlot(int curvesCnt, QWidget *parent):
    QCustomPlot(parent)
    ,allCurvesData(curvesCnt)
{
    mainInit();
    connectSignalsSlots();
    startTimer(100, Qt::CoarseTimer);//定频刷新曲线图
}

void MultiCurvesPlot::mainInit()
{
    memberValueInit();
    uiInit();
}

void MultiCurvesPlot::connectSignalsSlots()
{
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));//左右y轴同步放缩
    connect(this->selectionRect(), &QCPSelectionRect::accepted, this, &MultiCurvesPlot::slotRect);
}

void MultiCurvesPlot::memberValueInit()
{
    tracer = new QCPItemTracer(this);       //游标
    tracerXText = new QCPItemText(this);    //游标的X值文本框
    tracerYText = new QCPItemText(this);    //游标的Y值文本框
    traceItemMode = TRACEITEMMODE::MouseTraceItem;
    curvesCnt = allCurvesData.size();

    pauseFlag = false;
    tracerEnable = false;
    dataShowMode = 0;
}

void MultiCurvesPlot::uiInit()
{
    this->setMouseTracking(true);
    setStyleSheet();
    setMouseTraceItem();
    paintAttributeInit();
    controlsInit();
    curvesDataInit();
}

void MultiCurvesPlot::showCurves(QList<uint16_t> _idxList)
{
    this->clearGraphs();//先移除所有的graph
    idxList = _idxList;//备份，可能别的函数需要它
    curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射

    int graphIdx = 0;

    for(QList<uint16_t>::const_iterator it = idxList.begin(); it != idxList.end(); it++)
    {   //*it为每一个要显示的曲线编号
        uint16_t curveIdx = *it;
        if(curveIdx > curvesCnt)
        {
            qDebug() << QString("warning: MultiCurvesPlot::showCurves->超出数据源max index").arg(*it);
            continue;
        }

        this->addGraph(this->xAxis, this->yAxis);
        QCPGraph* pGraph = graph(graphIdx);
        curveIdx2graphPtr[curveIdx] = pGraph;//记录：曲线索引->graph指针的映射
        pGraph->setPen(QPen(getColor[curveIdx]));//线的颜色随机
        pGraph->setLineStyle(QCPGraph::lsLine);//阶梯线样式
        this->graph(graphIdx)->setName(getName[curveIdx]);
        graphIdx++;
    }

    if(graphCount() > 0)
        traceGraph = graph(0);
    else
        traceGraph = NULL;
//    showAllGraph();
    this->replot();
}

void MultiCurvesPlot::setGraphVisible(int channelNum, bool visible)
{
    this->graph(channelNum)->setVisible(visible);
}

void  MultiCurvesPlot::setXAxis(double range)
{
    xAxisShowRange =  range;//设置X轴的范围
}

void MultiCurvesPlot::setXAxisType(int type)
{
    xAxisType = type;
    if(xAxisType == 0)
    {
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
        dateTicker->setDateTimeFormat("hh:mm:ss\nyyyy-MM-dd");//日期格式(可参考QDateTime::fromString()函数)
        this->xAxis->setTicker(dateTicker);//设置X轴为时间轴

        if(!allCurvesData[0].valVec.isEmpty())
        {
            QCPGraph* pGraph = curveIdx2graphPtr[0];
            pGraph->data().data()->clear();
            pGraph->setData(allCurvesData[0].timeKeyVec, allCurvesData[0].valVec);
        }
    }
    else if(xAxisType == 1)
    {
        QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);//点数做X轴
        fixedTicker->setScaleStrategy( QCPAxisTickerFixed::ssMultiples );
        this->xAxis->setTicker(fixedTicker);
        this->xAxis->setRange(0,xAxisShowRange);

        if(!allCurvesData[0].valVec.isEmpty())
        {
            QCPGraph* pGraph = curveIdx2graphPtr[0];
            pGraph->data().data()->clear();
            pGraph->setData(allCurvesData[0].cntKeyVec, allCurvesData[0].valVec);
        }
    }
    else if(xAxisType == 2)
    {
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);//log做X轴
        logTicker->setLogBase(10);
        this->xAxis->setTicker(logTicker);
        this->xAxis->setScaleType(QCPAxis::stLogarithmic);
        this->xAxis->setRange(1,110000);
    }

    this->xAxis->rescale(true);
    this->yAxis->rescale(true);
    this->replot();
}

void MultiCurvesPlot::setYAxis(double lower, double upper)
{
    this->yAxis->setRange(lower, upper);//设置Y轴的范围
}

void MultiCurvesPlot::setYAxisType(int type)
{
    if(type == 0)
    {

    }
    else if(type == 1)
    {
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);//log做X轴
        logTicker->setLogBase(10);
        this->yAxis->setTicker(logTicker);
        this->yAxis->setScaleType(QCPAxis::stLogarithmic);
        this->yAxis2->setTicker(logTicker);
        this->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    }
    this->replot();
}

void MultiCurvesPlot::setYAxisAuto(bool flag)
{
    yAxisAuto = flag;
}


void MultiCurvesPlot::setUpdateTimes(int cnt)
{
    updateTimes = cnt;
}

void MultiCurvesPlot::setCurveName(int idx, const QString newName)
{
    if(idx < getName.size())
        getName[idx] = newName;
    else
        qDebug() << "warning: MultiCurvesPlot::setCurveName -> invalid idx!";
}

void MultiCurvesPlot::setCurvesName(QVector<QString> _nameVec)
{
    _nameVec.resize(getName.size());//截断或补长
    getName = _nameVec;
}

void MultiCurvesPlot::addData(int idx, double time, double cnt, double y)
{
    if(idx > curvesCnt)
    {
        qDebug() << QString("warning:下位机企图绘制编号为%1的曲线！").arg(idx);
        return;
    }
//    if(y > 1e10 || y < -1e10)//接收到的异常值直接不绘制
//        return;

    allCurvesData[idx].timeKeyVec.append(time);//备份数据源
    allCurvesData[idx].cntKeyVec.append(cnt);//备份数据源
    allCurvesData[idx].valVec.append(y);

    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];
        if(xAxisType == 0)
            pGraph->addData(time, y);
        else
            pGraph->addData(cnt, y);
//        pGraph->data().data()->removeBefore(0);
        updateTimes++;
    }
}

void MultiCurvesPlot::setData(int curveIdx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values)
{
    if(curveIdx > curvesCnt)
    {
        qDebug() << QString("warning:下位机企图绘制编号为%1的曲线！").arg(curveIdx);
        return;
    }

    allCurvesData[curveIdx].timeKeyVec.append(times);//备份数据源
    allCurvesData[curveIdx].cntKeyVec.append(cnts);//备份数据源
    allCurvesData[curveIdx].valVec.append(values);

    if(curveIdx2graphPtr.contains(curveIdx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[curveIdx];
        pGraph->data().data()->clear();

        if(xAxisType == 0)
        {
            pGraph->setData(times, values);
        }
        else
        {
            pGraph->setData(cnts, values);
        }
    }
}

void MultiCurvesPlot::setPause(bool enable)
{
    pauseFlag = enable;
}

void MultiCurvesPlot::setDataShowMode(int mode)
{
    dataShowMode = mode;
}

void MultiCurvesPlot::clearAllData()
{
    for(int i = 0; i < graphCount(); i++)
    {
        graph(i)->data().data()->clear();
    }

    for(int idx = 0; idx < allCurvesData.size(); idx++)
    {
        allCurvesData[idx].timeKeyVec.clear();
        allCurvesData[idx].cntKeyVec.clear();
        allCurvesData[idx].valVec.clear();
    }

    updateTimes = 0;
}

void MultiCurvesPlot::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(!pauseFlag)
    {
        switch (dataShowMode) {
        case DATASHOWMODE::FixedWindowMode:
            if(xAxisType == 0)
            {
                double curSeclf;
                if(allCurvesData[0].timeKeyVec.isEmpty())
                    curSeclf = (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
                else
                    curSeclf = allCurvesData[0].timeKeyVec.last();
                this->xAxis->setRange(curSeclf - xAxisShowRange, curSeclf);
            }
            else if(xAxisType == 1)
            {
                if(updateTimes>xAxisShowRange)
                {
                    this->xAxis->setRange(updateTimes - xAxisShowRange, updateTimes);
                }
                else
                {
                    this->xAxis->setRange(0, xAxisShowRange);
                }
            }
            else if(xAxisType == 2)
            {
                this->xAxis->setRange(1, 110000);
            }

            if(yAxisAuto)
                this->yAxis->rescale(true);
            break;
        case DATASHOWMODE::PushMode:
            this->xAxis->rescale(true);
            if(yAxisAuto)
                this->yAxis->rescale(true);
            break;
        default:
            break;
        }
    }

    if(traceItemMode == TRACEITEMMODE::FixedTraceItem)
    {
        getTracerItemCurValue();
    }

    this->replot();
}

void MultiCurvesPlot::setTracerEnable(bool enable)
{
    tracerEnable = enable;
    if(tracerEnable)
    {
        tracer->setVisible(true);
        tracerXText->setVisible(true);
        tracerYText->setVisible(true);
    }
    else
    {
        tracer->setVisible(false);
        tracerXText->setVisible(false);
        tracerYText->setVisible(false);
    }
}

void MultiCurvesPlot::setStyleSheet()
{
    this->setBackground(QColor(0, 32, 96));

    this->xAxis->setBasePen(QPen(QColor(102, 255, 255), 1));        // 轴线的画笔
    this->xAxis->setTickPen(QPen(QColor(102, 255, 255), 1));        // 轴刻度线的画笔
    this->xAxis->setSubTickPen(QPen(QColor(102, 255, 255), 1));     // 轴子刻度线的画笔
    this->xAxis->setTickLabelColor(QColor(102, 255, 255));          // 轴刻度文字颜色
    this->xAxis->setLabelColor(QColor(102, 255, 255));
    this->xAxis->setTickLengthIn(3);
    this->xAxis->setTickLengthOut(5);

    this->yAxis->setBasePen(QPen(QColor(102, 255, 255), 1));        // 轴线的画笔
    this->yAxis->setTickPen(QPen(QColor(102, 255, 255), 1));        // 轴刻度线的画笔
    this->yAxis->setSubTickPen(QPen(QColor(102, 255, 255), 1));     // 轴子刻度线的画笔
    this->yAxis->setTickLabelColor(QColor(102, 255, 255));          // 轴刻度文字颜色
    this->yAxis->setLabelColor(QColor(102, 255, 255));

    this->yAxis2->setBasePen(QPen(QColor(102, 255, 255), 1));       // 轴线的画笔
    this->yAxis2->setTickPen(QPen(QColor(102, 255, 255), 1));       // 轴刻度线的画笔
    this->yAxis2->setSubTickPen(QPen(QColor(102, 255, 255), 1));    // 轴子刻度线的画笔
    this->yAxis2->setTickLabelColor(QColor(102, 255, 255));         // 轴刻度文字颜色

    this->xAxis->grid()->setPen(QPen(QColor(183, 255, 255), 1, Qt::DashLine));
    this->yAxis->grid()->setPen(QPen(QColor(183, 255, 255), 1, Qt::DashLine));
    this->xAxis->grid()->setSubGridPen(QPen(QColor(183, 255, 255), 1, Qt::DotLine));
    this->yAxis->grid()->setSubGridPen(QPen(QColor(183, 255, 255), 1, Qt::DotLine));
    this->xAxis->grid()->setSubGridVisible(true);
    this->yAxis->grid()->setSubGridVisible(true);
    this->xAxis->grid()->setZeroLinePen(QPen(QColor(183, 255, 255)));
    this->yAxis->grid()->setZeroLinePen(QPen(QColor(183, 255, 255)));
}

void MultiCurvesPlot::setMouseTraceItem()
{
    //游标以及游标文本框设置
    tracer->setInterpolating(false);//禁用插值
    tracer->setPen(QPen(Qt::DashLine));//虚线游标
    tracer->setStyle(QCPItemTracer::tsCrosshair);//游标样式：十字星、圆圈、方框等

    tracerXText->setPositionAlignment(Qt::AlignBottom|Qt::AlignRight);
    tracerXText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerXText->position->setCoords(0, 0.99); // 在plot中的位置
    tracerXText->position->setParentAnchorX(tracer->position);//X位置锚定到游标的X位置
    tracerXText->setText("no curves..");
    tracerXText->setFont(QFont(font().family(), 12));
    tracerXText->setPen(QPen(Qt::black));
    tracerXText->setBackgroundColor(Qt::green);
    tracerXText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerXText->setVisible(false);

    tracerYText->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
    tracerYText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerYText->position->setCoords(0.01, 0); // 在plot中的位置
    tracerYText->setText("no curves..");
    tracerYText->position->setParentAnchorY(tracer->position);//Y位置锚定到游标的Y位置
    tracerYText->setFont(QFont(font().family(), 12));
    tracerYText->setPen(QPen(Qt::black));
    tracerYText->setBackgroundColor(Qt::green);
    tracerYText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerYText->setVisible(false);
}

void MultiCurvesPlot::paintAttributeInit()
{
    this->setInteractions(QCP::iRangeDrag //可平移
                          | QCP::iRangeZoom //可滚轮缩放
    //                          | QCP::iSelectPlottables //可选中曲线
                          | QCP::iSelectLegend );//可选中图例
    this->setNoAntialiasingOnDrag(true);//禁用抗锯齿，以提高性能
    this->setSelectionRectMode(QCP::srmCustom);
}

void MultiCurvesPlot::controlsInit()
{
//    this->setOpenGl(true);
//    qDebug() << "QCustomplot opengl status = " << this->openGl();
    this->yAxis2->setVisible(true);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
    dateTicker->setDateTimeFormat("hh:mm:ss\nyyyy-MM-dd");//日期格式(可参考QDateTime::fromString()函数)
    this->xAxis->setTicker(dateTicker);//设置X轴为时间轴
    this->xAxis->setTickLabels(true);//显示刻度标签
}

void MultiCurvesPlot::curvesDataInit()
{
    getColor.resize(curvesCnt);
    for(int i = 0; i < getColor.size();i++)
    {
        getColor[i] = QColor(colorListStr.at(i));
    }
    getName.resize(curvesCnt);
    for(int i = 0; i < getName.size();i++)//设置默认曲线名称
    {
        getName[i] = QString("通道 %1").arg(i+1);
    }

    for(int idx = 0; idx < allCurvesData.size(); idx++)
    {
        allCurvesData[idx].timeKeyVec.clear();
        allCurvesData[idx].cntKeyVec.clear();
        allCurvesData[idx].valVec.clear();
    }
}


void MultiCurvesPlot::getTracerItemCurValue()
{
    tracer->setGraph(traceGraph);
    tracer->setGraphKey(mouseXPos);
}

void MultiCurvesPlot::setScatterPointEnable(bool enable)
{
    for (int i = 0; i < this->graphCount(); ++i)
    {
        if(enable)
//            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));//显示散点
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));//显示散点
        else
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));//不显示散点
    }
}

void MultiCurvesPlot::slotRect()
{
    pauseFlag = true;
    emit updatePauseStaSignal(pauseFlag);
}

