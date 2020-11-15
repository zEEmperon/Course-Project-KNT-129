#include "schedule.h"
#include <iostream>
using namespace std;

Schedule::Schedule(){}
Schedule::~Schedule(){}

// розподіл подій з загального вектору до векторів за класами
void Schedule::FromSchedule()
{
    int n = int(schedule_arr.size());
    for (int i=0; i<n; i++)
    {
        if (schedule_arr[i]->type == "study"){
            Study* s = (Study*)schedule_arr[i];
            study_arr.push_back(s);
        }
        if (schedule_arr[i]->type == "meet"){
            Meet* m = (Meet*)schedule_arr[i];
            meet_arr.push_back(m);
        }
        if (schedule_arr[i]->type == "task"){
            Task* t = (Task*)schedule_arr[i];
            task_arr.push_back(t);
        }
        if (schedule_arr[i]->type == "birthday"){
            Birthday* bd = (Birthday*)schedule_arr[i];
            bd_arr.push_back(bd);
        }
    }
}

// додавання події
void Schedule::AddStudy(QDate d, QTime b, QTime e, string n, string p)
{
    Study* s = new Study(d, b, e, n, p);
    if (!isBusy(d, b, e))   //сюда и в meet можно исключение
        study_arr.push_back(s);
}
void Schedule::AddMeet(QDate d, QTime b, QTime e, string n, string p, QTime notific)
{
    Meet* m = new Meet(d, b, e, n, p, notific);
    if (!isBusy(d, b, e))
        meet_arr.push_back(m);
}
void Schedule::AddTask(QDate ddl, QTime tdl, int p, string n, QTime notific)
{
    Task* t = new Task(ddl, tdl, p, n, notific);
    task_arr.push_back(t);
}
void Schedule::AddBD(QDate d, string n, QTime notific)
{
    Birthday* b = new Birthday(d, n, notific);
    bd_arr.push_back(b);
}

bool Schedule::isBusy(QDate d, QTime b, QTime e)
{
    int n = int(study_arr.size());
    bool found_busy = false;
    int i = 0;
    while (!found_busy && i<n)
    {
        if ((study_arr[i]->date == d) && ((b>=study_arr[i]->timeBeg && b<=study_arr[i]->timeEnd) || (e>=study_arr[i]->timeBeg && e<=study_arr[i]->timeEnd)))
            found_busy = true;
        i++;
    }
    if (!found_busy)
    {
        n = int(meet_arr.size());
        i = 0;
        while (!found_busy && i<n)
        {
            if ((meet_arr[i]->date == d) && ((b>=meet_arr[i]->timeBeg && b<=meet_arr[i]->timeEnd) || (e>=meet_arr[i]->timeBeg && e<=meet_arr[i]->timeEnd)))
                found_busy = true;
            i++;
        }
    }

    return found_busy;
}

// видалення векторів
void Schedule::Delete()
{
    int n;// = int(schedule_arr.size());
    //for (int i=0; i<n; i++) delete schedule_arr[i];

    n = int(study_arr.size());
    for (int i=0; i<n; i++) delete study_arr[i];

    n = int(meet_arr.size());
    for (int i=0; i<n; i++) delete meet_arr[i];

    n = int(task_arr.size());
    for (int i=0; i<n; i++) delete task_arr[i];

    n = int(bd_arr.size());
    for (int i=0; i<n; i++) delete bd_arr[i];
}

