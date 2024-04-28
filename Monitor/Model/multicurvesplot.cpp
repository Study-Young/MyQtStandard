#include "multicurvesplot.h"

MultiCurvesPlot::MultiCurvesPlot(int curvesCnt, QWidget *parent):
    allCurvesData(curvesCnt),
    QCustomPlot(parent)
{
    mainInit();
    connectSignalsSlots();
}

MultiCurvesPlot::~MultiCurvesPlot()
{

}

void MultiCurvesPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        if(pauseFlag && slotRectFlag)
        {
            slotRectFlag = false;

            stZoomRangeLevel stRangeLevel;
            if(this->xAxis->visible())
            {
                stRangeLevel.dXLower = this->xAxis->range().lower;
                stRangeLevel.dXUpper = this->xAxis->range().upper;
            }
            if(this->yAxis->visible())
            {
                stRangeLevel.dYLower = this->yAxis->range().lower;
                stRangeLevel.dYUpper = this->yAxis->range().upper;
            }

            m_listZoomLevel.insert(0, stRangeLevel);
            if(m_listZoomLevel.size() > 1)
            {
                if(!this->actions().contains(m_pActionRestoreBefore))
                    this->insertAction(m_pActionContinue, m_pActionRestoreBefore);
            }
        }
    }
}

void MultiCurvesPlot::mainInit()
{
    memberValueInit();
    uiInit();
}

void MultiCurvesPlot::connectSignalsSlots()
{
    connect(this->selectionRect(), &QCPSelectionRect::accepted, this, &MultiCurvesPlot::slotRect);
}

void MultiCurvesPlot::memberValueInit()
{
    curvesCnt = allCurvesData.size();
    updateTimes = 0;
    slotRectFlag = false;
    pauseFlag = false;
    dataShowMode = 0;
    startTime = QDateTime::currentSecsSinceEpoch();

    contextMenuInit();
    curvesDataInit();
}

void MultiCurvesPlot::uiInit()
{
    setMyStyleSheet();
    paintAttributeInit();
//    legendInit();
}

