#include "event.h"
#include <QDate>

#include <QDebug>
#include<iostream>
using namespace std;

Event::Event(QDate d, string n)
{
    date = d;
    type = "event";
    name = n;
}

// Study

Study::Study(QDate d, QTime b, QTime e, string n, string p): Event(d, n)
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
    cout << name << " in " << place << endl;
    out << dt << " " << timeBeg.toString() << " " << timeEnd.toString() << endl;
}

// Meet

Meet::Meet(QDate d, QTime b, QTime e, string n, string p, QTime notific): Event(d, n)
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
    cout << name << " in " << place << endl;
    out << date.toString("yyyy.MM.dd") << " " << timeBeg.toString() << " " << timeEnd.toString() << " " << timeNotification.toString() << endl;
}

// Task

Task::Task(QDate ddl, QTime tdl, int p, string n, QTime notific): Event(ddl, n)
{
    timeDeadline = tdl;
    priority = p;
    timeNotification = notific;
    type = "task";
}


void Task::Show()
{
    QTextStream out(stdout);
    cout << "The " << name << " with " << priority << " priority" << endl;
    out << date.toString("yyyy.MM.dd") << " " << timeDeadline.toString() << " " << timeNotification.toString() << endl;
}

//методи обліку часу роботи над завданнями
void Task::Activate()
{
    isActive = true;
    startAct = QDateTime::currentDateTime();
}

void Task::Deactivate()
{
    if(isActive){ //сюда можно исключение, если задача не активирована
        isActive = false;
        endAct = QDateTime::currentDateTime();
        quint64 work_time = startAct.daysTo(endAct)*1000*60*60*24 + startAct.time().msecsTo(endAct.time());
        time_minut += work_time/60000;
    }
}

// Birthday

Birthday::Birthday(QDate d, string n, QTime notific): Event(d, n)
{
    timeNotification = notific;
    type = "birthday";
}


void Birthday::Show()
{
    QTextStream out(stdout);
    cout << "The " << name << "'s birthday!" << endl;
    out << date.toString("yyyy.MM.dd") << " " << " " << timeNotification.toString() << endl;
}
