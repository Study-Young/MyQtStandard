#include "caldatatablemodel.h"
#include <QtDebug>

#pragma execution_character_set("utf-8")  

CalDataTableModel::CalDataTableModel(QAbstractTableModel *parent)
    : QAbstractTableModel(parent)
{
    m_HorizontalHead << QString("时间间隔")
                     << QString("稳定度");
    m_timeInterval << "1" << "2" << "4" << "8" << "10" << "20" << "40" << "80"
                   << "100" << "200" << "400" << "800" << "1000" << "2000" << "4000" << "8000"
                   << "10000" << "20000" << "40000" << "80000" << "86400" << "100000";
    insertRows(0, m_timeInterval.size());
}

CalDataTableModel::~CalDataTableModel()
{

}

int CalDataTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return this->m_timeInterval.size();
}

// 返回列个数
int CalDataTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return CALDATATABLE_COLUMN_HEAD_INDEX::Colunm;
}

// 返回头文本
QVariant CalDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) // 返回一个默认数据的表头
        return QAbstractTableModel::headerData(section, orientation, role);


    if (orientation == Qt::Orientation::Horizontal) {
        return this->m_HorizontalHead[section];
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

// 返回索引数据
QVariant CalDataTableModel::data(const QModelIndex &index, int role) const {
    // 不存在则返回默认
    if (!index.isValid()) return QVariant();

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if (role == Qt::BackgroundColorRole)
    {
        int row = index.row();
        if(row % 2 == 0)
            return QColor(255, 229, 153);
        else
            return QColor(197, 224, 179);
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case CALDATATABLE_COLUMN_HEAD_INDEX::TimeInterval:
                return this->m_timeInterval[index.row()];
                break;
            case CALDATATABLE_COLUMN_HEAD_INDEX::CalRlt:
                return this->m_calRlt[index.row()];
                break;

            default:
                return QVariant();
                break;
        }
    }
    return QVariant();
}

// 返回模型索引
QModelIndex CalDataTableModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    // 行和列的合法性检查
    if (row < 0 || row >= this->m_timeInterval.size() ||
        column < 0 || column >= CALDATATABLE_COLUMN_HEAD_INDEX::Colunm) {
        return QModelIndex();
    }

    switch (column) {
        case CALDATATABLE_COLUMN_HEAD_INDEX::TimeInterval:
            // 新建一个索引并绑定指针数据返回
            return createIndex(row, column);
            break;
        case CALDATATABLE_COLUMN_HEAD_INDEX::CalRlt:
            return createIndex(row, column);
            break;
        default:
            return createIndex(row, column);
            break;
    }
    return createIndex(row, column);
}

// 返回模型索引的编辑方式
Qt::ItemFlags CalDataTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        // 返回用户可以与界面进行交互
        return Qt::ItemIsEnabled;
    }

    switch (index.column()) {
        case CALDATATABLE_COLUMN_HEAD_INDEX::TimeInterval:
            // 返回用户可以与界面进行交互
            return Qt::ItemIsEnabled;
            break;
        case CALDATATABLE_COLUMN_HEAD_INDEX::CalRlt:
            // 返回用户可以进行编辑
            return Qt::ItemIsEnabled;
            break;
        default:
            return Qt::ItemIsEnabled;
            break;
    }
    return Qt::ItemIsEnabled;
}

// 设置模型索引数据
bool CalDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        beginResetModel();
        switch (index.column()) {
            case CALDATATABLE_COLUMN_HEAD_INDEX::TimeInterval:
                this->m_timeInterval[index.row()] = value.toString();
                break;
            case CALDATATABLE_COLUMN_HEAD_INDEX::CalRlt:
                this->m_calRlt[index.row()] = value.toString();
                break;
            default:
                break;
        }
        endResetModel();
        return true;
    }
    return false;
}

// 插入行	参数：插入的位置；插入的行数；父项的模型索引
bool CalDataTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    // 如果插入零行，则返回false，表示插入失败
    if (count == 0) return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    // 按照位置在链表对应位置进行插入数据
    for (int addCount = 0; addCount < count; addCount++) {
//        this->m_timeInterval.append(QString("%1").arg(m_timeInterval));
        this->m_calRlt.append("");
    }
    // 结束插入行
    endInsertRows();
    return true;
}

// 删除行
bool CalDataTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (count == 0) return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    this->m_timeInterval.clear();
    this->m_calRlt.clear();

    endRemoveRows();
    return true;
}

void CalDataTableModel::setHeadDataList(QStringList list)
{
    m_HorizontalHead = list;
}
