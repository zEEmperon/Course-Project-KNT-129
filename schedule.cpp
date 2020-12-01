#include "schedule.h"
#include <iostream>
using namespace std;

Schedule::Schedule(vector<Study> Studies, vector<Meet> Meetings, vector<Task> Tasks, vector<Birthday> BDs)
{
    study_arr.resize(Studies.size());
    meet_arr.resize(Meetings.size());
    task_arr.resize(Tasks.size());
    bd_arr.resize(BDs.size());

    for (int i=0; i<int(study_arr.size()); i++)
        study_arr[i] = Studies[i];
    for (int i=0; i<int(meet_arr.size()); i++)
        meet_arr[i] = Meetings[i];
    for (int i=0; i<int(task_arr.size()); i++)
        task_arr[i] = Tasks[i];
    for (int i=0; i<int(bd_arr.size()); i++)
        bd_arr[i] = BDs[i];
}
Schedule::~Schedule(){}

// додавання події
void Schedule::AddStudy(QDateTime d, QTime b, QTime e, QString n, QString p, quint64 t_event_id)
{
    Study s(d, b, e, n, p, t_event_id);
    if (!isBusy(d.date(), b, e))   //сюда и в meet можно исключение
        study_arr.push_back(s);
}
void Schedule::AddMeet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id)
{
    Meet m(d, b, e, n, p, notific, t_event_id);
    if (!isBusy(d.date(), b, e))
        meet_arr.push_back(m);
}
void Schedule::AddTask(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id)
{
    Task t(d, tdl, p, n, notific, t_event_id, p_id);
    task_arr.push_back(t);
}
void Schedule::AddBD(QDateTime d, QString n, QDateTime notific, quint64 t_event_id)
{
    Birthday b(d, n, notific, t_event_id);
    bd_arr.push_back(b);
}

bool Schedule::isBusy(QDate d, QTime b, QTime e)
{
    int n = int(study_arr.size());
    bool found_busy = false;
    int i = 0;
    while (!found_busy && i<n)
    {
        if ((study_arr[i].date.date() == d) && ((b>=study_arr[i].timeBeg && b<=study_arr[i].timeEnd) || (e>=study_arr[i].timeBeg && e<=study_arr[i].timeEnd)))
            found_busy = true;
        i++;
    }
    if (!found_busy)
    {
        n = int(meet_arr.size());
        i = 0;
        while (!found_busy && i<n)
        {
            if ((meet_arr[i].date.date() == d) && ((b>=meet_arr[i].timeBeg && b<=meet_arr[i].timeEnd) || (e>=meet_arr[i].timeBeg && e<=meet_arr[i].timeEnd)))
                found_busy = true;
            i++;
        }
    }

    return found_busy;
}

