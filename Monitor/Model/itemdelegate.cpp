#include "itemdelegate.h"
#include <QDateEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <QStyleOption>
#include <QApplication>
#include <QTextDocument>
#include <QDebug>

/****************************************************
 * CheckBoxDelegate
 ****************************************************/
static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
    //绘制按钮所需要的参数
    QStyleOptionButton checkBoxStyleOption;
    //按照给定的风格参数 返回元素子区域
    QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    //返回QCheckBox坐标
    QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,
                         viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);
    //返回QCheckBox几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

CheckBoxDelegate::CheckBoxDelegate(int col, QObject *parent):
    QStyledItemDelegate(parent)
{
    column = col;
}

// 绘制复选框
void CheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    if(index.column() == column){
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;
        checkBoxStyleOption.rect = CheckBoxRect(option);
        int row = index.row();
        if(row % 2 == 0)
            painter->fillRect(option.rect, QBrush(QColor(255, 229, 153)));
        else
            painter->fillRect(option.rect, QBrush(QColor(197, 224, 179)));

        QApplication::style()->drawControl(QStyle::CE_CheckBox,&checkBoxStyleOption,painter);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

// 响应鼠标事件，更新数据
bool CheckBoxDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) {
    if(index.column() == column){
        if((event->type() == QEvent::MouseButtonRelease) ||
                (event->type() == QEvent::MouseButtonDblClick)){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() != Qt::LeftButton ||
                    !CheckBoxRect(option).contains(mouseEvent->pos())){
                return true;
            }
            if(event->type() == QEvent::MouseButtonDblClick){
                return true;
            }
        }else if(event->type() == QEvent::KeyPress){
            if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                    static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
                return false;
            }
        }else{
            return false;
        }

        bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
        int data = checked ? 0 : 1;     //互逆
        return model->setData(index, data, Qt::EditRole);
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

/****************************************************
 * DateDelegate
 ****************************************************/
DateDelegate::DateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    editor->setCalendarPopup(true);
    return editor;
}

void DateDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QDateEdit *pDate = static_cast<QDateEdit*>(editor);
    pDate->setDate(QDate::fromString(str,"yyyy-MM-dd"));
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *pDate = static_cast<QDateEdit*>(editor);
    QString str = pDate->date().toString("yyyy-MM-dd");
    model->setData(index,str);
}

void DateDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * TimeDelegate
 ****************************************************/
TimeDelegate::TimeDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *TimeDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QTimeEdit *editor = new QTimeEdit(parent);
    editor->setDisplayFormat("hh:mm:ss");
    editor->setCalendarPopup(true);
    return editor;
}

void TimeDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QTimeEdit *pTime = static_cast<QTimeEdit*>(editor);
    pTime->setTime(QTime::fromString(str,"hh:mm:ss"));
}

void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *pTime = static_cast<QTimeEdit*>(editor);
    QString str = pTime->time().toString("hh:mm:ss");
    model->setData(index,str);
}

void TimeDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * DoubleLineEditDelegate
 ****************************************************/
DoubleLineEditDelegate::DoubleLineEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    min = 0;
    max = 99;
}

void DoubleLineEditDelegate::setRange(double bottom, double top, int decimals)
{
    min = bottom;
    max = top;
    dec = decimals;
}

QWidget *DoubleLineEditDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(new QDoubleValidator(min, max, dec));
    return editor;
}

void DoubleLineEditDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(str);
}

void DoubleLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString str = lineEdit->text();
    model->setData(index,str);
}

void DoubleLineEditDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * ButtonDelegate
 ****************************************************/
