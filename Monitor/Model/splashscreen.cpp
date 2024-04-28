#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QSplashScreen(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    QPixmap pixmap(":/image/crystal.png");
    setPixmap(pixmap);
    setFont(QFont("Dejavu Sans", 23));
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

