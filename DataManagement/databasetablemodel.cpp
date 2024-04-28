#include "databasetablemodel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

DataBaseTableModel::DataBaseTableModel(QSqlDatabase dataBase, QObject *parent)
    : m_pDataBase(dataBase),
      QSqlQueryModel(parent)
{
    //由于用到了QPoint来存放limit两个常量，在这里初始化一下
    m_limit.setX(0);
    m_limit.setY(0);
}

/**
 * @brief customQSqlQueryModel::setSql
 * @param sql 初始sql语句
 * 功能：设置sql语句，获取表名
 */
void DataBaseTableModel::setSql(QString sql)
{
    m_sql = sql;
    //获取查询的表格名
    m_startTableName = m_sql.split(" ",QString::SkipEmptyParts).last();
}

/**
 * @brief customQSqlQueryModel::insertData
 * @return 插入成功、失败
 * 功能：插入数据
 */
QString DataBaseTableModel::insertData()
{
    QSqlQuery query(m_pDataBase);

    query.prepare(m_sql);

    if(query.exec())
    {
        return QString("成功！");
    }
    else
    {
        return QString("失败！%1").arg(query.lastError().text());
    }
}

/**
 * @brief customQSqlQueryModel::queryData
 * @param queryAgain 设置 是否从新拼接参数
 * 功能：设置是否从新拼接参数，并执行查询
 */
QString DataBaseTableModel::queryData()
{
    jointSql();

    this->setQuery(m_fullSql, m_pDataBase);
    //计算页数
    QString rlt;
    if(calculatePage(rlt))
    {
        return QString("成功！");
    }
    else
    {
        return QString("失败！%1").arg(rlt);
    }
}

/**
 * @brief customQSqlQueryModel::selectData
 * 功能：执行sql查询语句
 */
