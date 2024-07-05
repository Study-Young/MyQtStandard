#include "ellipsebutton.h"


EllipseButton::EllipseButton(QWidget *parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void EllipseButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::blue, 2));

    QRect rect(this->rect());
    int startAngle = 90 * 16;  // 弧的起始角度
    int endAngle = animationValue * 16;  // 弧的跨越角度

    painter.drawArc(rect, startAngle, endAngle); // 画弧形。由于它是逐渐绘制的，所以它看起来是在逐渐绘制四分之一圆的边框
    animationValue += 10;  // 每次递增10度
}

void EllipseButton::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
//    timer->start(1000/25); // 25 FPS
}
