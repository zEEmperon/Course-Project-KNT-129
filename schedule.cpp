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
    try {
        Study s(d, b, e, n, p, t_event_id);
        bool isB = isBusy(d.date(), b, e);
        if (isB) throw Exception(1);
        study_arr.push_back(s);
        SortStudy(0, int(study_arr.size())-1);
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
    }
    catch (Exception &e)
    {
        e.show();
    }
}
void Schedule::AddMeet(QDateTime d, QTime b, QTime e, QString n, QString p, QDateTime notific, quint64 t_event_id)
{
    try {
        Meet m(d, b, e, n, p, notific, t_event_id);
        bool isB = isBusy(d.date(), b, e);
        if (isB) throw Exception(1);
        meet_arr.push_back(m);
        SortMeet(0, int(meet_arr.size())-1);
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
    }
    catch (Exception &e)
    {
        e.show();
    }
}
void Schedule::AddTask(QDateTime d, QDateTime tdl, quint16 p, QString n, QDateTime notific, quint64 t_event_id, quint64 p_id)
{
    Task t(d, tdl, p, n, notific, t_event_id, p_id);
    task_arr.push_back(t);
    SortTask(0, int(task_arr.size())-1);
}
void Schedule::AddBD(QDateTime d, QString n, QDateTime notific, quint64 t_event_id)
{
    Birthday b(d, n, notific, t_event_id);
    bd_arr.push_back(b);
    SortBD(0, int(bd_arr.size())-1);
}

