#ifndef COMMONTABLEMODEL_H
#define COMMONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QColor>

struct TESTDATA
{
    int level;
    int pos;
    QString serialNumber;
    QString model;
    QString firstFreq;
    QString curFreq;
    QString temp;
    QString freqDiff;
    QString state;
    int showCurve;
};

class CommonTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CommonTableModel(QAbstractTableModel *parent = nullptr);
    ~CommonTableModel();

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

    void updateModelData(QList<TESTDATA>& data);
    QList<TESTDATA> getModelData();

signals:
    void selectCurveSignal(int sta, int level, int pos);

private:
    QList<TESTDATA> m_modelData;
    QStringList m_horizontalHeadData; //表头数据
    QStringList m_verticalHeadData; //表头数据

    enum COLUMN_HEAD_INDEX {
        LEVEL = 0,
        POS,
        SERIAL,
        MODEL,
        FIRSTFREQ,
        CURFREQ,
        TEMP,
        FREQDIFF,
        STATE,
        SHOWCURVE,
        COLUMN
    };
};

#endif // COMMONTABLEMODEL_H