void MultiCurvesPlot::contextMenuInit()
{
    this->setSelectionRectMode(QCP::srmZoom);
    //右键菜单
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    m_pActionRestoreBefore = new QAction("返回上一级");
    connect(m_pActionRestoreBefore, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pActionShowAll = new QAction("显示全部数据");
    connect(m_pActionShowAll, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pActionShowPart = new QAction("显示最新数据");
    connect(m_pActionShowPart, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pActionPause = new QAction("曲线暂停更新");
    connect(m_pActionPause, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pActionContinue = new QAction("曲线恢复更新");
    connect(m_pActionContinue, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pTimeMode = new QAction("X轴类型:时间");
    connect(m_pTimeMode, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pPointNumMode = new QAction("X轴类型:点数");
    connect(m_pPointNumMode, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);

    m_pActionClear = new QAction("清除曲线");
    connect(m_pActionClear, &QAction::triggered, this, &MultiCurvesPlot::slotContexMenu);
    this->addAction(m_pActionShowAll);
    this->addAction(m_pActionPause);
    this->addAction(m_pTimeMode);
    this->addAction(m_pActionClear);
}

void MultiCurvesPlot::curvesDataInit()
{
    graphColorVct.resize(colorListStr.size());
    for(int i = 0; i < colorListStr.size();i++)
    {
        graphColorVct[i] = QColor(colorListStr.at(i));
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

void MultiCurvesPlot::setMyStyleSheet()
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

    this->xAxis->grid()->setPen(QPen(QColor(183, 255, 255), 1, Qt::DashLine));
    this->yAxis->grid()->setPen(QPen(QColor(183, 255, 255), 1, Qt::DashLine));
    this->xAxis->grid()->setZeroLinePen(QPen(QColor(183, 255, 255)));
    this->yAxis->grid()->setZeroLinePen(QPen(QColor(183, 255, 255)));

    this->setStyleSheet("QMenu {\
                             background-color: rgb(10, 40, 70);\
                             border: 1px solid gray;\
                         }\
                         QMenu::item{\
                             background-color: transparent;\
                             padding: 4px 8px;\
                             margin: 0px 8px;\
                             border-bottom: 1px solid gray;\
                             color: rgb(10, 210, 240);\
                         }\
                         QMenu::item:selected{\
                             background-color: #94D2EF;\
                             color: black;\
                         }");
}

void MultiCurvesPlot::paintAttributeInit()
{
    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); //可平移 可滚轮缩放
    this->setNoAntialiasingOnDrag(true);//禁用抗锯齿，以提高性能
    this->setSelectionRectMode(QCP::srmZoom);
}

void MultiCurvesPlot::showCurves(QList<uint16_t> idxList)
{
    this->clearGraphs();//先移除所有的graph
    curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射

    int graphIdx = 0;

    for(QList<uint16_t>::const_iterator it = idxList.begin(); it != idxList.end(); it++)
    {   //*it为每一个要显示的曲线编号
        uint16_t curveIdx = *it;
        if(curveIdx > curvesCnt)
        {
            continue;
        }

        this->addGraph(this->xAxis, this->yAxis);
        QCPGraph* pGraph = graph(graphIdx);
        curveIdx2graphPtr[curveIdx] = pGraph;//记录：曲线索引->graph指针的映射
        pGraph->setPen(QPen(colorListStr[curveIdx]));//线的颜色随机
        pGraph->setLineStyle(QCPGraph::lsLine);//阶梯线样式
        this->graph(graphIdx)->setName(getName[curveIdx]);
        graphIdx++;
    }

    this->replot();
}

void MultiCurvesPlot::setGraphVisible(int idx, bool visible)
{
    this->graph(idx)->setVisible(visible);
}

void MultiCurvesPlot::setDataShowMode(int mode)
{
    dataShowMode = mode;
}

void  MultiCurvesPlot::setXAxisRange(double range)
{
    xAxisShowRange =  range;//设置X轴的范围
}

void MultiCurvesPlot::setXAxisType(int type)
{
    xAxisType = type;
    if(xAxisType == 0)
    {
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
        dateTicker->setDateTimeFormat("hh:mm:ss\nyyyy-MM-dd");  //日期格式(可参考QDateTime::fromString()函数)
        this->xAxis->setTicker(dateTicker);//设置X轴为时间轴
        this->xAxis->setLabel("时间");

        for(int i=0; i<curvesCnt; i++)
        {
            if(!allCurvesData[i].valVec.isEmpty())
            {
                QCPGraph* pGraph = curveIdx2graphPtr[i];
                pGraph->data().data()->clear();
                pGraph->setData(allCurvesData[i].timeKeyVec, allCurvesData[i].valVec);
            }
        }
    }
    else if(xAxisType == 1)
    {
        QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);//点数做X轴
        fixedTicker->setScaleStrategy( QCPAxisTickerFixed::ssMultiples );
        this->xAxis->setTicker(fixedTicker);
        this->xAxis->setRange(0, xAxisShowRange);
        this->xAxis->setLabel("采样点数");

        for(int i=0; i<curvesCnt; i++)
        {
            if(!allCurvesData[i].valVec.isEmpty())
            {
                QCPGraph* pGraph = curveIdx2graphPtr[i];
                pGraph->data().data()->clear();
                pGraph->setData(allCurvesData[i].cntKeyVec, allCurvesData[i].valVec);
            }
        }
    }
    else if(xAxisType == 2)
    {
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);//log做X轴
        logTicker->setLogBase(10);
        this->xAxis->setTicker(logTicker);
        this->xAxis->setScaleType(QCPAxis::stLogarithmic);
        this->xAxis->setRange(1,100000);
    }
}

void MultiCurvesPlot::setYAxisRange(double lower, double upper)
{
    this->yAxis->setRange(lower, upper);//设置Y轴的范围
}

void MultiCurvesPlot::setYAxisType(int type)
{
    if(type == 1)
    {
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);//log做X轴
        logTicker->setLogBase(10);
        this->yAxis->setTicker(logTicker);
        this->yAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    this->replot();
}

void MultiCurvesPlot::setYAxisAuto(bool flag)
{
    yAxisAuto = flag;
}

void MultiCurvesPlot::setYAxisName(QString name)
{
    this->yAxis->setLabel(name);
}

void MultiCurvesPlot::setPause(bool enable)
{
    pauseFlag = enable;
}

void MultiCurvesPlot::setUpdateTimes(int cnt)
{
    updateTimes = cnt;
}

int MultiCurvesPlot::getUpdateTimes()
{
    return updateTimes;
}

void MultiCurvesPlot::setStartTime(QDateTime startDateTime)
{
    startTime = startDateTime.toSecsSinceEpoch();
}

void MultiCurvesPlot::setCurveColor(int idx, int color)
{
    this->graph(idx)->setPen(QPen(graphColorVct[color]));//线的颜色随机
}

void MultiCurvesPlot::addData(int idx, double time, double cnt, double value)
{
    if(idx > curvesCnt)
        return;

//    if(y > 1e10 || y < -1e10)//接收到的异常值直接不绘制
//        return;

    allCurvesData[idx].timeKeyVec.append(time);//备份数据源
    allCurvesData[idx].cntKeyVec.append(cnt);//备份数据源
    allCurvesData[idx].valVec.append(value);

    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];
        if(xAxisType == 0)
            pGraph->addData(time, value);
        else
            pGraph->addData(cnt, value);
    }
}

void MultiCurvesPlot::addData(int idx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values)
{
    if(idx > curvesCnt)
        return;

    allCurvesData[idx].timeKeyVec.append(times);//备份数据源
    allCurvesData[idx].cntKeyVec.append(cnts);//备份数据源
    allCurvesData[idx].valVec.append(values);

    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];

        if(xAxisType == 0)
        {
            pGraph->addData(times, values);
        }
        else
        {
            pGraph->addData(cnts, values);
        }
    }
}

