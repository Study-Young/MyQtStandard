#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "Model/splashscreen.h"

#include <QApplication>

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString logText;

    // 选择日志级别，并构造日志文本
    switch (type) {
    case QtDebugMsg:
        logText = QString("Debug: %1").arg(msg);
        break;
    case QtInfoMsg:
        logText = QString("Info: %1").arg(msg);
        break;
    case QtWarningMsg:
        logText = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        logText = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        logText = QString("Fatal: %1").arg(msg);
        break;
    default:
        break;
    }

    // 写入日志文件
    QFile logFile("myTestDebug.txt");
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream textStream(&logFile);
    textStream << logText << endl;
    logFile.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

//    qInstallMessageHandler(myMessageOutput);

//    SplashScreen *splash = new SplashScreen();
//    splash->show();
//    splash->showMessage("软件初始化...", Qt::AlignLeft |Qt::AlignBottom, Qt::black);

    ConfigManagement::getInstance();
    MyDataBase::getInstance();
    UserManagement::getInstance();

    MainWindow w;
//    splash->finish(&w);
//    delete splash;

//    if(UserManagement::getInstance()->exec() == QDialog::Rejected)
//    {
//        exit(0);
//    }

//    w.updateCurrentUserInfo();
    w.show();
    return a.exec();
}
