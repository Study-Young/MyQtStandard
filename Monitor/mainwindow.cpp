#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/IconHelper/iconhelper.h"
#include "Model/messagetips.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainInit(); 
}

MainWindow::~MainWindow()
{
    delete ui;

    LogManagement::deleteInstance();
    ConfigManagement::deleteInstance();
    MyDataBase::deleteInstance();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
/*
    bool bOk = false;
    QString sName = QInputDialog::getText(this,
                                         "退出",
                                         "请输入密码",
                                         QLineEdit::Password,
                                         "",
                                         &bOk);

    if (!bOk)
    {
        event->ignore();
        return;
    }

    if (sName.isEmpty() || ((sName != "admin") && (sName != "zjsaisi")))
    {
        QMessageBox::information(this, "提示!", "验证失败!");
        event->ignore();
        return;
    }
*/
    switch(QMessageBox::information(NULL, "提示", "确认关闭软件？",
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::No | QMessageBox::Escape))
    {
        case QMessageBox::Yes:
        {
            event->accept();
            break;
        }
        case QMessageBox::No:
        {
            event->ignore();
            break;
        }
    }
}

void MainWindow::updateCurrentUserInfo()
{
    updateMsg(__FILE__, __FUNCTION__, MESSAGE_OPERATOR,
              QString("当前用户:%1 用户身份:%2").arg(g_userName).arg(g_userLevel == 2 ? "研发" : (g_userLevel == 1 ? "管理" : "测试")));
}

void MainWindow::mainInit()
{
    memberValueInit();
    uiInit();
}

void MainWindow::memberValueInit()
{
    signalSlotInit();
    configFileInit();
    workLogFileInit();
    plotInit();
    tableInit();
    pushButtonInit();

    catalogWidgetInit();
    iconWidgetInit();

    databaseInit();
    styleSheetInit();
}

void MainWindow::uiInit()
{
    ui->label_dbSta->setScaledContents(true);

    ui->widget_switchButton3->setType(1);
    ui->widget_switchButton4->setType(2);
    ui->widget_switchButton5->setType(3);
}

void MainWindow::signalSlotInit()
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaType<QList<int>>("QList<int>");
    qRegisterMetaType<QList<quint8>>("QList<quint8>");
    qRegisterMetaType<QList<double>>("QList<double>");
    qRegisterMetaType<QVector<double>>("QVector<double>");
    qRegisterMetaType<QList<float>>("QList<float>");
}

void MainWindow::configFileInit()
{
    m_configManagement = ConfigManagement::getInstance();
    g_saveWorkLogFlag = m_configManagement->getSaveWorkLogFlag();
}

void MainWindow::workLogFileInit()
{
    m_logManagement = LogManagement::getInstance();
    qRegisterMetaType<EN_MessageType>("EN_MessageType");
    connect(this, &MainWindow::recordEventSignal, m_logManagement, &LogManagement::recordEventSlot);
    connect(this, &MainWindow::recordErrorSignal, m_logManagement, &LogManagement::recordErrorSlot);
    connect(this, &MainWindow::recordOperatorSignal, m_logManagement, &LogManagement::recordOperatorSlot);
}

void MainWindow::plotInit()
{
    m_commonDataPlot = new MultiCurvesPlot(2);
    m_commonDataPlot->setXAxisType(1);
    m_commonDataPlot->xAxis->setLabel("点数");
    m_commonDataPlot->yAxis->setLabel("相差");
    QGridLayout *layout_commonPlot = new QGridLayout();
    layout_commonPlot->setContentsMargins(0, 0, 0, 0);
    layout_commonPlot->addWidget(m_commonDataPlot, 0, Qt::AlignCenter);
    ui->widget_commonPlot->setLayout(layout_commonPlot);

    m_logDataPlot = new MultiCurvesPlot(1);
    m_logDataPlot->setCalDataPlotProperty();
    m_logDataPlot->setXAxisType(2);
    m_logDataPlot->setYAxisType(1);
    m_logDataPlot->xAxis->setLabel("时间间隔");
    m_logDataPlot->yAxis->setLabel("Allan");
    QGridLayout *layout_logPlot = new QGridLayout();
    layout_logPlot->setContentsMargins(0, 0, 0, 0);
    layout_logPlot->addWidget(m_logDataPlot, 0, Qt::AlignCenter);
    ui->widget_logPlot->setLayout(layout_logPlot);

    QList<uint16_t> list_commonPlot;
    list_commonPlot.append(0);
    list_commonPlot.append(1);
    m_commonDataPlot->showCurves(list_commonPlot);

    QList<uint16_t> list_logPlot;
    list_logPlot.append(0);
    m_logDataPlot->showCurves(list_logPlot);

    m_commonDataPlot->setCurveColor(0, 1);
    m_commonDataPlot->setCurveColor(1, 2);
    m_logDataPlot->setCurveColor(0, 2);
}

