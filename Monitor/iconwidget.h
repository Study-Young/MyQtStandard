#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

namespace Ui {
class IconWidget;
}

class IconWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IconWidget(QWidget *parent = nullptr);
    ~IconWidget();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void initPanel();
    void toggled(bool checked);

private:
    void initForm();
    bool checkIcon(int icon);

private:
    Ui::IconWidget *ui;

    int iconSize;
    QList<QWidget *> labs;
};

#endif // ICONWIDGET_H
