#ifndef DB_DATA_H
#define DB_DATA_H

#include <QString>
#include <QDate>

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

struct Lessons
{
    quint64 id;
    QString name;
    quint16 week_day;
    quint16 lesson_number;
};

struct Hometask
{
    quint64 id;
    QString name;
    QDate delivery_day;
    bool completed = false;
};

#endif // DB_DATA_H
