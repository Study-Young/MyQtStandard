#include "catalogwidget.h"
#include "ui_catalogwidget.h"
#include "Model/IconHelper/iconhelper.h"

CatalogWidget::CatalogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogWidget)
{
    ui->setupUi(this);

    initForm();
    initWidget1();
    initWidget2();
    initWidget3();
    initWidget4();
    initWidget5();
    initWidget6();
}

CatalogWidget::~CatalogWidget()
{
    delete ui;
}

void CatalogWidget::initForm()
{
    iconSize = 40;
    //图标对应图形字体值
    icons << 0xf2ba << 0xf002 << 0xf013 << 0xf021;

    //设置样式表
    QStringList qss;
    qss << QString("#labIcon{color:#32B9CF;}");
    qss << QString("#btnIcon{color:#C13256;}");
    qss << QString("#labValue,#labCount,#labInfo1,#labInfo2,#labInfo3,#labInfo4{font-weight:bold;font-size:20px;}");
    qss << QString("QWidget#widget1 QAbstractButton{min-height:%1px;max-height:%1px;}").arg(35);
    qss << QString("QWidget#widget2 QAbstractButton{min-height:%1px;max-height:%1px;}").arg(35);
    qss << QString("QWidget#widget3 QAbstractButton{min-height:%1px;max-height:%1px;}").arg(70);
    qss << QString("QWidget#widget4 QAbstractButton{min-height:%1px;max-height:%1px;}").arg(70);
    qss << QString("QWidget#widget5 QAbstractButton{min-width:%1px;max-width:%1px;}").arg(90);
    qss << QString("QWidget#widget6 QAbstractButton{min-width:%1px;max-width:%1px;}").arg(90);
    this->setStyleSheet(qss.join(""));
}

void CatalogWidget::initBtn(QButtonGroup *btnGroup, bool textBesideIcon)
{
    QList<QAbstractButton *> btns = btnGroup->buttons();
    foreach (QAbstractButton *btn, btns) {
        QToolButton *b = (QToolButton *)btn;
        //关联按钮单击事件
        connect(b, SIGNAL(clicked(bool)), this, SLOT(btnClicked()));
        b->setCheckable(true);
        b->setToolButtonStyle(textBesideIcon ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonTextUnderIcon);
    }
}

void CatalogWidget::btnClicked()
{
    QAbstractButton *btn = (QAbstractButton *)sender();
    QString objName = btn->parent()->objectName();
    if (objName == "widget1") {
        ui->labInfo1->setText(btn->text());
    } else if (objName == "widget2") {
        ui->labInfo2->setText(btn->text());
    } else if (objName == "widget3") {
        ui->labInfo3->setText(btn->text());
    } else if (objName == "widget4") {
        ui->labInfo4->setText(btn->text());
    }
}

void CatalogWidget::initWidget1()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn11);
    btnGroup->addButton(ui->btn12);
    btnGroup->addButton(ui->btn13);
    btnGroup->addButton(ui->btn14);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, true);
    //设置弱属性以便应用样式
    ui->widget1->setProperty("flag", "left");

    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    styleColor.position = "left";
    styleColor.iconSize = 18;
    styleColor.iconWidth = 30;
    styleColor.iconHeight = 25;
    styleColor.borderWidth = 4;
    IconHelper::setStyle(ui->widget1, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn11->click();
}

void CatalogWidget::initWidget2()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn21);
    btnGroup->addButton(ui->btn22);
    btnGroup->addButton(ui->btn23);
    btnGroup->addButton(ui->btn24);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, true);
    //设置弱属性以便应用样式
    ui->widget2->setProperty("flag", "right");

    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    styleColor.position = "right";
    styleColor.iconSize = 18;
    styleColor.iconWidth = 25;
    styleColor.iconHeight = 20;
    styleColor.borderWidth = 4;
    styleColor.borderColor = "#32B9CF";
    styleColor.setColor("#187294", "#B6D7E3", "#145C75", "#F0F0F0");
    IconHelper::setStyle(ui->widget2, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn22->click();
}

void CatalogWidget::initWidget3()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn31);
    btnGroup->addButton(ui->btn32);
    btnGroup->addButton(ui->btn33);
    btnGroup->addButton(ui->btn34);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, false);
    //设置弱属性以便应用样式
    ui->widget3->setProperty("flag", "left");

    IconHelper::StyleColor styleColor;
    styleColor.position = "left";
    styleColor.iconSize = 30;
    styleColor.iconWidth = 40;
    styleColor.iconHeight = 40;
    styleColor.borderWidth = 3;
    styleColor.borderColor = "#609EE9";
    IconHelper::setStyle(ui->widget3, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn33->click();
}

void CatalogWidget::initWidget4()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn41);
    btnGroup->addButton(ui->btn42);
    btnGroup->addButton(ui->btn43);
    btnGroup->addButton(ui->btn44);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, false);
    //设置弱属性以便应用样式
    ui->widget4->setProperty("flag", "right");

    IconHelper::StyleColor styleColor;
    styleColor.position = "right";
    styleColor.iconSize = 30;
    styleColor.iconWidth = 40;
    styleColor.iconHeight = 40;
    styleColor.borderWidth = 3;
    styleColor.borderColor = "#F7AE13";
    styleColor.setColor("#FCDC97", "#54626F", "#FFF0BC", "#54626F");
    IconHelper::setStyle(ui->widget4, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn44->click();
}

void CatalogWidget::initWidget5()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn51);
    btnGroup->addButton(ui->btn52);
    btnGroup->addButton(ui->btn53);
    btnGroup->addButton(ui->btn54);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, false);
    //设置弱属性以便应用样式
    ui->widget5->setProperty("flag", "top");

    //设置整体按钮组样式
    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    styleColor.position = "top";
    styleColor.iconSize = 25;
    styleColor.iconWidth = 25;
    styleColor.iconHeight = 25;
    styleColor.borderWidth = 3;
    IconHelper::setStyle(ui->widget5, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn51->click();
}

void CatalogWidget::initWidget6()
{
    //加入按钮组自动互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn61);
    btnGroup->addButton(ui->btn62);
    btnGroup->addButton(ui->btn63);
    btnGroup->addButton(ui->btn64);

    //设置按钮可选中以及图标样式
    initBtn(btnGroup, false);
    //设置弱属性以便应用样式
    ui->widget6->setProperty("flag", "bottom");

    //设置整体按钮组样式
    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    styleColor.position = "bottom";
    styleColor.iconSize = 25;
    styleColor.iconWidth = 25;
    styleColor.iconHeight = 25;
    styleColor.borderWidth = 3;
    styleColor.borderColor = "#A279C5";
    styleColor.setColor("#292929", "#B6D7E3", "#10689A", "#F0F0F0");
    IconHelper::setStyle(ui->widget6, btnGroup->buttons(), icons, styleColor);

    //默认选中某个按钮
    ui->btn63->click();
}