void MultiCurvesPlot::setData(const int idx, const QVector<double> &times, const QVector<double> &cnts, const QVector<double> &values)
{
    if(idx > curvesCnt)
        return;

    allCurvesData[idx].timeKeyVec.clear();
    allCurvesData[idx].cntKeyVec.clear();
    allCurvesData[idx].valVec.clear();

    allCurvesData[idx].timeKeyVec.append(times);//备份数据源
    allCurvesData[idx].cntKeyVec.append(cnts);//备份数据源
    allCurvesData[idx].valVec.append(values);

    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];
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

    updateTimes = values.size();
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

    m_listZoomLevel.clear();

    updateTimes = 0;

    updateMyPlot();
}

void MultiCurvesPlot::clearIndexData(const int idx)
{
    graph(idx)->data().data()->clear();
    allCurvesData[idx].timeKeyVec.clear();
    allCurvesData[idx].cntKeyVec.clear();
    allCurvesData[idx].valVec.clear();
    updateMyPlot();
}

void MultiCurvesPlot::updateMyPlot()
{
    if(!pauseFlag)
    {
        switch (dataShowMode)
        {
            case DATASHOWMODE::FixedWindowMode:
            {
                if(xAxisType == 0)
                {
                    if(updateTimes > xAxisShowRange)
                    {
                        double curSeclf = (double)(QDateTime::currentSecsSinceEpoch());
                        this->xAxis->setRange(curSeclf - xAxisShowRange, curSeclf);
                    }
                    else
                    {
                        this->xAxis->setRange(startTime, startTime + xAxisShowRange);
                    }
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
                    this->xAxis->setRange(1, 100000);
                }

                if(yAxisAuto)
                {
//                    graph(0)->rescaleValueAxis(false, true);
                    this->yAxis->rescale(true);
                }
                break;
            }
            case DATASHOWMODE::PushMode:
                this->xAxis->rescale(true);
                if(yAxisAuto)
                    this->yAxis->rescale(true);
                break;
            default:
                break;
        }
    }

    this->replot();
}

