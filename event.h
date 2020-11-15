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

    QDate getDate(){return date;};
    string getType(){return type;};
    string getName(){return name;};

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

    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    string getPlace(){return place;};

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

    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    string getPlace(){return place;};
    QTime getTimeNotific(){return timeNotification;};

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

    QTime getTimeDeadline(){return timeDeadline;};
    int getPrior(){return priority;};
    QTime getTimeNotific(){return timeNotification;};
    int getWorkTime(){return time_minut;};

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

    QTime getTimeNotific(){return timeNotification;};

    void Show();

    friend class Schedule;
};

#endif // EVENT_H
