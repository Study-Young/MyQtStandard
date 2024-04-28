#ifndef USERTABLEMODEL_H
#define USERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QColor>

struct USERDATA
{
    int seq;
    QString userName;
    QString level;
};

class UserTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    UserTableModel(QAbstractTableModel *parent = nullptr);

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

    void SetHorizontalHeadData(const QStringList& horizontalList);  //设置表头数据
    void SetVerticalHeadData(const QStringList& verticalList);

    void updateModelData(QList<USERDATA>& data);
    QList<USERDATA> getModelData();

private:
    QList<USERDATA> m_modelData;
    QStringList m_horizontalHeadData; //表头数据
    QStringList m_verticalHeadData; //表头数据

    enum COLUMN_HEAD_INDEX {
        SEQ = 0,
        USERNAME,
        LEVEL,
        COLUMN
    };
};


#endif // USERTABLEMODEL_H