void MainWindow::tableInit()
{
    commonTableInit();
}

void MainWindow::commonTableInit()
{
    m_commonTableModel = new CommonTableModel;

    CheckBoxDelegate *checkBoxDelegate_showCurve = new CheckBoxDelegate(m_commonTableModel->columnCount()-1);
    ui->tableView_commonTable->setItemDelegateForColumn(m_commonTableModel->columnCount()-1, checkBoxDelegate_showCurve);
    ui->tableView_commonTable->setModel(m_commonTableModel);

    ui->tableView_commonTable->verticalHeader()->setVisible(false);
    ui->tableView_commonTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(0, 50);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(1, 50);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(2, 200);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(3, 80);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(6, 80);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(8, 100);
    ui->tableView_commonTable->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Fixed);
    ui->tableView_commonTable->setColumnWidth(9, 50);
    ui->tableView_commonTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView_commonTable->setFocusPolicy(Qt::NoFocus);

    connect(m_commonTableModel, &CommonTableModel::selectCurveSignal, this, &MainWindow::selectCurveSlot);
    connect(ui->tableView_commonTable->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::onHeaderDoubleClickedSlot);
}

void MainWindow::selectCurveSlot(int sta, int level, int pos)
{
    QString name = QString("%1层%2号").arg(level).arg(pos);
    QString key = QString("%1.%2").arg(level).arg(pos);

    if(sta)
    {
        if(m_curvesMap.contains(key))
            return;

        QCheckBox *cruveCheckBox = new QCheckBox(name);
        connect(cruveCheckBox, &QCheckBox::clicked, this, &MainWindow::showCurveSlot);
        cruveCheckBox->setCheckState(Qt::Unchecked);
        ui->verticalLayout_showCurve->insertWidget(m_curvesMap.size(), cruveCheckBox);
        m_curvesMap.insert(key, cruveCheckBox);
    }
    else
    {
        QCheckBox *rmCheckBox = m_curvesMap.value(key);
        if(rmCheckBox->isChecked())
            rmCheckBox->click();
        disconnect(rmCheckBox, &QCheckBox::clicked, this, &MainWindow::showCurveSlot);
        rmCheckBox->setParent(nullptr);
        ui->verticalLayout_showCurve->removeWidget(rmCheckBox);
        m_curvesMap.remove(key);
    }
    m_commonTableData = m_commonTableModel->getModelData();
}

void MainWindow::showCurveSlot(bool checked)
{
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox*>(sender());

    if(checked)
    {
        QMap<QString, QCheckBox *>::const_iterator cqmit;
        for(cqmit = m_curvesMap.begin(); cqmit != m_curvesMap.end(); cqmit++)
        {
            if(cqmit.value() != pCheckBox)
            {
                cqmit.value()->blockSignals(true);
                cqmit.value()->setChecked(false);
                cqmit.value()->blockSignals(false);
            }
        }
    }
}

void MainWindow::onHeaderDoubleClickedSlot(int index)
{
    if(index == m_commonTableModel->columnCount()-1)
    {
        m_commonTableData = m_commonTableModel->getModelData();
        for(int i=0; i<m_commonTableData.size(); i++)
        {
            m_commonTableData[i].showCurve = 1;
            QModelIndex modelIndex = m_commonTableModel->index(i, index);
            m_commonTableModel->setData(modelIndex, 1);
        }
    }
}

