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
    void AddStudy(QDate d, QTime b, QTime e, string n, string p);
    void AddMeet(QDate d, QTime b, QTime e, string n, string p, QTime notific);
    void AddTask(QDate ddl, QTime tdl, int p, string n, QTime notific);
    void AddBD(QDate d, string n, QTime notific);

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
    void MeetNotific(QTime curr_time);
    void TaskNotific(QTime curr_time);
    void BDNotific(QTime curr_time);

    //методи редагування подій
    void ChangeStudy(int i, QDate d, QTime b, QTime e, string n, string p);
    void ChangeMeet(int i, QDate d, QTime b, QTime e, string n, string p, QTime notific);
    void ChangeTask(int i, QDate ddl, QTime tdl, int p, string n, QTime notific);
    void ChangeBD(int i, QDate d, string n, QTime notific);

};

#endif // SCHEDULE_H
