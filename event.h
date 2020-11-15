#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <string>
using namespace std;

class Event
{
protected:
    QDate date;
    string type;
    string name;
public:
    Event(QDate d, string n);
    virtual ~Event(){};
    virtual void Add()=0;
    virtual void Del()=0;

    friend class Schedule;

};

//клас занять
class Study: public Event
{
protected:
    QTime timeBeg;
    QTime timeEnd;
    string place;

public:
    Study(QDate d, QTime b, QTime e, string n, string p);
    ~Study(){};
    void Add(){};
    void Del(){};
    bool IsFree(){return false;};

    void Show();

    friend class Schedule;
};

//клас зустрічей
class Meet: public Event
{
protected:
    QTime timeBeg;
    QTime timeEnd;
    string place;
    QTime timeNotification;
public:
    Meet(QDate d, QTime b, QTime e, string n, string p, QTime notific);
    ~Meet(){};
    void Add(){};
    void Del(){};
    bool IsFree(){return false;};
    void Notification(){};

    void Show();

    friend class Schedule;
};

//клас задач
class Task: public Event
{
protected:
    QTime timeDeadline;
    int priority;
    QTime timeNotification;
    int time_minut=0;

    bool isActive = false;
    QDateTime startAct;
    QDateTime endAct;
public:
    Task(QDate ddl, QTime tdl, int p, string n, QTime notific);
    ~Task(){};
    void Add(){};
    void Del(){};
    void Notification(){};
    void Activate();
    void Deactivate();

    void Show();

    friend class Schedule;
};

//клас днів народжень
class Birthday: public Event
{
protected:
    QTime timeNotification;
public:
    Birthday(QDate d, string n, QTime notific);
    ~Birthday(){};
    void Add(){};
    void Del(){};
    void Notification(){};

    void Show();

    friend class Schedule;
};

#endif // EVENT_H