void Schedule::SetID( quint64 id, quint64 t_event_id, int i )
{
    switch( t_event_id )
    {
        case 1:
            task_arr[i].SetID( id );
            break;
    }
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

// сортування
int Schedule::PartitionStudy(int p, int r)
{
    Study x = study_arr[r];
    int i=p-1;
    for (int j=p; j<=r-1; j++)
        if ((study_arr[j].date.date() < x.date.date()) || (study_arr[j].date.date() == x.date.date() && study_arr[j].timeBeg <= x.timeBeg))
        {
            i++;
            Study tmp = study_arr[j];
            study_arr[j] = study_arr[i];
            study_arr[i] = tmp;
        }
    Study tmp = study_arr[r];
    study_arr[r] = study_arr[i+1];
    study_arr[i+1] = tmp;
    return i+1;
}

void Schedule::SortStudy(int p, int r)
{
    if (p<r)
    {
        int q = PartitionStudy(p, r);
        SortStudy(p, q-1);
        SortStudy(q+1, r);
    }
}

int Schedule::PartitionMeet(int p, int r)
{
    Meet x = meet_arr[r];
    int i=p-1;
    for (int j=p; j<=r-1; j++)
        if (meet_arr[j].date.date() < x.date.date() || (meet_arr[j].date.date() == x.date.date() && meet_arr[j].timeBeg <= x.timeBeg))
        {
            i++;
            Meet tmp = meet_arr[j];
            meet_arr[j] = meet_arr[i];
            meet_arr[i] = tmp;
        }
    Meet tmp = meet_arr[r];
    meet_arr[r] = meet_arr[i+1];
    meet_arr[i+1] = tmp;
    return i+1;
}

void Schedule::SortMeet(int p, int r)
{
    if (p<r)
    {
        int q = PartitionMeet(p, r);
        SortMeet(p, q-1);
        SortMeet(q+1, r);
    }
}

int Schedule::PartitionTask(int p, int r)
{
    Task x = task_arr[r];
    int i=p-1;
    for (int j=p; j<=r-1; j++)
        if ((task_arr[j].date < x.date)||(task_arr[j].date == x.date && task_arr[j].timeDeadline < x.timeDeadline)||
            (task_arr[j].date == x.date && task_arr[j].timeDeadline == x.timeDeadline && task_arr[j].priority > x.priority))
        {
            i++;
            Task tmp = task_arr[j];
            task_arr[j] = task_arr[i];
            task_arr[i] = tmp;
        }
    Task tmp = task_arr[r];
    task_arr[r] = task_arr[i+1];
    task_arr[i+1] = tmp;
    return i+1;
}

void Schedule::SortTask(int p, int r)
{
    if (p<r)
    {
        int q = PartitionTask(p, r);
        SortTask(p, q-1);
        SortTask(q+1, r);
    }
}

int Schedule::PartitionBD(int p, int r)
{
    Birthday x = bd_arr[r];
    int i=p-1;
    for (int j=p; j<=r-1; j++)
        if (bd_arr[j].date.date() < x.date.date())
        {
            i++;
            Birthday tmp = bd_arr[j];
            bd_arr[j] = bd_arr[i];
            bd_arr[i] = tmp;
        }
    Birthday tmp = bd_arr[r];
    bd_arr[r] = bd_arr[i+1];
    bd_arr[i+1] = tmp;
    return i+1;
}

void Schedule::SortBD(int p, int r)
{
    if (p<r)
    {
        int q = PartitionBD(p, r);
        SortBD(p, q-1);
        SortBD(q+1, r);
    }
}

// сповіщення
void Schedule::MeetNotific(QTime curr_time)
{
    int n = int(meet_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((meet_arr[i].date.date() == curr_date || (meet_arr[i].date.date()).addDays(-3) == curr_date) &&
             meet_arr[i].timeNotification.time().hour() == curr_time.hour() &&
             meet_arr[i].timeNotification.time().minute() == curr_time.minute() &&
             curr_time.second() == 0)
        {
            NotificationDialog nDialog("Зустріч: " + meet_arr[i].name);
            nDialog.exec();
        }
}
void Schedule::TaskNotific(QTime curr_time)
{
    int n = int(task_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((task_arr[i].timeDeadline.date() == curr_date || (task_arr[i].timeDeadline.date()).addDays(-3) == curr_date) &&
             task_arr[i].timeNotification.time().hour() == curr_time.hour() &&
             task_arr[i].timeNotification.time().minute() == curr_time.minute() &&
             curr_time.second() == 0)
        {
            NotificationDialog nDialog("Дедлайн: " + task_arr[i].name);
            nDialog.exec();
        }
}
void Schedule::BDNotific(QTime curr_time)
{
    int n = int(bd_arr.size());
    QDate curr_date = QDate::currentDate();
    for (int i=0; i<n; i++)
        if ((bd_arr[i].date.date() == curr_date || (bd_arr[i].date.date()).addDays(-3) == curr_date) &&
             bd_arr[i].timeNotification.time().hour() == curr_time.hour() &&
             bd_arr[i].timeNotification.time().minute() == curr_time.minute() &&
             curr_time.second() == 0)
        {
            NotificationDialog nDialog("День народження: " + bd_arr[i].name);
            nDialog.exec();
        }
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
        if ((meet_arr[i].getDate() >= start_interval) && (meet_arr[i].getDate() <= end_interval))
            meet.push_back(meet_arr[i]);
}
void Schedule::GetTask(vector<Task>& task)
{
    task.resize(0);

    for (int i=0; i<int(task_arr.size()); i++)
            task.push_back(task_arr[i]);
}
void Schedule::GetBD(vector<Birthday>& bd, QDate needed_date)
{
    bd.resize(0);

    QDateTime start_interval(needed_date, QTime(0, 0, 0));
    QDateTime end_interval(needed_date, QTime(23, 59, 59));

    for (int i=0; i<int(bd_arr.size()); i++)
        if ((bd_arr[i].getDate() >= start_interval) && (bd_arr[i].getDate() <= end_interval))
            bd.push_back(bd_arr[i]);
}

int Schedule::FindStudy(QString n, QTime time, QDate date)
{
    try {
        int i = 0;
        bool found = false;
        while (!found && i<int(study_arr.size()))
        {
            if (study_arr[i].name == n && (study_arr[i].timeBeg.hour() == time.hour() && study_arr[i].timeBeg.minute() == time.minute()) && study_arr[i].date.date() == date)
                found = true;
            i++;
        }
        i--;
        if (!found)
            return -1;
        return i;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
}

int Schedule::FindMeet(QString n, QTime time, QDate date)
{
    try {
        int i = 0;
        bool found = false;
        while (!found && i<int(meet_arr.size()))
        {
            if (meet_arr[i].name == n && (meet_arr[i].timeBeg.hour() == time.hour() && meet_arr[i].timeBeg.minute() == time.minute()) && meet_arr[i].date.date() == date)
                found = true;
            i++;
        }
        i--;
        if (!found)
            return -1;
        return i;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
}

int Schedule::FindTask(QString n, QDate tdl)
{
    try {
        int i = 0;
        bool found = false;
        while (!found && i<int(task_arr.size()))
        {
            if (task_arr[i].name == n && task_arr[i].timeDeadline.date() == tdl)
                found = true;
            i++;
        }
        i--;
        if (!found)
            return -1;
        return i;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
}

int Schedule::FindBD(QString n)
{
    try {
        int i = 0;
        bool found = false;
        while (!found && i<int(bd_arr.size()))
        {
            if (bd_arr[i].name == n)
                found = true;
            i++;
        }
        i--;
        if (!found)
        {
            throw Exception(2);
        }
        return i;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
    catch (Exception &e)
    {
        e.show();
        return -1;
    }
}

quint64 Schedule::DeleteStudy(int i)
{
    try {
        if (i>int(study_arr.size())) throw Exception(3);
        quint64 id = study_arr[i].GetID();
        if (i == int(study_arr.size())-1)
            study_arr.pop_back();
        else
        {
            for (int j=i; j<int(study_arr.size())-1; j++)
                study_arr[j] = study_arr[j+1];
            study_arr.pop_back();
        }
        return id;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
    catch (Exception &e)
    {
        e.show();
        return -1;
    }
}
quint64 Schedule::DeleteMeet(int i)
{
    try {
        if (i>int(meet_arr.size())) throw Exception(3);
        quint64 id = meet_arr[i].GetID();
        if (i == int(meet_arr.size())-1)
            meet_arr.pop_back();
        else
        {
            for (int j=i; j<int(meet_arr.size())-1; j++)
                meet_arr[j] = meet_arr[j+1];
            meet_arr.pop_back();
        }
        return id;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
    catch (Exception &e)
    {
        e.show();
        return -1;
    }
}
quint64 Schedule::DeleteTask(int i)
{
    try {
        if (i>int(task_arr.size())) throw Exception(3);
        quint64 id = task_arr[i].GetID();
        if (i == int(task_arr.size())-1)
            task_arr.pop_back();
        else
        {
            for (int j=i; j<int(task_arr.size())-1; j++)
                task_arr[j] = task_arr[j+1];
            task_arr.pop_back();
        }
        return id;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
    catch (Exception &e)
    {
        e.show();
        return -1;
    }
}
quint64 Schedule::DeleteBD(int i)
{
    try {
        if (i>int(bd_arr.size())) throw Exception(3);
        quint64 id = bd_arr[i].GetID();
        if (i == int(bd_arr.size())-1)
            bd_arr.pop_back();
        else
        {
            for (int j=i; j<int(bd_arr.size())-1; j++)
                bd_arr[j] = bd_arr[j+1];
            bd_arr.pop_back();
        }
        return id;
    }
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
        return -1;
    }
    catch (Exception &e)
    {
        e.show();
        return -1;
    }
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
