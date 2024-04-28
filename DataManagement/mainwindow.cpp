#include "mainwindow.h"
#include "ui_mainwindow.h"
//#define MYSQL

#define DATETIMEFORMAT  "yyyy-MM-dd hh:mm:ss"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainIint();
    connectSignalSlot();

    setWindowTitle("光纤时间同步设备 V1.0.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainIint()
{
    memberValueInit();
    uiInit();
}

void MainWindow::connectSignalSlot()
{
    connect(m_pTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::headerViewOnClicked);
}

void MainWindow::memberValueInit()
{
    m_screenWidth = QApplication::desktop()->width();
    m_screenHeight = QApplication::desktop()->height();
    m_databaseWidgetWidth = 616;
    m_analyzeWidgetWidth = 980;
    m_width = 1600;
    m_height = 900;

    m_pDataBase = new QSqlDatabase();    
    dataBaseInit();
    m_pModel = new DataBaseTableModel(*m_pDataBase);
    dataBaseTableInit();
    plotInit();
    calDataTableInit();

    m_selectDataTableName = "";
    m_selectDeviceInfoTableName = "";
    m_selectDeviceId = 0;
    m_selectLinkId = 0;
    m_selectDeviceType = 0;
    m_selectDeviceIp = "";

    m_selectDeviceDataFlag = false;

    m_startPoint = -1;
    m_stopPoint = -1;

    m_tdevKeys = {1.0, 2.0, 4.0, 10.0, 20.0, 40.0, 100.0, 200.0, 400.0,
                  1000.0, 2000.0, 4000.0, 10000.0, 100000.0};
}

void MainWindow::uiInit()
{
    styleSheetInit();
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    ui->pushButton_showInfo->hide();
    ui->pushButton_read->hide();
    ui->pushButton_downLoad->hide();
    ui->pushButton_analyzeWidgetShowHide->hide();

    ui->line->hide();
    ui->widget_plotSet->hide();
    ui->line_2->hide();
    ui->widget_function->hide();

    this->setFixedSize(m_databaseWidgetWidth, m_height);
    this->move((m_screenWidth - m_databaseWidgetWidth) / 2,
               (m_screenHeight - m_height) / 2);
}

void MainWindow::styleSheetInit()
{
    QFile file(":/qss/database.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        qssByteArray = file.readAll();
        updateStyleSheet();
    }
}

void MainWindow::updateStyleSheet()
{
    this->setStyleSheet(qssByteArray);
}

/**
 *   @brief     数据库初始化
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::dataBaseInit()
{
#ifdef MYSQL
    *m_pDataBase = QSqlDatabase::addDatabase("QMYSQL");
    m_pDataBase->setHostName("127.0.0.1");
    m_pDataBase->setPort(3307);
    m_pDataBase->setUserName("root");
    m_pDataBase->setPassword("123456");
    m_pDataBase->setDatabaseName("ft6500");
#else
    *m_pDataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_pDataBase->setDatabaseName(m_databaseName);
#endif
    if (!m_pDataBase->open())
    {
        updateLogInfo(QString("数据库打开失败！\n%1").arg(m_pDataBase->lastError().text()));
        return;
    }
    else
    {
        updateLogInfo("数据库打开成功！");
    }
}

/**
 *   @brief     数据库查询显示表格初始化
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::dataBaseTableInit()
{
    //设置显示条数
    m_pModel->setLimitCount(8);
    //设置表格属性
    m_pTableView = new QTableView;
    //设置模型
    m_pTableView->setModel(m_pModel);
    QGridLayout *layout_tableView = new QGridLayout;
    layout_tableView->addWidget(m_pTableView);
    ui->widget_dataTable->setLayout(layout_tableView);

    m_pTableView->verticalHeader()->hide();//隐藏垂直索引
    m_pTableView->setEditTriggers( QAbstractItemView::AllEditTriggers);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//设置列宽自适应
//    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setStretchLastSection(true); //设置最后一列充满表宽度
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);//设置选择模式
    m_pTableView->setAlternatingRowColors(true);
    m_pTableView->horizontalHeader()->setFixedHeight(25); //设置表头的高度
    m_pTableView->setStyleSheet("selection-"); //设置选中背景色
    m_pTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
}

/**
 *   @brief     曲线图初始化
 *   @author    hongweiYang
 *   @date      2022-09-06
 */
void MainWindow::plotInit()
{
    m_oriDataPlot = new MultiCurvesPlot(1);
    m_oriDataPlot->setXAxisType(1);
    m_oriDataPlot->setYAxis(-1.5, 1.5);
    m_oriDataPlot->setDataShowMode(1);    
    m_oriDataPlot->xAxis->setLabel("采样点数");
    m_oriDataPlot->yAxis->setLabel("时差/ps");
    QGridLayout *layout_oriDataPlot = new QGridLayout;
    layout_oriDataPlot->setContentsMargins(0, 0, 0, 0);
    layout_oriDataPlot->addWidget(m_oriDataPlot, 0, Qt::AlignCenter);
    ui->widget_readDataPlot->setLayout(layout_oriDataPlot);

    m_calDataPlot = new MultiCurvesPlot(1);
    m_calDataPlot->setXAxisType(2);
    m_calDataPlot->setYAxisType(1);
    m_calDataPlot->setYAxis(-1.5, 1.5);
    m_calDataPlot->xAxis->setLabel("时间间隔");
    m_calDataPlot->yAxis->setLabel("TDEV");
    m_calDataPlot->xAxis->grid()->setSubGridVisible(true);
    m_calDataPlot->yAxis->grid()->setSubGridVisible(true);
    QGridLayout *layout_calDataPlot = new QGridLayout;
    layout_calDataPlot->setContentsMargins(0, 0, 0, 0);
    layout_calDataPlot->addWidget(m_calDataPlot, 0, Qt::AlignCenter);
    ui->widget_calDataPlot->setLayout(layout_calDataPlot);

    QList<uint16_t> plot_list;
    plot_list.append(0);
    m_oriDataPlot->showCurves(plot_list);
    m_calDataPlot->showCurves(plot_list);
}

/**
 *   @brief     计算数据结果表格初始化
 *   @author    hongweiYang
 *   @date      2022-09-06
 */
void MainWindow::calDataTableInit()
{
    m_calDataTableModel = new CalDataTableModel();
    m_calDataTableView = new QTableView;
    m_calDataTableView->setModel(m_calDataTableModel);
    QGridLayout *layout_m_calDataTableView = new QGridLayout;
    layout_m_calDataTableView->setContentsMargins(0, 0, 0, 0);
    layout_m_calDataTableView->addWidget(m_calDataTableView);
    ui->widget_calDataTable->setLayout(layout_m_calDataTableView);

    m_calDataTableView->verticalHeader()->hide();
    m_calDataTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    m_calDataTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_calDataTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_calDataTableView->setColumnWidth(0, 70);
    m_calDataTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_calDataTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{color:rgb(102,255,255); background:rgb(82,82,82); height:30px}");
}

/**
 *   @brief     数据管理页面查询按钮槽函数
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::on_pushButton_query_clicked()
{
    QString startTimeStr = ui->dateTimeEdit_start->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString endTimeStr = ui->dateTimeEdit_end->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(ui->dateTimeEdit_start->dateTime().toTime_t() > ui->dateTimeEdit_end->dateTime().toTime_t())
    {
        QMessageBox::warning(this, "警告", "查询时间范围错误！");
        return;
    }
    QString cmdStr = QString("SELECT * FROM InformationSummaryTable "
                             "WHERE TestTime >= \"%1\" AND TestTime <= \"%2\"")
                            .arg(startTimeStr).arg(endTimeStr);

    m_pModel->setSql(cmdStr);
    m_selectDeviceDataFlag = false;
    updateLogInfo(QString("查询%1").arg(m_pModel->queryData()));
    ui->pushButton_showInfo->show();
    ui->pushButton_read->hide();
    ui->pushButton_downLoad->hide();
    ui->pushButton_analyzeWidgetShowHide->hide();
    on_pushButton_firstPage_clicked();
}

/**
 *   @brief     查看本次测试站点信息
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::on_pushButton_showInfo_clicked()
{
    int row = m_pTableView->currentIndex().row();
    if(row < 0)
    {
        QMessageBox::warning(this, "错误！", "未选择查询对象！请用鼠标点击选中查询对象！");
        return;
    }
    else
    {
        QAbstractItemModel *model = m_pTableView->model();
        QModelIndex index;
        index = model->index(row, 2);//导出数据文件名称
        m_selectDeviceInfoTableName = model->data(index).toString();
    }

    QString cmdStr = QString("SELECT DeviceId, DeviceType, DeviceIp, DataTableName FROM %1")
                            .arg(m_selectDeviceInfoTableName);
    m_selectDeviceDataStr = cmdStr;
    m_selectDeviceDataFlag = true;

    m_pModel->setSql(cmdStr);
    updateLogInfo(QString("查看设备信息%1").arg(m_pModel->queryData()));
    ui->pushButton_showInfo->hide();
    ui->pushButton_read->show();
    ui->pushButton_downLoad->show();
    ui->pushButton_analyzeWidgetShowHide->hide();
    on_pushButton_firstPage_clicked();
}

/**
 *   @brief     读取数据
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::on_pushButton_read_clicked()
{
    int row = m_pTableView->currentIndex().row();
    if(row < 0)
    {
        QMessageBox::warning(this, "错误！", "未选择查询对象！请用鼠标点击选中查询对象！");
        return;
    }
    else
    {
        QAbstractItemModel *model = m_pTableView->model();
        QModelIndex index;
        index = model->index(row, 0);   //选中行设备ID
        m_selectDeviceId = model->data(index).toUInt();
        index = model->index(row, 1);   //选中行设备类型
        m_selectDeviceType = model->data(index).toUInt();
        index = model->index(row, 2);   //选中行设备IP
        m_selectDeviceIp = model->data(index).toString();
        index = model->index(row, 3);   //选中行设备数据表名称
        m_selectDataTableName = model->data(index).toString();
    }

    m_dateTimeStr.clear();
    m_dateTimes.clear();
    m_dataKeys.clear();
    m_timeDiffs.clear();

    QString cmdStr;
//    cmdStr = QString("SELECT COUNT(*) FROM %1").arg(m_selectDataTableName);
//    m_pModel->setSql(cmdStr);
//    int dataLen = 0;
//    if(m_pModel->getDataLen(dataLen))
//    {
//        m_dateTimes.resize(dataLen);
//        m_dataKeys.resize(dataLen);
//        m_timeDiffs.resize(dataLen);
//    }

    cmdStr = QString("SELECT * FROM %1").arg(m_selectDataTableName);
    m_pModel->setSql(cmdStr);
    if(m_pModel->selectData(m_dateTimeStr, m_dateTimes, m_dataKeys, m_timeDiffs))
    {
        m_dataStartTime = m_dateTimeStr.first();
        m_dataEndTime = m_dateTimeStr.last();
        ui->dateTimeEdit_plotStart->setDateTime(QDateTime::fromString(m_dataStartTime, Qt::ISODate));
        ui->dateTimeEdit_plotEnd->setDateTime(QDateTime::fromString(m_dataEndTime, Qt::ISODate));
        ui->spinBox_stopPoint->setValue(m_timeDiffs.size());
        m_startPoint = 0;
        m_stopPoint = m_timeDiffs.size()-1;
        updateLogInfo(QString("数据表%1 数据读取成功！").arg(m_selectDataTableName));
        initDataPlotTable();
        updateReadDataPlot();
    }
    else
    {
        updateLogInfo(QString("数据表%1 数据读取失败！").arg(m_selectDataTableName));
    }
}

/**
 *   @brief     导出数据
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::on_pushButton_downLoad_clicked()
{
    QString dstFilePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+ "/" + "DownLoadData";
    QDir dstDir;
    if(!dstDir.exists(dstFilePath))
    {
        dstDir.mkpath(dstFilePath);
    }

    int row = m_pTableView->currentIndex().row();
    if(row < 0)
    {
        QMessageBox::warning(this, "错误！", "未选择查询对象！请用鼠标点击选中查询对象！");
        return;
    }
    else
    {
        QAbstractItemModel *model = m_pTableView->model();
        QModelIndex index;
        index = model->index(row, 0);   //选中行设备ID
        m_selectDeviceId = model->data(index).toUInt();
        index = model->index(row, 1);   //选中行设备类型
        m_selectDeviceType = model->data(index).toUInt();
        index = model->index(row, 2);   //选中行设备IP
        m_selectDeviceIp = model->data(index).toString();
        index = model->index(row, 3);   //选中行设备数据表名称
        m_selectDataTableName = model->data(index).toString();
    }

    m_dateTimeStr.clear();
    m_dateTimes.clear();
    m_dataKeys.clear();
    m_timeDiffs.clear();

    QString cmdStr;
//    cmdStr = QString("SELECT COUNT(*) FROM %1").arg(m_selectDataTableName);
//    m_pModel->setSql(cmdStr);
//    int dataLen = 0;
//    if(m_pModel->getDataLen(dataLen))
//    {
//        m_dateTimes.resize(dataLen);
//        m_dataKeys.resize(dataLen);
//        m_timeDiffs.resize(dataLen);
//    }

    cmdStr = QString("SELECT * FROM %1").arg(m_selectDataTableName);
    m_pModel->setSql(cmdStr);
    if(m_pModel->selectData(m_dateTimeStr, m_dateTimes, m_dataKeys, m_timeDiffs))
    {
        updateLogInfo(QString("数据表%1 数据读取成功！").arg(m_selectDataTableName));

        QString downDataFileName = QString("%1/Link(%2)_DeviceType(%3)_DeviceId(%4)_IP(%5)_Time(%6).txt")
                                          .arg(dstFilePath)
                                          .arg(m_selectLinkId).arg(m_selectDeviceType).arg(m_selectDeviceId)
                                          .arg(m_selectDeviceIp).arg(m_selectDataTableName.mid(m_selectDataTableName.size()-14, 14));
        QFile downFile(downDataFileName);
        if(!downFile.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug()<<downFile.errorString();
            QMessageBox::information(this, "警告", "数据导出文件打开失败");
            return;
        }
        QTextStream out(&downFile);
        for(int i=0; i<m_timeDiffs.length(); i++)
        {
            out << m_dataKeys.at(i) << "\t" << m_dateTimeStr.at(i) << "\t" << m_timeDiffs.at(i) << "\n";
        }
        downFile.close();
        updateLogInfo("数据导出成功");
    }
    else
    {
        updateLogInfo(QString("数据表%1 数据读取失败！").arg(m_selectDataTableName));
    }
}

/**
 *   @brief     更新读取数据曲线图
 *   @author    hongweiYang
 *   @date      2022-09-06
 */
void MainWindow::updateReadDataPlot()
{
    m_tdevFirstFlag = true;
    m_tdevValues.clear();
    m_calDataPlot->clearAllData();
    m_oriDataPlot->clearAllData();

    m_oriDataPlot->setData(0, m_dateTimes.mid(m_startPoint, m_stopPoint-m_startPoint+1),
                           m_dataKeys.mid(m_startPoint, m_stopPoint-m_startPoint+1),
                           m_timeDiffs.mid(m_startPoint, m_stopPoint-m_startPoint+1));
}

/**
 *   @brief     重新读取数据后，计算结果曲线、数据表格等初始化
 *   @author    hongweiYang
 *   @date      2022-09-06
 */
void MainWindow::initDataPlotTable()
{
    ui->pushButton_downLoad->show();
    ui->pushButton_analyzeWidgetShowHide->setText("隐藏");
    ui->pushButton_analyzeWidgetShowHide->show();

    ui->line->show();
    ui->widget_plotSet->show();
    ui->line_2->show();
    ui->widget_function->show();
    this->setFixedSize(m_width, m_height);
    this->move((m_screenWidth - m_width) / 2, (m_screenHeight - m_height) / 2);
}

void MainWindow::on_pushButton_firstPage_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    m_pModel->currentPageEdit(1);
    updateDataBaseTableStatus();
}

