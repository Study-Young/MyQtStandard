#ifndef CATALOGWIDGET2_H
#define CATALOGWIDGET2_H

#include <QWidget>
#include <QDebug>

#include "Model/MyButton/navbutton.h"
#include "Model/IconHelper/iconhelper.h"

namespace Ui {
class CatalogWidget2;
}

class CatalogWidget2 : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget2(QWidget *parent = nullptr);
    ~CatalogWidget2();

private slots:
    void buttonClick1();
    void buttonClick2();
    void buttonClick3();
    void buttonClick4();
    void buttonClick5();
    void buttonClick6();
    void buttonClick7();

private:
    void initForm();
    void initBtn1();
    void initBtn2();
    void initBtn3();
    void initBtn4();
    void initBtn5();
    void initBtn6();
    void initBtn7();

private:
    Ui::CatalogWidget2 *ui;
    QList<int> icons;
    QList<NavButton *> btns1;
    QList<NavButton *> btns2;
    QList<NavButton *> btns3;
    QList<NavButton *> btns4;
    QList<NavButton *> btns5;
    QList<NavButton *> btns6;
    QList<NavButton *> btns7;
};

#endif // CATALOGWIDGET2_H
