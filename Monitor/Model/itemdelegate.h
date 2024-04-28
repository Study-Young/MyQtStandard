#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPushButton>
#include <QComboBox>

/*
 * ReadOnly
 */
class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = 0): QItemDelegate(parent) {}

//    void setItems(QStringList items);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const
    {
        return NULL;
    }
};

/*
 * CheckBox
 */
class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CheckBoxDelegate(int col, QObject *parent = 0);

protected:
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);

private:
    int column;         //设置复选框的列
};

/*
 * calendar
 */
class DateDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DateDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
};

/*
 * TimeEdit
 */
class TimeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TimeDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
};

/*
 * DoubleLineEdit
 */
class DoubleLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DoubleLineEditDelegate(QObject *parent = 0);

    void setRange(double bottom, double top, int decimals = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;

private:
    int min;        //最小值
    int max;        //最大值
    int dec;        //小数位数
};

//头像列，只是在单元格中央放一张小图而已
class IconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    IconDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex & index ) const
    {
        //show.bmp是在工程目录中的一张图片（其实就是QQ的图标啦，呵呵）
        QPixmap pixmap = QPixmap("show.bmp").scaled(24, 24);
        qApp->style()->drawItemPixmap(painter, option.rect,  Qt::AlignCenter, QPixmap(pixmap));
    }
};

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QString text, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

public slots:

private:
    QMap<QModelIndex, QStyleOptionButton*> m_btns;
    QString m_btnText;
};

class ComboBoxDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QItemDelegate *parent = nullptr);
    ComboBoxDelegate(QStringList list, QItemDelegate *parent = nullptr);
    ~ComboBoxDelegate();

    // 创建编辑器
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 设置编辑器数据
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    // 更新编辑器集合属性
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 设置模型数据
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;


    /*****根据项目需求增添辅佐函数*****/

    // 获取索引数据
    QString getCurrentComboBoxData(int index);
    // 插入
    void insertCoBData(QString str);
    // 删除
    void removeCobData(QString str);

private:
    QStringList m_comboBoxList;

};

class WrapTextDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit WrapTextDelegate(QObject *parent = Q_NULLPTR);
    ~WrapTextDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ITEMDELEGATE_H
