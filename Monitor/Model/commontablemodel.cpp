#include "commontablemodel.h"
#include <QDebug>

CommonTableModel::CommonTableModel(QAbstractTableModel *parent)
    : QAbstractTableModel(parent)
{
    m_horizontalHeadData << "层号" << "位号" << "序列号" << "赛思料号" << "第一次测试频率" << "当前频率" << "当前温度" << "频率偏移" << "当前状态" << "绘图";
}

CommonTableModel::~CommonTableModel()
{

}

int CommonTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->m_modelData.size();
}

int CommonTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    else
    {
        return COLUMN_HEAD_INDEX::COLUMN;   //表头的大小
    }
}

QVariant CommonTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) // 返回一个默认数据的表头
        return QAbstractTableModel::headerData(section, orientation, role);


    if (orientation == Qt::Orientation::Horizontal) {
        return this->m_horizontalHeadData[section];
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant CommonTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)
    {    //对齐方式
        return Qt::AlignCenter;             //对齐格式为居中
    }

    if (role == Qt::BackgroundColorRole)
    {
        int row = index.row();
        if(row % 2 == 0)
            return QColor(255, 229, 153);
        else
            return QColor(197, 224, 179);
    }

    if (role == Qt::TextColorRole)
    {
        if(index.column() == COLUMN_HEAD_INDEX::STATE)
        {
            if(m_modelData.at(index.row()).state.contains("异常"))
                return QColor(Qt::red);
            else
                return QColor(Qt::black);
        }
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
            case COLUMN_HEAD_INDEX::LEVEL:
                return this->m_modelData.at(index.row()).level;
                break;
            case COLUMN_HEAD_INDEX::POS:
                return this->m_modelData.at(index.row()).pos;
                break;
            case COLUMN_HEAD_INDEX::SERIAL:
                return this->m_modelData.at(index.row()).serialNumber;
                break;
            case COLUMN_HEAD_INDEX::MODEL:
                return this->m_modelData.at(index.row()).model;
                break;
            case COLUMN_HEAD_INDEX::FIRSTFREQ:
                return this->m_modelData.at(index.row()).firstFreq;
                break;
            case COLUMN_HEAD_INDEX::CURFREQ:
                return this->m_modelData.at(index.row()).curFreq;
                break;
            case COLUMN_HEAD_INDEX::TEMP:
                return this->m_modelData.at(index.row()).temp;
                break;
            case COLUMN_HEAD_INDEX::FREQDIFF:
                return this->m_modelData.at(index.row()).freqDiff;
                break;
            case COLUMN_HEAD_INDEX::STATE:
                return this->m_modelData.at(index.row()).state;
                break;
            case COLUMN_HEAD_INDEX::SHOWCURVE:
                return this->m_modelData.at(index.row()).showCurve;
                break;
            default:
                return QVariant();
                break;
        }
    }
    return QVariant();
}

// 返回模型索引
QModelIndex CommonTableModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // 行和列的合法性检查
    if (row < 0 || row >= m_modelData.size() || column < 0 || column >= COLUMN_HEAD_INDEX::COLUMN)
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}

// 返回模型索引的编辑方式
Qt::ItemFlags CommonTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled;
}

// 设置模型索引数据
bool CommonTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        if(role == Qt::EditRole)
        {
            switch (index.column())
            {
                case COLUMN_HEAD_INDEX::LEVEL:
                    this->m_modelData[index.row()].level = value.toInt();
                    break;
                case COLUMN_HEAD_INDEX::POS:
                    this->m_modelData[index.row()].pos = value.toInt();
                    break;
                case COLUMN_HEAD_INDEX::SERIAL:
                    this->m_modelData[index.row()].serialNumber = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::MODEL:
                    this->m_modelData[index.row()].model = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::FIRSTFREQ:
                    this->m_modelData[index.row()].firstFreq = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::CURFREQ:
                    this->m_modelData[index.row()].curFreq = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::TEMP:
                    this->m_modelData[index.row()].temp = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::FREQDIFF:
                    this->m_modelData[index.row()].freqDiff = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::STATE:
                    this->m_modelData[index.row()].state = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::SHOWCURVE:
                    this->m_modelData[index.row()].showCurve = value.toInt();
                    emit selectCurveSignal(m_modelData[index.row()].showCurve, m_modelData[index.row()].level, m_modelData[index.row()].pos);
                    break;
                default:
                    break;
            }
        }
        emit dataChanged(index, index);	// 参数是左上角和右下角的模型索引（这里是相同的）
        return true;
    }
    return false;
}

// 插入行	参数：插入的位置；插入的行数；父项的模型索引
bool CommonTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    // 如果插入零行，则返回false，表示插入失败
    if (count == 0) return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    // 按照位置在链表对应位置进行插入数据
    for (int addCount = 0; addCount < count; addCount++) {
        int addRow = row + addCount;
        m_modelData.insert(addRow, TESTDATA());
    }

    // 结束插入行
    endInsertRows();

    return true;
}

// 删除行
bool CommonTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (count == 0) return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int removeCount = 0; removeCount < count; removeCount++)
    {
        int rmRow = row + removeCount;
        m_modelData.removeAt(rmRow);
    }

    endRemoveRows();

    return true;
}

void CommonTableModel::SetHorizontalHeadData(const QStringList& horizontalList)
{
    m_horizontalHeadData = horizontalList;
}

void CommonTableModel::SetVerticalHeadData(const QStringList &verticalList)
{
    m_verticalHeadData = verticalList;
}

void CommonTableModel::updateModelData(QList<TESTDATA>& data)
{
    beginResetModel();
    m_modelData = data;
    endResetModel();
}

QList<TESTDATA> CommonTableModel::getModelData()
{
    return m_modelData;
}
