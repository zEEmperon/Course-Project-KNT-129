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
    QDate get_eventDate();
    QTime get_eventStartTime();
    QTime get_eventEndTime();
    QTime get_eventNotificationTime();
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
    QDate* eventDate;
    QTime* eventStart;
    QTime* eventEnd;
    QTime* eventNotificationTime;
    QTime* eventDeadlineTime;

    void disableAllTheWidgetsForInput();
};

#endif // ADDEVENTDIALOG_H
