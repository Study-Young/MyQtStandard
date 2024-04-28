/****************************************************
 *   @projectName    FT6500
 *   @fileName       datahandlemodule.h
 *   @brief          摘要
 *   @author         hongweiYang
 *   @date           2022-08-05
 **************************************************/

#ifndef DATAHANDLEMODULE_H
#define DATAHANDLEMODULE_H

#include <QObject>
#include "datastruct.h"

class DataHandleModule : public QObject
{
    Q_OBJECT
public:
    explicit DataHandleModule(QObject *parent = nullptr);
    ~DataHandleModule();
};

#endif // DATAHANDLEMODULE_H
