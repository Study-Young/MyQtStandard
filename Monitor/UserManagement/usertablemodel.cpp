#include "usertablemodel.h"

#include <QDebug>

UserTableModel::UserTableModel(QAbstractTableModel *parent)
    : QAbstractTableModel(parent)
{
    m_horizontalHeadData << "序号" << "用户名" << "身份";
}


int UserTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->m_modelData.size();
}

int UserTableModel::columnCount(const QModelIndex &parent) const
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

QVariant UserTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) // 返回一个默认数据的表头
        return QAbstractTableModel::headerData(section, orientation, role);


    if (orientation == Qt::Orientation::Horizontal) {
        return this->m_horizontalHeadData[section];
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant UserTableModel::data(const QModelIndex& index, int role) const
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

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
            case COLUMN_HEAD_INDEX::SEQ:
                return this->m_modelData.at(index.row()).seq;
                break;
            case COLUMN_HEAD_INDEX::USERNAME:
                return this->m_modelData.at(index.row()).userName;
                break;
            case COLUMN_HEAD_INDEX::LEVEL:
                return this->m_modelData.at(index.row()).level;
                break;
            default:
                return QVariant();
                break;
        }
    }
    return QVariant();
}

// 返回模型索引
QModelIndex UserTableModel::index(int row, int column, const QModelIndex &parent) const
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
Qt::ItemFlags UserTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled;
}

// 设置模型索引数据
bool UserTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        if(role == Qt::EditRole)
        {
            switch (index.column())
            {
                case COLUMN_HEAD_INDEX::SEQ:
                    this->m_modelData[index.row()].seq = value.toInt();
                    break;
                case COLUMN_HEAD_INDEX::USERNAME:
                    this->m_modelData[index.row()].userName = value.toString();
                    break;
                case COLUMN_HEAD_INDEX::LEVEL:
                    this->m_modelData[index.row()].level = value.toString();
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
bool UserTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    // 如果插入零行，则返回false，表示插入失败
    if (count == 0) return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    // 按照位置在链表对应位置进行插入数据
    for (int addCount = 0; addCount < count; addCount++) {
        int addRow = row + addCount;
        m_modelData.insert(addRow, USERDATA());
    }

    // 结束插入行
    endInsertRows();

    return true;
}

// 删除行
bool UserTableModel::removeRows(int row, int count, const QModelIndex &parent)
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

void UserTableModel::SetHorizontalHeadData(const QStringList& horizontalList)
{
    m_horizontalHeadData = horizontalList;
}

void UserTableModel::SetVerticalHeadData(const QStringList &verticalList)
{
    m_verticalHeadData = verticalList;
}

void UserTableModel::updateModelData(QList<USERDATA>& data)
{
    beginResetModel();
    m_modelData = data;
    endResetModel();
}

QList<USERDATA> UserTableModel::getModelData()
{
    return m_modelData;
}
