#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include "event.h"

class Schedule
{
private:
    vector <Event* > schedule_arr;
    vector <Study* > study_arr;
    vector <Meet* > meet_arr;
    vector <Task* > task_arr;
    vector <Birthday* > bd_arr;
public:
    Schedule();
    ~Schedule();

    //метод "розбиття" загального вектору подій на 4 вектори за класами
    void FromSchedule();

    //методи додавання подій
    void AddStudy(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id);
    void AddMeet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id);
    void AddTask(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id);
    void AddBD(QDateTime d, QString n, QDateTime notific, quint64 t_event_id);

    //перевірка, чи вільний обраний проміжок часу
    bool isBusy(QDate d, QTime b, QTime e);

    //видалення елементів
    void Delete();

    //методи сортування
    void SortStudy(int b, int e);
    void SortMeet(int b, int e);
    void SortTask(int b, int e);
    void SortBD(int b, int e);

    //сповіщення про події
    void MeetNotific(QDateTime curr_time);
    void TaskNotific(QDateTime curr_time);
    void BDNotific(QDateTime curr_time);

    //методи редагування подій
    void ChangeStudy(int i, QDateTime d, QTime b, QTime e, QString n, QString p);
    void ChangeMeet(int i, QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific);
    void ChangeTask(int i, QDateTime tdl, quint16 p, QString n, QDateTime notific);
    void ChangeBD(int i, QDateTime d, QString n, QDateTime notific);

};

#endif // SCHEDULE_H
