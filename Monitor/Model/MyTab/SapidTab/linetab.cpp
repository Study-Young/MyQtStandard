#include "linetab.h"

LineTab::LineTab(QWidget *parent):
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_animation = new QPropertyAnimation(this, "itemPos");
}

void LineTab::setItemNum(int num)
{
    m_itemNum = num;
}

int LineTab::getItemNum()
{
    return m_itemNum;
}

void LineTab::setLineDirection(int direction)
{
    m_lineDirection = direction;
}

int LineTab::getLineDirection()
{
    return m_lineDirection;
}

double LineTab::getItemPos() const
{
    return m_curItemPos;
}

void LineTab::setItemPos(double pos)
{
    m_curItemPos = pos;
    update();
}

void LineTab::updatePaintSlot(double pos)
{
    m_animation->setStartValue(m_lastItemPos);
    m_animation->setEndValue(pos);
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutBack);
    m_animation->start();
    m_lastItemPos = pos;
}

void LineTab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::blue);
    pen.setWidth(3);  // 设置线宽
    painter.setPen(pen);  //把画笔交给画家

    if(m_lineDirection == 0)
    {
        int startX = m_itemWidth * m_curItemPos;  // 开始点X坐标
        int endX = startX + m_itemWidth;  // 结束点X坐标
        int y = 0;  // Y坐标
        painter.drawLine(startX, y, endX, y);
    }
    else if(m_lineDirection == 1)
    {
        int x = 0;
        int startY = m_itemHeight * m_curItemPos;  // 开始点X坐标
        int endY = startY + m_itemHeight;  // 结束点X坐标
        painter.drawLine(x, startY, x, endY);
    }
}

void LineTab::resizeEvent(QResizeEvent *event)
{
    if(m_lineDirection == 0)
        m_itemWidth = width() / m_itemNum;
    else if(m_lineDirection == 1)
        m_itemHeight = height() / m_itemNum;
}

