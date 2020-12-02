#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <vector>
#include <QDialog>
#include <QButtonGroup>
#include "eventsEnum.h"
#include "db_data.h"


namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT

public:

    explicit AddEventDialog( const std::vector <PriorityData>& priority_data, QWidget *parent = nullptr);
    ~AddEventDialog();

    quint64 get_eventNumPriority();
    quint16 get_eventWeightPriority();
    Events get_selectedEvent();
    QString get_eventDescription();
    QString get_eventLocation();
    QDateTime get_eventDate();
    QTime get_eventStartTime();
    QTime get_eventEndTime();
    QDateTime get_eventDateAndNotificationTime();
    QDateTime get_eventDeadlineTime();

private slots:
    void on_rb_clicked();

    void on_AddEventDialog_finished(int result);

private:
    QButtonGroup* btnGroup;
    Ui::AddEventDialog *ui;

    quint64 eventNumPriority;
    Events selectedEvent;
    QString eventDescription;
    QString eventLocation;
    QDateTime* eventDate;
    QTime* eventStart;
    QTime* eventEnd;
    QDateTime* eventDateAndNotificationTime;
    QDateTime* eventDeadlineTime;

    std::vector <PriorityData> priorities;

    void disableAllTheWidgetsForInput();
};

#endif // ADDEVENTDIALOG_H