void MainWindow::on_pushButton_pageUp_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    m_pModel->currentPageEdit(m_pModel->getCurrentPage() - 1);
    updateDataBaseTableStatus();
}

void MainWindow::on_pushButton_pageDown_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    m_pModel->currentPageEdit(m_pModel->getCurrentPage() + 1);
    updateDataBaseTableStatus();
}

void MainWindow::on_pushButton_endPage_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    m_pModel->currentPageEdit(m_pModel->getTotalNumPage());
    updateDataBaseTableStatus();
}

void MainWindow::on_pushButton_toThePage_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    //得到输入字符串
    QString szText = ui->lineEdit_toThePageNum->text();
    //数字正则表达式
    QRegExp regExp("-?[0-9]*");
    //判断是否为数字
    if(!regExp.exactMatch(szText))
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入数字"));
        return;
    }
    //是否为空
    if(szText.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入跳转索引"));
        return;
    }
    //得到页数
    int pageIndex = szText.toInt();
    //判断是否有指定页
    if(pageIndex > m_pModel->getTotalNumPage() || pageIndex < 1)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("页面超出范围，请重新输入"));
        return;
    }
    //记录查询
    currentPageEdit(QString::number(pageIndex));
    //刷新状态
    updateDataBaseTableStatus();
    return;
}

