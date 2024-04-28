#include "customindicator.h"

CustomIndicator::CustomIndicator(QWidget *parent):
     QFrame(parent)
{
//    setFixedWidth(100);

}

void CustomIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = this->rect();
    QPen pen(Qt::blue);

    painter.setPen(Qt::NoPen); // 不绘制边框
    painter.setBrush(QBrush(QColor(0, 255, 0, 75))); // 设置填充颜色
    painter.drawRoundedRect(rect, 10, 10); // 设置圆角半径

    pen.setWidth(3);  // 设置线宽
    painter.setPen(pen);  //把画笔交给画家
    int startX = width() / 2 - 25;  // 开始点X坐标, 这里的25可设置为需要的线的长度
    int endX = width() / 2 + 25;  // 结束点X坐标
    int y = height() - 3;  // Y坐标
    painter.drawLine(startX, y, endX, y);
}
