#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include "event.h"

class Schedule
{
private:
    //vector <Event* > schedule_arr;
    vector <Study> study_arr;
    vector <Meet> meet_arr;
    vector <Task> task_arr;
    vector <Birthday> bd_arr;
public:
    Schedule(vector<Study> Studies, vector<Meet> Meetings, vector<Task> Tasks, vector<Birthday> BDs);
    ~Schedule();

    //метод "розбиття" загального вектору подій на 4 вектори за класами
    //void FromSchedule();

    //методи додавання подій
    void AddStudy(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id);
    void AddMeet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id);
    void AddTask(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id);
    void AddBD(QDateTime d, QString n, QDateTime notific, quint64 t_event_id);

    //перевірка, чи вільний обраний проміжок часу
    bool isBusy(QDate d, QTime b, QTime e);

    //методи сортування
    void SortStudy(int b, int e); int PartitionStudy(int p, int r);
    void SortMeet(int b, int e); int PartitionMeet(int p, int r);
    void SortTask(int b, int e); int PartitionTask(int p, int r);
    void SortBD(int b, int e); int PartitionBD(int p, int r);

    //сповіщення про події
    void MeetNotific(QDateTime curr_time);
    void TaskNotific(QDateTime curr_time);
    void BDNotific(QDateTime curr_time);

    //методи отримання подій на сьогоднішній день
    void GetStudy(vector<Study>& stud, QDate needed_date);
    void GetMeet(vector<Meet>& meet, QDate needed_date);
    void GetTask(vector<Task>& task);
    void GetBD(vector<Birthday>& bd, QDate needed_date);


    int FindStudy(QString n, QTime time, QDate date);
    int FindMeet(QString n, QTime time, QDate date);
    int FindTask(QString n, QDate tdl);
    int FindBD(QString n);

    //видалення елементів
    quint64 DeleteStudy(int i);
    quint64 DeleteMeet(int i);
    quint64 DeleteTask(int i);
    quint64 DeleteBD(int i);

    Task& ActivateTask(int i);
    Task& DeactivateTask(int i);

};

#endif // SCHEDULE_H