void MainWindow::on_pushButton_rowCountsSet_clicked()
{
    if(m_selectDeviceDataFlag)
        m_pModel->setSql(m_selectDeviceDataStr);
    //得到输入字符串
    QString szText = ui->lineEdit_everyPageRowCounts->text();

    //数字正则表达式
    QRegExp regExp("-?[0-9]*");
    //判断是否为数字
    if(!regExp.exactMatch(szText))
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入数字"));
        return;
    }
    //是否为空
    if(szText.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入设置的行数"));
        return;
    }
    //得到行数
    int Index = szText.toInt();

    //设置每页容量
    m_pModel->setLimitCount(Index);
    m_pModel->queryData();
    //刷新状态
    updateDataBaseTableStatus();
    return;
}

/**
 *   @brief     更新页码
 *   @author    hongweiYang
 *   @date      2022-09-05
 */
void MainWindow::updateDataBaseTableStatus()
{
    //刷新表格
    m_pTableView->reset();
    //总页数
    QString szPageCountText = QString(QStringLiteral("总共 %1 页")).arg(m_pModel->getTotalNumPage());
    ui->label_totalPage->setText(szPageCountText);
    //设置当前页文本
    int iCurPage = m_pModel->getCurrentPage();
    QString szCurrentText = QString(QStringLiteral("当前第 %1 页")).arg(iCurPage);
    ui->label_currentPage->setText(szCurrentText);
    //每页显示行数
    QString strPerPageCount = QString(QStringLiteral("%1")).arg(m_pModel->getRowsPerPage());
    ui->lineEdit_everyPageRowCounts->setText(strPerPageCount);
    //当前第一页，且总共只有一页
    if (1 == iCurPage && 1 == m_pModel->getTotalNumPage())
    {
        ui->pushButton_firstPage->setEnabled(false);
        ui->pushButton_endPage->setEnabled(false);
        ui->pushButton_pageUp->setEnabled(false);
        ui->pushButton_pageDown->setEnabled(false);
    }
    //当前第一页，且总页数大于1页
    else if(1 == iCurPage && m_pModel->getTotalNumPage() > 1)
    {
        ui->pushButton_firstPage->setEnabled(false);
        ui->pushButton_endPage->setEnabled(true);
        ui->pushButton_pageUp->setEnabled(false);
        ui->pushButton_pageDown->setEnabled(true);
    }
    //当前是最后一页
    else if(iCurPage == m_pModel->getTotalNumPage())
    {
        ui->pushButton_firstPage->setEnabled(true);
        ui->pushButton_endPage->setEnabled(false);
        ui->pushButton_pageUp->setEnabled(true);
        ui->pushButton_pageDown->setEnabled(false);
    }
    //中间页
    else
    {
        ui->pushButton_firstPage->setEnabled(true);
        ui->pushButton_endPage->setEnabled(true);
        ui->pushButton_pageUp->setEnabled(true);
        ui->pushButton_pageDown->setEnabled(true);
    }
    return;
}