void MultiCurvesPlot::restoreBeforePlot()
{
    if(pauseFlag)
    {
        if(m_listZoomLevel.size() > 1)
        {
            if(this->xAxis->visible())
            {
                this->xAxis->setRange(m_listZoomLevel.at(1).dXLower, m_listZoomLevel.at(1).dXUpper);
            }
            if(this->yAxis->visible())
            {
                this->yAxis->setRange(m_listZoomLevel.at(1).dYLower, m_listZoomLevel.at(1).dYUpper);
            }
//            qDebug()<<__FUNCTION__<<m_listZoomLevel.at(1).dXLower<<m_listZoomLevel.at(1).dXUpper<<m_listZoomLevel.at(1).dYLower<<m_listZoomLevel.at(1).dYUpper;
            m_listZoomLevel.removeAt(0);
        }
        this->replot();
    }
}

void MultiCurvesPlot::slotRect()
{
    slotRectFlag = true;
    if(pauseFlag == false)
        m_pActionPause->trigger();
}

void MultiCurvesPlot::slotContexMenu()
{
    QAction *pAction = dynamic_cast<QAction*>(sender());

    if(pAction == m_pActionRestoreBefore)
    {
        if(m_listZoomLevel.size() < 2)
            m_pActionContinue->trigger();
        else
            restoreBeforePlot();
    }
    else if(pAction == m_pActionShowAll)
    {
        QAction *lastAction = this->actions().at(1);
        this->removeAction(pAction);
        this->insertAction(lastAction, m_pActionShowPart);
        setDataShowMode(1);
    }
    else if(pAction == m_pActionShowPart)
    {
        QAction *lastAction = this->actions().at(1);
        this->removeAction(pAction);
        this->insertAction(lastAction, m_pActionShowAll);
        setDataShowMode(0);
    }
    else if(pAction == m_pActionPause)
    {
        QAction *lastAction = this->actions().at(2);
        this->removeAction(pAction);
        this->insertAction(lastAction, m_pActionContinue);
        emit updatePauseStaSignal(true);
        setPause(true);
    }
    else if(pAction == m_pActionContinue)
    {
        m_listZoomLevel.clear();
        this->removeAction(pAction);
        if(this->actions().contains(m_pActionRestoreBefore))
            this->removeAction(m_pActionRestoreBefore);
        QAction *lastAction = this->actions().at(1);
        this->insertAction(lastAction, m_pActionPause);
        emit updatePauseStaSignal(false);
        setPause(false);
    }
    else if(pAction == m_pTimeMode)
    {
        this->removeAction(pAction);
        this->insertAction(m_pActionClear, m_pPointNumMode);
        setXAxisType(0);
    }
    else if(pAction == m_pPointNumMode)
    {
        this->removeAction(pAction);
        this->insertAction(m_pActionClear, m_pTimeMode);
        setXAxisType(1);
    }
    else if(pAction == m_pActionClear)
    {
        clearAllData();
    }
    updateMyPlot();
}

void MultiCurvesPlot::legendInit()
{
    // 设置图例可见
    this->legend->setVisible(true);
    // 设置图例颜色
    this->legend->setBrush(QBrush(QColor(0, 32, 96)));
    this->legend->setTextColor(QColor(102, 255, 255));
    // 设置图例位置，这里选择显示在QCPAxisRect下方，同理可设置显示在QCustomPlot中任意位置
//    this->plotLayout()->addElement(0 , 1, this->legend);
    // 设置显示比例
//    this->plotLayout()->setColumnStretchFactor(1, 0.001);
    // 设置边框隐藏
    this->legend->setBorderPen(Qt::NoPen);
}
