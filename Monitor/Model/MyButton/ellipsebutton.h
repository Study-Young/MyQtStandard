#ifndef ELLIPSEBUTTON_H
#define ELLIPSEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTimerEvent>

class EllipseButton : public QWidget
{
    Q_OBJECT
public:
    explicit EllipseButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QTimer *timer;
    int animationValue = 0;
};

#endif // ELLIPSEBUTTON_H