void MainWindow::currentPageEdit(QString pageStr)
{
    int page = 0;

    if(pageStr.isNull() || pageStr.isEmpty())
    {
        return;
    }

    bool isOK;

    page = pageStr.toInt(&isOK);

    if(isOK)
    {
        if(page > m_pModel->getTotalNumPage() || page <= 0)
        {
            QMessageBox::warning(this,"警告","您输入的数字超出范围，请重新输入");
            return;
        }
        m_pModel->currentPageEdit(page);
        m_pModel->getCurrentPage();
    }
}

void MainWindow::headerViewOnClicked(int colIndex)
{
    //获取列名
    QString headerStr = m_pModel->headerData(colIndex,Qt::Horizontal).toString();
    //进行切割，如果排过序的话 列名是不正确的这个操作是为了获取正确的列名
    QStringList headers = headerStr.split(" ",QString::SkipEmptyParts);
    //进行排序
    int enumValue = m_pModel->setOrderBy(headers[0]);
    //对列名进行拼接，升序加个上箭头降序加个下箭头不排序就是原列名
    QString temp = " ";

    switch (enumValue) {
    case 1:
        temp.append("↑");
        break;
    case 2:
        temp.append("↓");
        break;
    default:
        break;
    }

    //设置列名
    m_pModel->setHeaderData(colIndex, Qt::Horizontal, QString("%1%2").arg(headers[0]).arg(temp));

    //查询数据，并提示需要重新拼接sql
    m_pModel->queryData();
}

