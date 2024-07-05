#ifndef LINETAB_H
#define LINETAB_H

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QDebug>

class LineTab : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double itemPos READ getItemPos WRITE setItemPos)
public:
    explicit LineTab(QWidget *parent = nullptr);

    void setItemNum(int num);
    int getItemNum();

    void setLineDirection(int direction);
    int getLineDirection();

    double getItemPos() const;
    void setItemPos(double pos);

public slots:
    void updatePaintSlot(double pos);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    int m_itemNum = 0; // 按钮数量
    int m_lineDirection = 0;    //0:横向 1:纵向

    int m_itemWidth = 0;
    int m_itemHeight = 0;

    double m_curItemPos = 0.0;
    double m_lastItemPos = 0.0;

    QPropertyAnimation *m_animation;
};

#endif // LINETAB_H
