#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "eventsEnum.h"

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT

public:

    explicit AddEventDialog(QWidget *parent = nullptr);
    ~AddEventDialog();

    int get_eventNumPriority();
    Events get_selectedEvent();
    QString get_eventDescription();
    QString get_eventLocation();
    QDateTime get_eventDate();
    QTime get_eventStartTime();
    QTime get_eventEndTime();
    QDateTime get_eventDateAndNotificationTime();
    QTime get_eventDeadlineTime();

private slots:
    void on_rb_clicked();

    void on_AddEventDialog_finished(int result);

private:
    QButtonGroup* btnGroup;
    Ui::AddEventDialog *ui;

    int eventNumPriority;
    Events selectedEvent;
    QString eventDescription;
    QString eventLocation;
    QDateTime* eventDate;
    QTime* eventStart;
    QTime* eventEnd;
    QDateTime* eventDateAndNotificationTime;
    QTime* eventDeadlineTime;

    void disableAllTheWidgetsForInput();
};

#endif // ADDEVENTDIALOG_H