void MainWindow::updateLogInfo(QString info)
{
    ui->textBrowser_log->append(QString("%1: %2")
                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:hh:ss"))
                                .arg(info));
}

void MainWindow::on_pushButton_databaseShowHide_clicked()
{
    if(ui->pushButton_databaseShowHide->text() == "隐藏")
    {
        ui->line_2->hide();
        ui->widget_databaseWidget->hide();
        ui->pushButton_databaseShowHide->setText("显示");
        this->setFixedSize(m_analyzeWidgetWidth, m_height);
        this->move((m_screenWidth - m_analyzeWidgetWidth) / 2,
                   (m_screenHeight - m_height) / 2);
    }
    else
    {
        ui->line_2->show();
        ui->widget_databaseWidget->show();
        ui->pushButton_databaseShowHide->setText("隐藏");
        this->setFixedSize(m_width, m_height);
        this->move((m_screenWidth - m_width) / 2,
                   (m_screenHeight - m_height) / 2);
    }
}

void MainWindow::on_pushButton_tdev_clicked()
{
    QStringList headList;
    headList << QString("时间间隔") << QString("TDEV");
    m_calDataTableModel->setHeadDataList(headList);

    m_calDataPlot->clearAllData();

    QVector<double> selectTimeDiffVct(m_stopPoint-m_startPoint+1);
    selectTimeDiffVct = m_timeDiffs.mid(m_startPoint, m_stopPoint-m_startPoint+1);

    if(m_tdevFirstFlag)
    {
        int N = selectTimeDiffVct.size();
        for(int i=0; i<14; i++)
        {
            int n = m_tdevKeys[i];
            if(n >= (int)(N/3))
                continue;

            double tdev_value = 0;

            for(int j=0; j<(N-3*n+1); j++)
            {
                double tmp_value = 0;
                for(int i=j; i<=n+j-1; i++)
                {
                    double tmp = selectTimeDiffVct.at(i+2*n)/1000000000000.0 - 2*selectTimeDiffVct.at(i+n)/1000000000000.0 + selectTimeDiffVct.at(i)/1000000000000.0;
                    tmp_value += tmp;
                }
                tdev_value += qPow(tmp_value, 2);
            }
            tdev_value = qSqrt(tdev_value/(6.0*n*n*(N-3.0*n+1)));
            m_tdevValues.append(tdev_value);
        }
        m_tdevFirstFlag = false;
    }
    updateCalDataPlot(TDEV);
    updateCalDataTable(TDEV);
}