// сортування вектору занять
void Schedule::SortStudy(int b, int e)
{
    int l=b, r=e;
    Study st = study_arr[(l+r)/2];
    while (l<=r)
    {
        while ((study_arr[l].date < st.date)||(study_arr[l].date == st.date && study_arr[l].timeBeg < st.timeBeg))
            l++;
        while ((study_arr[l].date > st.date)||(study_arr[l].date == st.date && study_arr[l].timeBeg > st.timeBeg))
            r--;
        if (l<=r)
        {
            Study temp = study_arr[l+1];
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
    Meet m = meet_arr[(l+r)/2];
    while (l<=r)
    {
        while ((meet_arr[l].date < m.date)||(meet_arr[l].date == m.date && meet_arr[l].timeBeg < m.timeBeg))
            l++;
        while ((meet_arr[l].date > m.date)||(meet_arr[l].date == m.date && meet_arr[l].timeBeg > m.timeBeg))
            r--;
        if (l<=r)
        {
            Meet temp = meet_arr[l+1];
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
    Task task = task_arr[(l+r)/2];
    while (l<=r)
    {
        while ((task_arr[l].date < task.date)||
               (task_arr[l].date == task.date && task_arr[l].timeDeadline < task.timeDeadline)||
               (task_arr[l].date == task.date && task_arr[l].timeDeadline == task.timeDeadline && task_arr[l].priority > task_arr[r].priority))
            l++;
        while ((task_arr[l].date > task.date)||
               (task_arr[l].date == task.date && task_arr[l].timeDeadline > task.timeDeadline)||
               (task_arr[l].date == task.date && task_arr[l].timeDeadline == task.timeDeadline && task_arr[l].priority < task_arr[r].priority))
            r--;
        if (l<=r)
        {
            Task temp = task_arr[l+1];
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
    Birthday m = bd_arr[(l+r)/2];
    while (l<=r)
    {
        while (bd_arr[l].date.date() < m.date.date())
            l++;
        while (bd_arr[r].date.date() > m.date.date())
            r--;
        if (l<=r)
        {
            Birthday temp = bd_arr[l+1];
            bd_arr[l+1] = bd_arr[r-1];
            bd_arr[r-1] = temp;
        }
        if (b<r)
            SortBD(b, r);
        if (e>l)
            SortBD(l, e);
    }
}


void Schedule::MeetNotific(QDateTime curr_time)
{
    int n = int(meet_arr.size());
    QDate curr_date = QDate::currentDate();//QDateTime::currentDateTime();//
    for (int i=0; i<n; i++)
        if ((meet_arr[i].date.date() == curr_date || (meet_arr[i].date.date()).addDays(-3) == curr_date) && meet_arr[i].timeNotification == curr_time)
            cout << "Notification";
}
void Schedule::TaskNotific(QDateTime curr_time)
{
    int n = int(task_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((task_arr[i].date.date() == curr_date || (task_arr[i].date.date()).addDays(-3) == curr_date) && task_arr[i].timeNotification == curr_time)
            cout << "Notification";
}
void Schedule::BDNotific(QDateTime curr_time)
{
    int n = int(bd_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((bd_arr[i].date.date() == curr_date || (bd_arr[i].date.date()).addDays(-3) == curr_date) && bd_arr[i].timeNotification == curr_time)
            cout << "Notification";
}

void Schedule::GetStudy(vector<Study>& stud, QDate needed_date)
{
    stud.resize(0);

    QDateTime start_interval(needed_date, QTime(0, 0, 0));
    QDateTime end_interval(needed_date, QTime(23, 59, 59));

    for (int i=0; i<int(study_arr.size()); i++)
        if ((study_arr[i].getDate() >= start_interval) && (study_arr[i].getDate() <= end_interval))
            stud.push_back(study_arr[i]);
}
void Schedule::GetMeet(vector<Meet>& meet, QDate needed_date)
{
    meet.resize(0);

    QDateTime start_interval(needed_date, QTime(0, 0, 0));
    QDateTime end_interval(needed_date, QTime(23, 59, 59));

    for (int i=0; i<int(meet_arr.size()); i++)
        //if ((meet_arr[i]->date).date() == needed_date)
        if ((meet_arr[i].getDate() >= start_interval) && (meet_arr[i].getDate() <= end_interval))
            meet.push_back(meet_arr[i]);
}
void Schedule::GetTask(vector<Task>& task)
{
    task.resize(0);

    //QDateTime start_interval(needed_date, QTime(0, 0, 0));
    //QDateTime end_interval(needed_date, QTime(23, 59, 59));

    for (int i=0; i<int(task_arr.size()); i++)
        //if ((task_arr[i]->date).date() == needed_date)
        //if ((task_arr[i].getDate() >= start_interval) && (task_arr[i].getDate() <= end_interval))
            task.push_back(task_arr[i]);
}
void Schedule::GetBD(vector<Birthday>& bd, QDate needed_date)
{
    bd.resize(0);

    QDateTime start_interval(needed_date, QTime(0, 0, 0));
    QDateTime end_interval(needed_date, QTime(23, 59, 59));

    for (int i=0; i<int(bd_arr.size()); i++)
        //if ((bd_arr[i]->date).date() == needed_date)
        if ((bd_arr[i].getDate() >= start_interval) && (bd_arr[i].getDate() <= end_interval))
            bd.push_back(bd_arr[i]);
}

int Schedule::FindStudy(QString n, QTime time, QDate date)
{
    int i = 0;
    bool found = false;
    while (!found && i<int(study_arr.size()))
    {
        if (study_arr[i].name == n && (study_arr[i].timeBeg.hour() == time.hour() && study_arr[i].timeBeg.minute() == time.minute()) && study_arr[i].date.date() == date)
            found = true;
        i++;
    }
    i--;
    return i;
}

int Schedule::FindMeet(QString n, QTime time, QDate date)
{
    int i = 0;
    bool found = false;
    while (!found && i<int(meet_arr.size()))
    {
        if (meet_arr[i].name == n && (meet_arr[i].timeBeg.hour() == time.hour() && meet_arr[i].timeBeg.minute() == time.minute()) && meet_arr[i].date.date() == date)
            found = true;
        i++;
    }
    i--;
    return i;
}

int Schedule::FindTask(QString n, QDate tdl)
{
    int i = 0;
    bool found = false;
    while (!found && i<int(task_arr.size()))
    {
        if (task_arr[i].name == n && task_arr[i].timeDeadline.date() == tdl)
            found = true;
        i++;
    }
    i--;
    return i;
}

int Schedule::FindBD(QString n)
{
    int i = 0;
    bool found = false;
    while (!found && i<int(bd_arr.size()))
    {
        if (bd_arr[i].name == n)
            found = true;
        i++;
    }
    i--;
    return i;
}

quint64 Schedule::DeleteStudy(int i)
{
    quint64 id = study_arr[i].GetID();
    if (i == int(study_arr.size())-1) study_arr.pop_back();
    for (int j=i; j<int(study_arr.size())-1; j++)
        study_arr[j] = study_arr[j+1];
    study_arr.pop_back();
    return id;
}
quint64 Schedule::DeleteMeet(int i)
{
    quint64 id = meet_arr[i].GetID();
    if (i == int(meet_arr.size())-1) meet_arr.pop_back();
    for (int j=i; j<int(meet_arr.size())-1; j++)
        meet_arr[j] = meet_arr[j+1];
    meet_arr.pop_back();
    return id;
}
quint64 Schedule::DeleteTask(int i)
{
    quint64 id = task_arr[i].GetID();
    if (i == int(task_arr.size())-1) task_arr.pop_back();
    for (int j=i; j<int(task_arr.size())-1; j++)
        task_arr[j] = task_arr[j+1];
    task_arr.pop_back();
    return id;
}
quint64 Schedule::DeleteBD(int i)
{
    quint64 id = bd_arr[i].GetID();
    if (i == int(bd_arr.size())-1) bd_arr.pop_back();
    for (int j=i; j<int(bd_arr.size())-1; j++)
        bd_arr[j] = bd_arr[j+1];
    bd_arr.pop_back();
    return id;
}

Task& Schedule::ActivateTask(int i)
{
    task_arr[i].Activate();
    return task_arr[i];
}
Task& Schedule::DeactivateTask(int i)
{
    task_arr[i].Deactivate();
    return task_arr[i];
}