void MainWindow::pushButtonInit()
{
    //加入按钮组自动互斥
    m_catalogBtnGroup = new QButtonGroup(this);
    m_catalogBtnGroup->addButton(ui->toolButton_workLog, 0);
    m_catalogBtnGroup->addButton(ui->toolButton_plot, 1);
    m_catalogBtnGroup->addButton(ui->toolButton_dataTable, 2);
    m_catalogBtnGroup->addButton(ui->toolButton_catalog, 3);
    m_catalogBtnGroup->addButton(ui->toolButton_icon, 4);
    m_catalogBtnGroup->addButton(ui->toolButton_button, 5);
    m_catalogBtnGroup->addButton(ui->toolButton_delegate, 6);

    //设置按钮可选中以及图标样式
    QList<QAbstractButton *> btns = m_catalogBtnGroup->buttons();
    foreach (QAbstractButton *btn, btns) {
        QToolButton *b = (QToolButton *)btn;
        //关联按钮单击事件
        connect(b, &QToolButton::clicked, this, &MainWindow::catalogBtnClicked);
        b->setCheckable(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }

    ui->widget_lineTab3->setMinimumWidth(3);
    m_lineTab3 = new LineTab(ui->widget_lineTab3);
    m_lineTab3->setItemNum(7);
    m_lineTab3->setLineDirection(1);
    QGridLayout *pGridLayout3 = new QGridLayout();
    pGridLayout3->setContentsMargins(0,0,0,0);
    pGridLayout3->addWidget(m_lineTab3);
    ui->widget_lineTab3->setLayout(pGridLayout3);
    /*

    //设置弱属性以便应用样式
    ui->scrollAreaWidgetContents_catalog->setProperty("flag", "left");

    //图标对应图形字体值
    QList<int> icons;
    icons << 0xe6dc << 0xe59c << 0xe60a << 0xe67c << 0xe504 << 0xe702 << 0xe657;

    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    styleColor.position = "left";
    styleColor.iconSize = 18;
    styleColor.iconWidth = 30;
    styleColor.iconHeight = 25;
    styleColor.borderWidth = 4;
    styleColor.borderColor = "#32B9CF";
    styleColor.setColor("#187294", "#B6D7E3", "#145C75", "#F0F0F0");
    IconHelper::setStyle(ui->scrollAreaWidgetContents_catalog, m_catalogBtnGroup->buttons(), icons, styleColor);
    */

    //默认选中某个按钮
    ui->toolButton_workLog->click();

    ui->widget_lineTab->setMinimumHeight(3);
    m_lineTab = new LineTab(ui->widget_lineTab);
    m_lineTab->setItemNum(4);
    m_lineTab->setLineDirection(0);
    QGridLayout *pGridLayout1 = new QGridLayout();
    pGridLayout1->setContentsMargins(0,0,0,0);
    pGridLayout1->addWidget(m_lineTab);
    ui->widget_lineTab->setLayout(pGridLayout1);

    m_slideBtnGroup = new QButtonGroup(this);
    m_slideBtnGroup->addButton(ui->pushButton_1, 0);
    m_slideBtnGroup->addButton(ui->pushButton_2, 1);
    m_slideBtnGroup->addButton(ui->pushButton_3, 2);
    m_slideBtnGroup->addButton(ui->pushButton_4, 3);    
    QList<QAbstractButton *> btns2 = m_slideBtnGroup->buttons();
    foreach (QAbstractButton *btn, btns2) {
        QPushButton *b = (QPushButton *)btn;
        //关联按钮单击事件
        connect(b, &QToolButton::clicked, this, &MainWindow::slideBtnClicked);
    }
    ui->pushButton_1->click();

    ui->widget_lineTab2->setMinimumWidth(3);
    m_lineTab2 = new LineTab(ui->widget_lineTab2);
    m_lineTab2->setItemNum(4);
    m_lineTab2->setLineDirection(1);
    QGridLayout *pGridLayout2 = new QGridLayout();
    pGridLayout2->setContentsMargins(0,0,0,0);
    pGridLayout2->addWidget(m_lineTab2);
    ui->widget_lineTab2->setLayout(pGridLayout2);

    m_slideBtnGroup2 = new QButtonGroup(this);
    m_slideBtnGroup2->addButton(ui->pushButton_5, 0);
    m_slideBtnGroup2->addButton(ui->pushButton_6, 1);
    m_slideBtnGroup2->addButton(ui->pushButton_7, 2);
    m_slideBtnGroup2->addButton(ui->pushButton_8, 3);
    QList<QAbstractButton *> btns3 = m_slideBtnGroup2->buttons();
    foreach (QAbstractButton *btn, btns3) {
        QPushButton *b = (QPushButton *)btn;
        //关联按钮单击事件
        connect(b, &QToolButton::clicked, this, &MainWindow::slideBtn2Clicked);
    }
    ui->pushButton_5->click();

}

void MainWindow::catalogBtnClicked()
{
    QAbstractButton *btn = (QAbstractButton *)sender();
    ui->stackedWidget->setCurrentIndex(m_catalogBtnGroup->id(btn));
    m_lineTab3->updatePaintSlot(m_catalogBtnGroup->id(btn));
}

void MainWindow::functionBtnClicked()
{
    QPushButton *pBtn = (QPushButton *)sender();
    for(int i=0; i<m_functionBtnList.size(); i++)
    {
        auto pFunctionBtn = m_functionBtnList.at(i);
        if(pFunctionBtn == pBtn)
        {
            pFunctionBtn->setProperty("status", "selected");
        }
        else
        {
            pFunctionBtn->setProperty("status", "unselected");
        }
    }
    updateStyleSheet();
}

void MainWindow::slideBtnClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    ui->stackedWidget_catalog->setCurrentIndex(m_slideBtnGroup->id(btn));
    m_lineTab->updatePaintSlot(m_slideBtnGroup->id(btn));
//    if(m_slideBtnGroup->id(btn) == 2)
//        ui->widget_triangleTab->updatePaintSlot(m_slideBtnGroup->id(btn));
}