void MainWindow::updateCalDataPlot(int type)
{
    if(type == TDEV)
    {
        QVector<double> times;
        m_calDataPlot->setData(0, times, m_tdevKeys.mid(0, m_tdevValues.size()), m_tdevValues);
    }
}

void MainWindow::updateCalDataTable(int type)
{
    if(type == TDEV)
    {
        for(int i=0; i<m_tdevValues.size(); i++)
        {
            m_calDataTableModel->setData(m_calDataTableModel->index(i, 1), QString::number(m_tdevValues.at(i)));
        }
    }
}

void MainWindow::on_pushButton_analyzeWidgetShowHide_clicked()
{
    if(ui->pushButton_analyzeWidgetShowHide->text() == "隐藏")
    {       
        ui->line->hide();
        ui->widget_plotSet->hide();
        ui->line_2->hide();
        ui->widget_function->hide();
        ui->pushButton_analyzeWidgetShowHide->setText("显示");
        this->setFixedSize(m_databaseWidgetWidth, m_height);
        this->move((m_screenWidth - m_databaseWidgetWidth) / 2,
                   (m_screenHeight - m_height) / 2);
    }
    else
    {       
        ui->line->show();
        ui->widget_plotSet->show();
        ui->line_2->show();
        ui->widget_function->show();
        ui->pushButton_analyzeWidgetShowHide->setText("隐藏");
        this->setFixedSize(m_width, m_height);
        this->move((m_screenWidth - m_width) / 2,
                   (m_screenHeight - m_height) / 2);
    }
}

