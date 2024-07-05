#ifndef CALDATATABLEMODEL_H
#define CALDATATABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QColor>

enum CALDATATABLE_COLUMN_HEAD_INDEX {
    TimeInterval = 0,
    CalRlt = 1,
    Colunm = 2
};

class CalDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CalDataTableModel(QAbstractTableModel *parent = nullptr);
    ~CalDataTableModel();

    // 返回行个数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // 返回列个数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 返回头文本
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // 返回索引数据
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // 返回模型索引
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // 返回模型索引的编辑方式
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    // 设置模型索引数据
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    // 插入行
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // 删除行
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setHeadDataList(QStringList list);

private:
    QStringList m_timeInterval;
    QStringList m_calRlt;
    // 存储水平方向头数据
    QStringList m_HorizontalHead;
};

#endif // CALDATATABLEMODEL_H
