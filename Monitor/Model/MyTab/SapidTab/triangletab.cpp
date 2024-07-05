#include "triangletab.h"

TriangleTab::TriangleTab(QWidget *parent) :
    QWidget(parent),
    m_trianglePosition(0, 0)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_animation = new QPropertyAnimation(this, "trianglePosition");
}

QPoint TriangleTab::getTrianglePosition() const
{
    return m_trianglePosition;
}

void TriangleTab::setTrianglePosition(const QPoint &pos)
{
    m_trianglePosition = pos;
    update();  // 重绘组件
}

void TriangleTab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

    // 定义三个点以绘制三角形
    QPoint p1 = m_trianglePosition + QPoint(0, -30); // 顶点
    QPoint p2 = m_trianglePosition + QPoint(-20, 20); // 左下角
    QPoint p3 = m_trianglePosition + QPoint(20, 20); // 右下角

    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.closeSubpath();

    painter.drawPath(path);
}

void TriangleTab::updatePaintSlot(int idx)
{
    m_animation->setDuration(1000); // 动画持续时间 1000 毫秒
    m_animation->setStartValue(QPoint(50, 50));  // 起始位置
    m_animation->setEndValue(QPoint(150, 150));  // 结束位置
    m_animation->setLoopCount(-1); // 循环动画
    m_animation->start();
}
