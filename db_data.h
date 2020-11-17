#ifndef DB_DATA_H
#define DB_DATA_H

#include <QString>

enum TypeEvents
{
    EVENT_TASK = 1,
    EVENT_BIRTHDAY,
    EVENT_MEET,
    EVENT_STUDY
};

struct PriorityData
{
    quint64 id;
    quint16 weight;
    QString name;
};

struct TypeData
{
    quint64 id;
    QString name;
};

#endif // DB_DATA_H
