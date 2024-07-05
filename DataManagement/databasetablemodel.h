#ifndef DATABASETABLEMODEL_H
#define DATABASETABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPoint>
#include <QThread>
#include <QFile>

class DataBaseTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit DataBaseTableModel(QSqlDatabase dataBase, QObject *parent = 0);

    void setSql(QString sql);
    //插入数据
    QString insertData();
    //设置sql语句 到 from tableName 结束
    //执行查询语句--是否重新拼接sql语句
    QString queryData();
    //执行查询语句
    bool selectData(QStringList &dateTimeStr, QVector<double> &dateTimes, QVector<double> &dataKeys, QVector<double> &readDatas);
    bool downloadData(QString filePath, QString tableName);
    bool getDataLen(int &len);

    //设置过滤条件
    void setFilter(QString filter);
    //设置sql查询条数
    void setLimitCount(int count);
    //设置排序
    int setOrderBy(QString field);
    //添加数据到 m_orderByFields
    void appOrderByList(QString field,bool isDESC = false);
    //设置left join 的表
    void setJoin(QString tabelName,QString oldFieldName,QString newFieldName);
    //设置除了主表其他表进行相连
    void setOtherJoin(QString tableNameLeft,QString tableNameRight,QString joinFieldName,QString showFieldName,int colIndex);
    //是否已连接
    bool isJoin(QString tableName,QString sql);
    //拼接sql语句
    void jointSql();

    //分页的方法---------------
    //计算页数
    bool calculatePage(QString &rlt);
    //设置第几页
    QString currentPageEdit(int page);
    //获取当前页
    int getCurrentPage();
    //获取总页数
    int getTotalNumPage();
    //获取每页显示行数
    int getRowsPerPage();

signals:
    void returnSelectDataSignal(QList<double>& phaseDiff, QList<double>& freqAccuracy);
    void returnDownLoadDataSignal(QList<double>& phaseDiff, QList<double>& freqAccuracy);
    void updateMsgSignal(const QString msg);

    //拼接sql私有方法
private:
    //拼接left join
    void jointLeftJoin(QString &tmp);
    //拼接ohterLeftJOin
    void jointOtherLeftJoin(QString &tmp);
    //拼接where
    void jointWhere(QString &tmp);
    //拼接OrderBy
    void jointOrderBy(QString &tmp);
    //拼接LIMIT
    void jointLimit(QString &tmp);


private:
    QString m_sql;                      //初始sql
    QString m_startTableName;           //初始的表名，left join 时 用到
    QString m_fullSql;                  //拼接完成的sql

    //Left join 使用到的储存列表
    QStringList m_joinTableNames;       //连接表
    QStringList m_joinOldFieldNames;    //旧列
    QStringList m_joinNewFieldNames;    //新列

    //其它join
    QStringList m_otherJoinTableLeft;   //左，作为连接表
    QStringList m_otherJoinTableRight;  //右，作为被连接表
    QStringList m_joinFieldNames;       //连接的键值，双方名字需要相同
    QStringList m_showFieldNames;       //右，被连接表显示字段
    QList<int> m_colIndexs;             //显示的列索引

    QString m_filter;                   //过滤条件

    QStringList m_orderByFields;        //排序字段集合
    QList<bool> m_isDESCs;              //是否倒序

    //分页的私有成员变量
    QPoint m_limit;                     //查询控制条数  x ：从x - 1开始 , y : 查询条数
    QString m_NotLimitStr;              //不包含limit语句,用于查询数据总条数
    int m_thisPage = 0;                 //当前页
    int m_totalPage = 0;                //总页数

    QSqlDatabase m_pDataBase;

public:
//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    //控制数据居中显示
    QVariant data(const QModelIndex &index, int role) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const;
//    QMap<int, Qt::CheckState> m_checkStateMap;
};


#endif // DATABASETABLEMODEL_H