ButtonDelegate::ButtonDelegate(QString text, QObject *parent) :
    QItemDelegate(parent)
{
    m_btnText = text;
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* button = m_btns.value(index);
    if (!button) {
        button = new QStyleOptionButton();
        button->rect = option.rect.adjusted(4, (option.rect.height()/2-11), -4, -(option.rect.height()/2-11));
        button->text = m_btnText;
        button->state |= QStyle::State_Enabled;

        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, button);
    }

    button->rect = option.rect.adjusted(4, (option.rect.height()/2-11), -4, -(option.rect.height()/2-11));

    int row = index.row();
    if(row % 2 == 0)
        painter->fillRect(option.rect, QBrush(QColor(255, 229, 153)));
    else
        painter->fillRect(option.rect, QBrush(QColor(197, 224, 179)));

    painter->setPen(QPen(QColor(0, 0, 0)));
    painter->setBrush(QBrush(QColor(240, 240, 240)));
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->drawRoundedRect(button->rect, 4, 4);
    painter->drawText(button->rect, Qt::AlignCenter, button->text);

    painter->save();
    if(option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    painter->restore();

    QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, (option.rect.height()/2-11), -4, -(option.rect.height()/2-11)).contains(e->x(), e->y()) && m_btns.contains(index))
        {
            m_btns.value(index)->state |= QStyle::State_Sunken;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, (option.rect.height()/2-11), -4, -(option.rect.height()/2-11)).contains(e->x(), e->y()) && m_btns.contains(index))
        {
            m_btns.value(index)->state &= (~QStyle::State_Sunken);

            if(index.model()->data(index, Qt::DisplayRole).toInt() == 0)
                return model->setData(index, 1, Qt::EditRole);
            else
                return model->setData(index, 0, Qt::EditRole);
        }
    }
}

/****************************************************
 * ComboBoxDelegate
 ****************************************************/
ComboBoxDelegate::ComboBoxDelegate(QItemDelegate *parent)
    : QItemDelegate(parent)
{
    this->m_comboBoxList << "";
}

ComboBoxDelegate::ComboBoxDelegate(QStringList list, QItemDelegate *parent)
    : QItemDelegate(parent)
{
    this->m_comboBoxList = list;
}

ComboBoxDelegate::~ComboBoxDelegate() {

}
// 创建编辑器
QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // 创建自己需要的控件进行返回
    QComboBox *editor = new QComboBox(parent);

    return editor;
}

// 设置编辑器数据
void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    // 将参数editor转换为对应创建的控件，再进行数据初始设置就行
    QComboBox *cob = static_cast<QComboBox *>(editor);
    if (cob)
    {
        cob->addItems(m_comboBoxList);
    }
}

// 更新编辑器集合属性
void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // 将编辑器设置为矩形属性
    editor->setGeometry(option.rect);
}

// 设置模型数据
void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox *>(editor);	// 类型转换
    // 模型（单元格）显示的数据
    if (comboBox)
    {
        model->setData(index, comboBox->currentText());
    }
}

// 获取索引处的数据返回
QString ComboBoxDelegate::getCurrentComboBoxData(int index) {
    return this->m_comboBoxList.at(index);
}

// 插入数据
void ComboBoxDelegate::insertCoBData(QString str) {
    this->m_comboBoxList.append(str);
}

// 移除数据
void ComboBoxDelegate::removeCobData(QString str) {
    for (int i = 0; i < this->m_comboBoxList.size(); i++) {
        if (str == this->m_comboBoxList[i]) {
            this->m_comboBoxList.removeAt(i);
            return;
        }
    }
}

WrapTextDelegate::WrapTextDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

WrapTextDelegate::~WrapTextDelegate()
{
}

void WrapTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)(index);

    int row = index.row();
    if(row % 2 == 0)
        painter->fillRect(option.rect, QBrush(QColor(255, 229, 153)));
    else
        painter->fillRect(option.rect, QBrush(QColor(197, 224, 179)));


    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->save();

    // Set up a QTextDocument to calculate the size needed for the wrapped text
    QTextDocument doc;
    doc.setPlainText(text);
    doc.setTextWidth(option.rect.width());

    // Calculate the height needed for the wrapped text
    int textHeight = static_cast<int>(doc.size().height());

    // Adjust the item rectangle to include the required height for the wrapped text
    QRect adjustedRect = option.rect;
    adjustedRect.setHeight(textHeight);
    //绘制文本
    painter->drawText(adjustedRect, Qt::TextWordWrap | Qt::AlignVCenter | Qt::AlignHCenter, text);

    painter->restore();
}

QSize WrapTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)(index);

    QString text = index.model()->data(index, Qt::DisplayRole).toString();

    // Set up a QTextDocument to calculate the size needed for the wrapped text
    QTextDocument doc;
    doc.setPlainText(text);
    doc.setTextWidth(option.rect.width());

    // Calculate the height needed for the wrapped text
    int textHeight = static_cast<int>(doc.size().height());

    // Return the adjusted size hint
    return QSize(option.rect.width(), textHeight);
}
