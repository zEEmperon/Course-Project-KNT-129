#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <string>
using namespace std;

class Event
{
protected:
    quint64 id;
    quint64 type_event_id;
    QDateTime date;
    QString type;
    QString name;
public:
    Event();
    Event(QDateTime d, QString n, quint64 t_event_id);
    virtual ~Event(){};

    QDateTime getDate(){return date;};
    virtual QString getType() = 0;
    QString getName(){return name;};

    void SetID( quint64 new_id );

    quint64 GetID(){return id;};
    quint64 GetTypeID(){return type_event_id;};

    friend class Schedule;
};

//клас занять
class Study: public Event
{
protected:
    QTime timeBeg;
    QTime timeEnd;
    QString place;
public:
    Study();
    Study(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id);
    ~Study(){};

    QString getType(){return "study";};
    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    QString getPlace(){return place;};

    Study& operator=(const Study &to_copy);

    friend class Schedule;

};

//клас зустрічей
class Meet: public Event
{
protected:
    QTime timeBeg;
    QTime timeEnd;
    QString place;
    QDateTime timeNotification;
public:
    Meet();
    Meet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id);
    ~Meet(){};

    QString getType(){return "meet";};
    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    QString getPlace(){return place;};
    QDateTime getTimeNotific(){return timeNotification;};

    Meet& operator=(const Meet &to_copy);

    friend class Schedule;
};

//клас задач
class Task: public Event
{
protected:
    QDateTime timeDeadline;
    quint16 priority;
    quint64 priority_id;
    QDateTime timeNotification;
    quint64 time_minut=0;

    bool isActive = false;
    QDateTime startAct;
    QDateTime endAct;
public:
    Task();
    Task(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id);
    ~Task(){};
    void Activate();
    void Deactivate();

    QDateTime getTimeDeadline(){return timeDeadline;};
    int getPrior(){return priority;};
    QDateTime getTimeNotific(){return timeNotification;};
    int getWorkTime(){return time_minut;};

    void SetIsActive( bool is_active );
    void SetWorkTime( quint64 new_work_time );

    QString getType(){return "task";};
    quint64 GetPriorityID(){return priority_id;};

    Task& operator=(const Task &to_copy);

    friend class Schedule;
};

//клас днів народжень
class Birthday: public Event
{
protected:
    QDateTime timeNotification;
public:
    Birthday();
    Birthday(QDateTime d, QString n, QDateTime notific, quint64 t_event_id);
    ~Birthday(){};

    QString getType(){return "birthday";};
    QDateTime getTimeNotific(){return timeNotification;};

    Birthday& operator=(const Birthday &to_copy);

    friend class Schedule;
};

#endif // EVENT_H
