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
    Event(QDateTime d, QString n, quint64 t_event_id);
    virtual ~Event(){};
    virtual void Add()=0;
    virtual void Del()=0;

    QDateTime getDate(){return date;};
    QString getType(){return type;};
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
    Study(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id);
    ~Study(){};
    void Add(){};
    void Del(){};
    bool IsFree(){return false;};

    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    QString getPlace(){return place;};

    void Show();

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
    Meet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id);
    ~Meet(){};
    void Add(){};
    void Del(){};
    bool IsFree(){return false;};
    void Notification(){};

    QTime getTimeBeg(){return timeBeg;};
    QTime getTimeEnd(){return timeEnd;};
    QString getPlace(){return place;};
    QDateTime getTimeNotific(){return timeNotification;};

    void Show();

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
    Task(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id);
    ~Task(){};
    void Add(){};
    void Del(){};
    void Notification(){};
    void Activate();
    void Deactivate();

    QDateTime getTimeDeadline(){return timeDeadline;};
    int getPrior(){return priority;};
    QDateTime getTimeNotific(){return timeNotification;};
    int getWorkTime(){return time_minut;};

    void Show();

    void SetIsActive( bool is_active );
    void SetWorkTime( quint64 new_work_time );

    quint64 GetPriorityID(){return priority_id;};

    friend class Schedule;
};

//клас днів народжень
class Birthday: public Event
{
protected:
    QDateTime timeNotification;
public:
    Birthday(QDateTime d, QString n, QDateTime notific, quint64 t_event_id);
    ~Birthday(){};
    void Add(){};
    void Del(){};
    void Notification(){};

    QDateTime getTimeNotific(){return timeNotification;};

    void Show();

    friend class Schedule;
};

#endif // EVENT_H
