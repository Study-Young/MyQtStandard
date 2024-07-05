#ifndef MYCHECKBOX2_H
#define MYCHECKBOX2_H

#include "mycheckbox.h"

class MyCheckBox2 : public MyCheckBox
{
public:
    MyCheckBox2(QWidget* parent = nullptr);

    void setUncheckedColor(QColor c);

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

    virtual void checkStateChanged(int state) override;

    virtual void drawBox(QPainter &painter, QRectF rect) override;

protected:
    QColor uncheckedColor = QColor("#88888888");
};

#endif // MYCHECKBOX2_H