bool DataBaseTableModel::selectData(QStringList &dateTimeStr, QVector<double> &dateTimes, QVector<double> &dataKeys, QVector<double> &timeDiffs)
{
    QSqlQuery query(m_pDataBase);

    query.prepare(m_sql);

    bool flag = false;
    if(query.exec())
    {
        while(query.next())
        {
            dataKeys.append(query.value(0).toDouble());
            dateTimeStr.append(query.value(1).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            dateTimes.append(query.value(1).toDateTime().toSecsSinceEpoch());
            timeDiffs.append(query.value(2).toDouble());
            flag = true;
        }
    }
    return flag;
}

bool DataBaseTableModel::getDataLen(int &len)
{
    QSqlQuery query(m_pDataBase);

    query.prepare(m_sql);

    bool flag = false;
    if(query.exec())
    {
        while(query.next())
        {
            len = query.value(0).toInt();
            flag = true;
        } 
    }
    return flag;
}

/**
 * @brief customQSqlQueryModel::setFilter
 * @param filter          条件sql语句，不包含where，请自己注意sql语句的空格
 * @param filterValues    条件语句的 值
 * 功能：设置条件语句
 */
void DataBaseTableModel::setFilter(QString filter)
{
    m_filter = filter;
}

/**
 * @brief customQSqlQueryModel::setLimit
 * @param count     读取的总数
 * 功能：设置LIMIT 第二个参数，并将当前页初始化为第一页
 */
void DataBaseTableModel::setLimitCount(int count)
{
    m_limit.setY(count);
    m_thisPage = 1;
}

/**
 * @brief customQSqlQueryModel::setOrderBy
 * @param field 列名
 * @return   1 ： 升序  ， 2 ：降序  ， 0 ：不排序
 * 功能：通过列名进行排序，排序顺序 升序--》降序--》不排序
 */
int DataBaseTableModel::setOrderBy(QString field)
{
    int index = m_orderByFields.indexOf(field);

    if(-1 != index)
    {
        //存在,判断是否降序
        if(m_isDESCs[index])
        {
            //降序，将降序--》不排序:将列名和是否降序从列表中删除
            m_orderByFields.removeAt(index);
            m_isDESCs.removeAt(index);
            return 0;
        }
        else
        {
            //升序，将升序--》降序
            m_isDESCs[index] = true;
            return 2;
        }
    }
    else
    {
        //不存在,添加
        appOrderByList(field);
        return 1;
    }
}

/**
 * @brief customQSqlQueryModel::appOrderByList
 * @param field         排序字段
 * @param isDESC        是否倒叙，true倒序
 * 功能：设置order by 参数,添加进入容器，升序
 */
void DataBaseTableModel::appOrderByList(QString field, bool isDESC)
{
    m_orderByFields.append(field);
    m_isDESCs.append(isDESC);
}

/**
 * @brief customQSqlQueryModel::setJoin
 * @param tableName         连接的表
 * @param oldFieldName      旧的列名，列名一定是外键列
 * @param newFieldName      新的列名,外键表的某个字段
 *
 * 功能：添加 LEFT JOIN 需要的参数
 *
 */
void DataBaseTableModel::setJoin(QString tableName, QString oldFieldName,QString newFieldName)
{
    if(tableName.isEmpty() || oldFieldName.isEmpty() || newFieldName.isEmpty())
    {
        return;
    }

    //当存在一列相同的不添加
    if(m_joinTableNames.contains(tableName))
    {
        if(m_joinOldFieldNames.contains(oldFieldName))
        {
            return;
        }
    }

    m_joinTableNames.append(tableName);
    m_joinOldFieldNames.append(oldFieldName);
    m_joinNewFieldNames.append(newFieldName);
}

/**
 * @brief customQSqlQueryModel::setOtherJoin
 * @param tableNameLeft     连接表
 * @param tableNameRight    被连接表
 * @param joinFieldName     连接键值
 * @param showFieldName     显示字段
 */
void DataBaseTableModel::setOtherJoin(QString tableNameLeft, QString tableNameRight, QString joinFieldName, QString showFieldName,int colIndex)
{
    if(tableNameLeft.isEmpty() || tableNameRight.isEmpty() || joinFieldName.isEmpty() || showFieldName.isEmpty() || colIndex < 0)
    {
        return;
    }
    m_otherJoinTableLeft.append(tableNameLeft);
    m_otherJoinTableRight.append(tableNameRight);
    m_joinFieldNames.append(joinFieldName);
    m_showFieldNames.append(showFieldName);
    m_colIndexs.append(colIndex);
}

/**
 * @brief customQSqlQueryModel::isJoin
 * @param tabelName  要连接的表名
 * @return  true：以连接过了
 */
bool DataBaseTableModel::isJoin(QString tableName,QString sql)
{
    QStringList splitList = sql.split(" ");

    for(int i = 0; i < splitList.size(); i++)
    {
        if(splitList[i].compare("LEFT") == 0)
        {
            if(tableName.compare(splitList[i+2]) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief customQSqlQueryModel::jointSql
 * 功能：拼接完整的sql语句
 */
void DataBaseTableModel::jointSql()
{
    QString tmp = "";

    tmp.append(m_sql);

    jointLeftJoin(tmp);

    jointOtherLeftJoin(tmp);

    jointWhere(tmp);

    jointOrderBy(tmp);

    //用于查询数据总条数的 m_NotLimitStr
    m_NotLimitStr = tmp;

    jointLimit(tmp);

    m_fullSql = tmp;
}


/**
 * @brief customQSqlQueryModel::calculatePage
 * 功能：计算页数，总页数
 */
bool DataBaseTableModel::calculatePage(QString &rlt)
{
    QSqlQuery query(m_pDataBase);

    query.prepare(m_NotLimitStr);

    if(query.exec())
    {
        int rowCount = 0;
        while(query.next())
        {
            rowCount++;
        }
        int tmpTotal = rowCount / m_limit.y();

        //不能取余，加一页
        if(rowCount % m_limit.y() != 0)
        {
            tmpTotal++;
        }

        m_totalPage = tmpTotal;

        return true;
    }

    m_totalPage = 0;
    rlt = query.lastError().text();
    return false;
}

/**
 * @brief customQSqlQueryModel::currentPageEdit
 * @param page 第几页
 * 功能：跳转至page页
 */
QString DataBaseTableModel::currentPageEdit(int page)
{
    if(page > m_totalPage || page <= 0)
    {
        return QString("页码异常!");
    }

    m_thisPage = page;

    --page;

    int count = m_limit.y();

    m_limit.setX(page * count);

    return queryData();
}

/**
 * @brief customQSqlQueryModel::getCurrentPage
 * @return 当前页
 */
int DataBaseTableModel::getCurrentPage()
{
    return m_thisPage;
}

/**
 * @brief customQSqlQueryModel::getTotalNumPage
 * @return 总页数
 */
int DataBaseTableModel::getTotalNumPage()
{
    return m_totalPage;
}

/**
 * @brief customQSqlQueryModel::getTotalNumPage
 * @return 每页显示行数
 */
int DataBaseTableModel::getRowsPerPage()
{
    return m_limit.y();
}

/**
 * @brief customQSqlQueryModel::jointLeftJoin
 * @param tmp
 * 功能：拼接连接
 */
void DataBaseTableModel::jointLeftJoin(QString &tmp)
{
    //left join
    if(!m_joinTableNames.isEmpty())
    {
        for(int i = 0; i < m_joinTableNames.size(); i ++)
        {
            //替换查询列
            QString tempNewFieldName = QString("%1.%2").arg(m_joinTableNames[i]).arg(m_joinNewFieldNames[i]);

            //将字段替换
            tmp.replace(m_joinOldFieldNames[i],tempNewFieldName);

            //当第一次连接就拼接
            if(false == isJoin(m_joinTableNames[i],tmp))
            {
                QString joinStr = QString(" LEFT JOIN %1 ON %2.%3 = %4.%5 ").arg(m_joinTableNames[i]).arg(m_startTableName)
                        .arg(m_joinOldFieldNames[i]).arg(m_joinTableNames[i]).arg(m_joinOldFieldNames[i]);

                tmp.append(joinStr);
            }
        }
    }
}

/**
 * @brief customQSqlQueryModel::jointOtherLeftJoin
 * @param tmp
 * 功能：拼接其它连接
 */
void DataBaseTableModel::jointOtherLeftJoin(QString &tmp)
{
    //其它 left join
    if(!m_otherJoinTableRight.isEmpty())
    {
        for(int i = 0; i < m_otherJoinTableRight.size(); i++)
        {
            //拼接显示列名
            QString showFieldName = QString(" %1.%2 ").arg(m_otherJoinTableRight[i]).arg(m_showFieldNames[i]);

            //将显示列名添加到tmp
            QStringList list = tmp.split("from");

            QStringList colList = list[0].split(",");

            QString appTem;

            for(int j = 0; j < colList.size(); j++)
            {
                if(j == m_colIndexs[i] - 1)
                {
                    appTem.append(showFieldName);
                    appTem.append(",");
                }

                appTem.append(colList[j]);

                if(j + 1 != colList.size())
                {
                    appTem.append(",");
                }
            }

            tmp = QString("%1 %2 %3").arg(appTem).arg("from").arg(list[1]);

            if(false == isJoin(m_otherJoinTableRight[i],tmp))
            {
                QString joinStr = QString(" LEFT JOIN %1 ON %2.%3 = %4.%5 ").arg(m_otherJoinTableRight[i]).arg(m_otherJoinTableLeft[i])
                        .arg(m_joinFieldNames[i]).arg(m_otherJoinTableRight[i]).arg(m_joinFieldNames[i]);

                tmp.append(joinStr);
            }
        }
    }
}

/**
 * @brief customQSqlQueryModel::jointWhere
 * @param tmp
 * 功能：拼接where
 */
void DataBaseTableModel::jointWhere(QString &tmp)
{
    //where
    if(!m_filter.isEmpty())
    {
        tmp.append(m_filter);
    }
}

/**
 * @brief customQSqlQueryModel::jointOrderBy
 * @param tmp
 * 功能：拼接排序参数
 */
void DataBaseTableModel::jointOrderBy(QString &tmp)
{
    //orderby
    if(!m_orderByFields.isEmpty())
    {
        QString orderTmp = " ORDER BY ";

        for(int i = 0; i < m_orderByFields.size(); i++)
        {
            orderTmp.append(m_orderByFields[i]);

            orderTmp.append(QString(" ").append(m_isDESCs[i] ? "DESC" : "ASC"));

            if(i + 1 != m_orderByFields.size())
            {
                orderTmp.append(",");
            }
        }

        tmp.append(orderTmp);
    }

}

/**
 * @brief customQSqlQueryModel::jointLimit
 * @param tmp
 * 功能：拼接limit参数
 */
void DataBaseTableModel::jointLimit(QString &tmp)
{
    //QPoint.isNill() 当x和y为0，返回true
    if(!m_limit.isNull())
    {
        tmp.append(QString(" LIMIT %1 , %2 ").arg(m_limit.x()).arg(m_limit.y()));
    }
}

/*
bool DataBaseTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role == Qt::CheckStateRole && index.column() == 0)
        m_checkStateMap[index.row()] = (value.toInt() == Qt::Checked ? Qt::Checked : Qt::Unchecked);

    return true;
}
*/

//重写data方法，主要使文字居中显示
QVariant DataBaseTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QVariant value = QSqlQueryModel::data(index,role);

    //单元格内容居中
    if(role == Qt::TextAlignmentRole)
    {
        value = Qt::AlignCenter;
    }
    /*
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0:
                return this->m_timeInterval[index.row()];
                break;
            case 1:
                return this->m_calRlt[index.row()];
                break;

            default:
                return QVariant();
                break;
        }
    }

    if(index.column() != 0)
        return value;

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
            return m_checkStateMap[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
        return Qt::Unchecked;
    }
    */

    return value;
}

/*
Qt::ItemFlags DataBaseTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    if(index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
*/