void MainWindow::on_pushButton_showAll_clicked()
{
    m_oriDataPlot->setPause(false);
}

void MainWindow::on_pushButton_clear_clicked()
{
    m_oriDataPlot->clearAllData();
}

/**
 *   @brief     X轴类型切换
 *   @author    hongweiYang
 *   @date      2022-11-05
 */
void MainWindow::on_pushButton_xAxisType_clicked()
{
    if(ui->pushButton_xAxisType->text() == "时间")
    {
        ui->pushButton_xAxisType->setText("点数");
        m_oriDataPlot->setXAxisType(0);
    }
    else
    {
        ui->pushButton_xAxisType->setText("时间");
        m_oriDataPlot->setXAxisType(1);
    }
}

void MainWindow::on_pushButton_plot_clicked()
{
    int start = 0;
    int end = 0;

    if(ui->checkBox_time->isChecked())
    {
        if(ui->dateTimeEdit_plotStart->dateTime() < QDateTime::fromString(m_dataStartTime, Qt::ISODate) ||
           ui->dateTimeEdit_plotEnd->dateTime() > QDateTime::fromString(m_dataEndTime, Qt::ISODate) ||
           ui->dateTimeEdit_plotStart->dateTime() > ui->dateTimeEdit_plotEnd->dateTime())
        {
            QMessageBox::warning(nullptr, "错误!", "时间范围选取异常,请重新选择!");
            return;
        }

        bool startFlag = false;
        QDateTime startDateTime = ui->dateTimeEdit_plotStart->dateTime();
        QDateTime endDateTime = ui->dateTimeEdit_plotEnd->dateTime();

        for(int i=0; i<m_timeDiffs.size(); i++)
        {
            QDateTime curDateTime = QDateTime::fromString(m_dateTimeStr.at(i), DATETIMEFORMAT);
            if((curDateTime >= startDateTime) && !startFlag)
            {
                start = i;
                startFlag = true;
            }

            if(curDateTime >= endDateTime)
            {
                end = i;
                break;
            }
        }
    }
    else if(ui->checkBox_point->isChecked())
    {
        if(ui->spinBox_startPoint->value() > ui->spinBox_stopPoint->value() ||
           ui->spinBox_stopPoint->value() > m_timeDiffs.size())
        {
            QMessageBox::warning(nullptr, "错误!", "点数范围选取异常,请重新选择!");
            return;
        }
        start = ui->spinBox_startPoint->value()-1;
        end = ui->spinBox_stopPoint->value()-1;
    }

    m_startPoint = start;
    m_stopPoint = end;

    updateReadDataPlot();
}

void MainWindow::on_pushButton_resetTimeRange_clicked()
{
    m_startPoint = 0;
    m_stopPoint = m_timeDiffs.size()-1;
    updateReadDataPlot();
}
