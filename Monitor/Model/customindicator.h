#ifndef CUSTOMINDICATOR_H
#define CUSTOMINDICATOR_H

#include <QFrame>
#include <QPainter>

class CustomIndicator : public QFrame
{
    Q_OBJECT
public:
    explicit CustomIndicator(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CUSTOMINDICATOR_H