// сортування вектору занять
void Schedule::SortStudy(int b, int e)
{
    int l=b, r=e;
    Study* st = study_arr[(l+r)/2];
    while (l<=r)
    {
        while ((study_arr[l]->date < st->date)||(study_arr[l]->date == st->date && study_arr[l]->timeBeg < st->timeBeg))
            l++;
        while ((study_arr[l]->date > st->date)||(study_arr[l]->date == st->date && study_arr[l]->timeBeg > st->timeBeg))
            r--;
        if (l<=r)
        {
            Study* temp = study_arr[l+1];
            study_arr[l+1] = study_arr[r-1];
            study_arr[r-1] = temp;
        }
        if (b<r)
            SortStudy(b, r);
        if (e>l)
            SortStudy(l, e);
    }
}
void Schedule::SortMeet(int b, int e)
{
    int l=b, r=e;
    Meet* m = meet_arr[(l+r)/2];
    while (l<=r)
    {
        while ((meet_arr[l]->date < m->date)||(meet_arr[l]->date == m->date && meet_arr[l]->timeBeg < m->timeBeg))
            l++;
        while ((meet_arr[l]->date > m->date)||(meet_arr[l]->date == m->date && meet_arr[l]->timeBeg > m->timeBeg))
            r--;
        if (l<=r)
        {
            Meet* temp = meet_arr[l+1];
            meet_arr[l+1] = meet_arr[r-1];
            meet_arr[r-1] = temp;
        }
        if (b<r)
            SortMeet(b, r);
        if (e>l)
            SortMeet(l, e);
    }
}
void Schedule::SortTask(int b, int e)
{
    int l=b, r=e;
    Task* task = task_arr[(l+r)/2];
    while (l<=r)
    {
        while ((task_arr[l]->date < task->date)||
               (task_arr[l]->date == task->date && task_arr[l]->timeDeadline < task->timeDeadline)||
               (task_arr[l]->date == task->date && task_arr[l]->timeDeadline == task->timeDeadline && task[l].priority > task[r].priority))
            l++;
        while ((task_arr[l]->date > task->date)||
               (task_arr[l]->date == task->date && task_arr[l]->timeDeadline > task->timeDeadline)||
               (task_arr[l]->date == task->date && task_arr[l]->timeDeadline == task->timeDeadline && task[l].priority < task[r].priority))
            r--;
        if (l<=r)
        {
            Task* temp = task_arr[l+1];
            task_arr[l+1] = task_arr[r-1];
            task_arr[r-1] = temp;
        }
        if (b<r)
            SortTask(b, r);
        if (e>l)
            SortTask(l, e);
    }
}
void Schedule::SortBD(int b, int e)
{
    int l=b, r=e;
    Birthday* m = bd_arr[(l+r)/2];
    while (l<=r)
    {
        while (bd_arr[l]->date < m->date)
            l++;
        while (bd_arr[r]->date > m->date)
            r--;
        if (l<=r)
        {
            Birthday* temp = bd_arr[l+1];
            bd_arr[l+1] = bd_arr[r-1];
            bd_arr[r-1] = temp;
        }
        if (b<r)
            SortBD(b, r);
        if (e>l)
            SortBD(l, e);
    }
}


void Schedule::MeetNotific(QTime curr_time)
{
    int n = int(meet_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((meet_arr[i]->date == curr_date || (meet_arr[i]->date).addDays(-3) == curr_date) && meet_arr[i]->timeNotification == curr_time)
            cout << "Notification";
}
void Schedule::TaskNotific(QTime curr_time)
{
    int n = int(task_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((task_arr[i]->date == curr_date || (task_arr[i]->date).addDays(-3) == curr_date) && task_arr[i]->timeNotification == curr_time)
            cout << "Notification";
}
void Schedule::BDNotific(QTime curr_time)
{
    int n = int(bd_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((bd_arr[i]->date == curr_date || (bd_arr[i]->date).addDays(-3) == curr_date) && bd_arr[i]->timeNotification == curr_time)
            cout << "Notification";
}

void Schedule::ChangeStudy(int i, QDate d, QTime b, QTime e, string n, string p)
{
    study_arr[i]->date = d;
    study_arr[i]->timeBeg = b;
    study_arr[i]->timeEnd = e;
    study_arr[i]->name = n;
    study_arr[i]->place = p;
}
void Schedule::ChangeMeet(int i, QDate d, QTime b, QTime e, string n, string p, QTime notific)
{
    meet_arr[i]->date = d;
    meet_arr[i]->timeBeg = b;
    meet_arr[i]->timeEnd = e;
    meet_arr[i]->name = n;
    meet_arr[i]->place = p;
    meet_arr[i]->timeNotification = notific;
}
void Schedule::ChangeTask(int i, QDate ddl, QTime tdl, int p, string n, QTime notific)
{
    task_arr[i]->date = ddl;
    task_arr[i]->timeDeadline = tdl;
    task_arr[i]->priority = p;
    task_arr[i]->name = n;
    task_arr[i]->timeNotification = notific;
}
void Schedule::ChangeBD(int i, QDate d, string n, QTime notific)
{
    bd_arr[i]->date = d;
    bd_arr[i]->name = n;
    bd_arr[i]->timeNotification = notific;
}
