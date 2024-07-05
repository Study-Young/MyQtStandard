#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class CatalogWidget;
}

class CatalogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget(QWidget *parent = nullptr);
    ~CatalogWidget();

private slots:
    void btnClicked();

private:
    void initForm();

    void initBtn(QButtonGroup *btnGroup, bool textBesideIcon);

    void initWidget1();
    void initWidget2();
    void initWidget3();
    void initWidget4();
    void initWidget5();
    void initWidget6();

private:
    Ui::CatalogWidget *ui;

    int iconSize;
    QList<int> icons;
};

#endif // CATALOGWIDGET_H
