#include "event.h"
#include <QDate>

#include <QDebug>
#include<iostream>
using namespace std;

Event::Event(){}
Event::Event(QDateTime d, QString n, quint64 t_event_id)
{
    id = 0;
    type_event_id = t_event_id;
    date = d;
    type = "event";
    name = n;
}

// Study
Study::Study(): Event(){}
Study::Study(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id): Event(d, n, t_event_id)
{
    timeBeg = b;
    timeEnd = e;
    type = "study";
    place = p;
}

void Study::Show()
{
    QTextStream out(stdout);
    QString dt(date.toString("yyyy.MM.dd"));
    cout << name.toStdString() << " in " << place.toStdString() << endl;
    out << dt << " " << timeBeg.toString() << " " << timeEnd.toString() << endl;
}

Study& Study::operator=(const Study &to_copy)
{
    if (this == &to_copy)
        return *this;

    id = to_copy.id;
    type_event_id = to_copy.type_event_id;
    date = to_copy.date;
    type = to_copy.type;
    name = to_copy.name;
    timeBeg = to_copy.timeBeg;
    timeEnd = to_copy.timeEnd;
    place = to_copy.place;

    return *this;
}

// Meet
Meet::Meet(): Event(){}
Meet::Meet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id): Event(d, n, t_event_id)
{
    timeBeg = b;
    timeEnd = e;
    timeNotification = notific;
    type = "meet";
    place = p;
}

void Meet::Show()
{
    QTextStream out(stdout);
    cout << name.toStdString() << " in " << place.toStdString() << endl;
    out << date.toString("yyyy.MM.dd") << " " << timeBeg.toString() << " " << timeEnd.toString() << " " << timeNotification.toString() << endl;
}

Meet& Meet::operator=(const Meet &to_copy)
{
    if (this == &to_copy)
        return *this;

    id = to_copy.id;
    type_event_id = to_copy.type_event_id;
    date = to_copy.date;
    type = to_copy.type;
    name = to_copy.name;
    timeBeg = to_copy.timeBeg;
    timeEnd = to_copy.timeEnd;
    place = to_copy.place;
    timeNotification = to_copy.timeNotification;

    return *this;
}

// Task
Task::Task(): Event(){}
Task::Task(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id): Event(d, n, t_event_id)
{
    timeDeadline = tdl;
    priority = p;
    priority_id = p_id;
    timeNotification = notific;
    type = "task";
}


void Task::Show()
{
    QTextStream out(stdout);
    cout << "The " << name.toStdString() << " with " << priority << " priority" << endl;
    out << date.toString("yyyy.MM.dd") << " " << timeDeadline.toString() << " " << timeNotification.toString() << endl;
}

Task& Task::operator=(const Task &to_copy)
{
    if (this == &to_copy)
        return *this;

    id = to_copy.id;
    type_event_id = to_copy.type_event_id;
    date = to_copy.date;
    type = to_copy.type;
    name = to_copy.name;
    timeDeadline = to_copy.timeDeadline;
    priority = to_copy.priority;
    priority_id = to_copy.priority_id;
    timeNotification = to_copy.timeNotification;
    time_minut = to_copy.time_minut;
    isActive = to_copy.isActive;
    startAct = to_copy.startAct;
    endAct = to_copy.endAct;

    return *this;
}

//методи обліку часу роботи над завданнями
void Task::Activate()
{
    if(!isActive){ //сюда можно исключение, если задача не активирована
        isActive = true;
        startAct = QDateTime::currentDateTime();
        date = QDateTime::currentDateTime();
    }
}

void Task::Deactivate()
{
    if(isActive){ //сюда можно исключение, если задача не активирована
        isActive = false;
        startAct = date;
        endAct = QDateTime::currentDateTime();
        quint64 work_time = startAct.daysTo(endAct)*1000*60*60*24 + startAct.time().msecsTo(endAct.time());
        time_minut += work_time/60000;
        date = QDateTime();
    }
}

// Birthday
Birthday::Birthday(): Event(){}
Birthday::Birthday(QDateTime d, QString n, QDateTime notific, quint64 t_event_id): Event(d, n, t_event_id)
{
    timeNotification = notific;
    type = "birthday";
}


void Birthday::Show()
{
    QTextStream out(stdout);
    cout << "The " << name.toStdString() << "'s birthday!" << endl;
    out << date.toString("yyyy.MM.dd") << " " << " " << timeNotification.toString() << endl;
}

Birthday& Birthday::operator=(const Birthday &to_copy)
{
    if (this == &to_copy)
        return *this;

    id = to_copy.id;
    type_event_id = to_copy.type_event_id;
    date = to_copy.date;
    type = to_copy.type;
    name = to_copy.name;
    timeNotification = to_copy.timeNotification;

    return *this;
}

//----------------------------------------------------------------------------
void Event::SetID( quint64 new_id )
{
    id = new_id;
}

//----------------------------------------------------------------------------
void Task::SetIsActive( bool is_active )
{
    if( is_active )
    {
        isActive = true;
        startAct = date;
    }
    else
    {
        date = QDateTime();
    }
}

//----------------------------------------------------------------------------
void Task::SetWorkTime( quint64 new_work_time )
{
    time_minut = new_work_time;
}