void MainWindow::slideBtn2Clicked()
{
    QPushButton *btn = (QPushButton *)sender();
    m_lineTab2->updatePaintSlot(m_slideBtnGroup2->id(btn));
}

void MainWindow::catalogWidgetInit()
{
    CatalogWidget *pCatalogWidget1 = new CatalogWidget(ui->widget_catalogWidget1);
    QGridLayout *pLayout1 = new QGridLayout(ui->widget_catalogWidget1);
    pLayout1->addWidget(pCatalogWidget1);

    CatalogWidget2 *pCatalogWidget2 = new CatalogWidget2(ui->widget_catalogWidget2);
    QGridLayout *pLayout2 = new QGridLayout(ui->widget_catalogWidget2);
    pLayout2->addWidget(pCatalogWidget2);
}

void MainWindow::iconWidgetInit()
{
    IconWidget *pIconWidget = new IconWidget(ui->widget_iconWidget);
    QGridLayout *pLayout = new QGridLayout(ui->widget_iconWidget);
    pLayout->addWidget(pIconWidget);
}

void MainWindow::databaseInit()
{
    m_myDatabase = MyDataBase::getInstance();

    connect(this, &MainWindow::databaseInitSignal, m_myDatabase, &MyDataBase::databaseInitSlot);
    connect(m_myDatabase, &MyDataBase::databaseInitCompleteSignal, this, &MainWindow::databaseInitCompleteSlot);
    connect(m_myDatabase, &MyDataBase::updateMsgSignal, this, &MainWindow::updateMsg);

//    connect(m_database, &DataBase::updateProgressValueSignal, this, &MainWindow::updateProgressValueSlot);

    emit databaseInitSignal("MYSQL");
}

void MainWindow::databaseInitCompleteSlot(bool sta)
{
    if(sta)
    {
        ui->label_dbSta->setPixmap(QPixmap(":/qss/image/db_success.png"));
        ui->label_dbSta->setToolTip("数据库连接成功");
    }
    else
    {
        ui->label_dbSta->setPixmap(QPixmap(":/qss/image/db_failed.png"));
        ui->label_dbSta->setToolTip("数据库连接失败");
    }
    updateStyleSheet();
}

void MainWindow::styleSheetInit()
{
    QFile file(":/Monitor.qss");
    if (file.open(QIODevice::ReadOnly))
    {
        m_qssByteArray = file.readAll();
        updateStyleSheet();
    }
    else
    {
        qDebug()<<file.errorString();
    }
}

void MainWindow::updateStyleSheet()
{
    this->setStyleSheet(m_qssByteArray);
}

void MainWindow::updateMsg(const QString& fileName, const QString& functionName, EN_MessageType msgType, const QString& msg)
{
    QString workLogStr;
    if(msgType == MESSAGE_EVENT)
    {
        workLogStr = QString("%1 : %2")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                            .arg(msg);
        ui->textBrowser_workLog->append(workLogStr);
        emit recordEventSignal(getCurTime(), fileName, functionName, msg);
    }
    else if(msgType == MESSAGE_OPERATOR)
    {
        workLogStr = QString("<font color=blue>%1 : %2")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                            .arg(msg);
        ui->textBrowser_operatorLog->append(workLogStr);
        emit recordOperatorSignal(getCurTime(), fileName, functionName, msg);
        return;
    }
    else if(msgType == MESSAGE_ERROR)
    {
        workLogStr = QString("<font color=red>%1 : %2")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                            .arg(msg);
        ui->textBrowser_errorLog->append(workLogStr);
        emit recordErrorSignal(getCurTime(), fileName, functionName, msg);
    }
}

void MainWindow::on_pushButton_messageTipsTest_clicked()
{
    MessageTips::showMessageTips("OK!", this);
}
