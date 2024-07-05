#ifndef TRIANGLETAB_H
#define TRIANGLETAB_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QPropertyAnimation>
#include <QDebug>

class TriangleTab : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint trianglePosition READ getTrianglePosition WRITE setTrianglePosition)
public:
    explicit TriangleTab(QWidget *parent = nullptr);

    QPoint getTrianglePosition() const ;
    void setTrianglePosition(const QPoint &pos) ;

signals:

public slots:
    void updatePaintSlot(int idx);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPoint m_trianglePosition; // 用于存储三角形顶点位置的属性
    QPropertyAnimation *m_animation;

};

#endif // TRIANGLETAB_H
