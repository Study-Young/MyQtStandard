#include "iconwidget.h"
#include "ui_iconwidget.h"

#include "Model/IconHelper/iconhelper.h"

IconWidget::IconWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IconWidget)
{
    ui->setupUi(this);
    initForm();
    QTimer::singleShot(100, this, SLOT(initPanel()));
}

IconWidget::~IconWidget()
{
    delete ui;
}

bool IconWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        QLabel *lab = (QLabel *)watched;
        if (lab != 0) {
            //由于有图形字体的范围值冲突需要手动切换索引
            if (ui->rbtnFontAwesome6->isChecked()) {
                IconHelper::setIconFontIndex(2);
            } else if (ui->rbtnFontWeather->isChecked()) {
                IconHelper::setIconFontIndex(3);
            } else {
                IconHelper::setIconFontIndex(-1);
            }

            //对应图形字体的16进制值已经赋值给了 toolTip
            QString value = lab->toolTip();
            ui->labValue->setText(value);
            int icon = value.toInt(NULL, 16);

            IconHelper::setIcon(ui->labIcon, icon, iconSize);
            IconHelper::setIcon(ui->btnIcon, icon, iconSize);

            //万能大法直接从指定标识获取图片文件
            QPixmap pix = IconHelper::getPixmap("#753775", icon, iconSize, iconSize, iconSize);
            ui->labImage->setPixmap(pix);

            //设置图标  以下方法二选一都可以
            //ui->btnImage->setIcon(QIcon(pix));
            IconHelper::setPixmap(ui->btnImage, "#FD8B28", icon, iconSize, iconSize, iconSize);

            //取出对应图形字体类
            QFont font = IconHelper::getIconFontAwesome();
            if (ui->rbtnFontAliBaBa->isChecked()) {
                font = IconHelper::getIconFontAliBaBa();
            } else if (ui->rbtnFontAwesome6->isChecked()) {
                font = IconHelper::getIconFontAwesome6();
            } else if (ui->rbtnFontWeather->isChecked()) {
                font = IconHelper::getIconFontWeather();
            }

            //直接设置图标+文本到按钮
            font.setPixelSize(15);
            ui->btnTest1->setFont(font);
            ui->btnTest1->setText(QString("%1 测试图标").arg((QChar)icon));

            //分别设置图标+文本到按钮
            ui->btnTest2->setIcon(QIcon(pix));
        }
    }

    return QWidget::eventFilter(watched, event);
}

void IconWidget::initForm()
{
    iconSize = 40;

    //设置图形字体
    IconHelper::setIcon(ui->labIcon, 0xf067, iconSize);
    IconHelper::setIcon(ui->btnIcon, 0xf067, iconSize);
    QPixmap pix = IconHelper::getPixmap("#753775", 0xf067, iconSize, iconSize, iconSize);
    ui->labImage->setPixmap(pix);
    ui->btnImage->setIconSize(QSize(iconSize, iconSize));
    ui->btnImage->setIcon(QIcon(pix));

    //关联单选框切换
    QList<QRadioButton *> rbtns = ui->widgetRight->findChildren<QRadioButton *>();
    foreach (QRadioButton *rbtn, rbtns) {
        connect(rbtn, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
    }
}

void IconWidget::initPanel()
{
    //清空原有对象
    qDeleteAll(labs);
    labs.clear();

    //选择不同的图形字体
    int start = 0xf000;
    int end = 0xf2e0;
    QFont iconFont = IconHelper::getIconFontAwesome();
    IconHelper::setIconFontIndex(-1);

    if (ui->rbtnFontAliBaBa->isChecked()) {
        start = 0xe500;
        end = 0xec00;
        iconFont = IconHelper::getIconFontAliBaBa();
    } else if (ui->rbtnFontAwesome6->isChecked()) {
        start = 0xe000;
        end = 0xf8ff;
        iconFont = IconHelper::getIconFontAwesome6();
        IconHelper::setIconFontIndex(2);
    } else if (ui->rbtnFontWeather->isChecked()) {
        start = 0xe900;
        end = 0xe9cf;
        iconFont = IconHelper::getIconFontWeather();
        IconHelper::setIconFontIndex(3);
    }

    //设置字体大小
    iconFont.setPixelSize(15);

    //加载图形字体面板
    QStringList list;
    for (int icon = start; icon <= end; icon++) {
        //中间有一段是空的,可以自行屏蔽下面这段代码查看这段空的值对应的文字
        if (ui->rbtnFontAliBaBa->isChecked()) {
            if (icon >= 0xe76c && icon <= 0xe8f8) {
                continue;
            }
        } else if (ui->rbtnFontAwesome6->isChecked()) {
            if (icon >= 0xe33d && icon <= 0xefff) {
                continue;
            }
        }

        QString tip = "0x" + QString::number(icon, 16);
        if (!checkIcon(icon)) {
            list << tip;
            continue;
        }

        QLabel *lab = new QLabel;
        lab->installEventFilter(this);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFont(iconFont);
        lab->setText((QChar)icon);
        lab->setToolTip(tip);
        lab->setMinimumSize(30, 30);
        labs << lab;
    }

    //qDebug() << "no text font" << list.count() << list;
    ui->widgetFontPanel->setAutoWidth(true);
    ui->widgetFontPanel->setMargin(3);
    ui->widgetFontPanel->setSpace(3);
    ui->widgetFontPanel->setColumnCount(18);
    ui->widgetFontPanel->setWidgets(labs);

    //设置图形字体面板鼠标悬停时的效果
    QString qss = QString("QLabel:hover,QLabel:focus{color:%1;border:1px solid %1;}").arg("#00BB9E");
    ui->widgetFontPanel->setStyleSheet(qss);

    int count = end - start + 1;
    ui->labCount->setText(QString("%1/%2").arg(labs.count()).arg(count));
}

bool IconWidget::checkIcon(int icon)
{
    //从图形字体对应值生成一个指定颜色的图片
    QPixmap pix = IconHelper::getPixmap("#FF0000", icon, 120, 120, 120);
    QImage img = pix.toImage();
    int width = img.width();
    int height = img.height();

    //过滤不存在的图形字体
    //对该图片逐个扫描像素点,都是空白则意味着当前图形字体不存在
    for (int i = 0; i < height; ++i) {
        uchar *lineByte = img.scanLine(i);
        for (int j = 0; j < width; j++) {
            uchar tp = lineByte[j];
            if (tp > 0x00) {
                return true;
            }
        }
    }

    //保存下图片看下
    //QString fileName = QString("%1/icon/%2.jpg").arg(qApp->applicationDirPath()).arg(icon);
    //pix.save(fileName, "jpg");
    return false;
}

void IconWidget::toggled(bool checked)
{
    //单选框按下后自动重新加载对应的图形字体
    if (checked) {
        initPanel();
    }
}
