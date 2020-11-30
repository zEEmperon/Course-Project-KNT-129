#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{

    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    selectedEvent = Events::NONE;
    eventDate = nullptr;
    eventStart = nullptr;
    eventEnd = nullptr;
    eventNotificationTime = nullptr;
    eventDeadlineTime = nullptr;
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon
    QObject::connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(ui->rbBirthday, SIGNAL(clicked()), this, SLOT(on_rb_clicked()));
    QObject::connect(ui->rbBusiness, SIGNAL(clicked()), this, SLOT(on_rb_clicked()));
    QObject::connect(ui->rbTask, SIGNAL(clicked()), this, SLOT(on_rb_clicked()));
    QObject::connect(ui->rbMeet, SIGNAL(clicked()), this, SLOT(on_rb_clicked()));
    btnGroup = new QButtonGroup();
    btnGroup->addButton(ui->rbBusiness,0);
    btnGroup->addButton(ui->rbTask,1);
    btnGroup->addButton(ui->rbMeet,2);
    btnGroup->addButton(ui->rbBirthday,3);

}

AddEventDialog::~AddEventDialog()
{
    if(eventDate) delete eventDate;
    if(eventStart) delete eventStart;
    if(eventEnd) delete eventEnd;
    if(eventNotificationTime) delete eventNotificationTime;
    if(eventDeadlineTime) delete eventDeadlineTime;
    delete btnGroup;
    delete ui;

}

void AddEventDialog::on_rb_clicked(){
    int selectedRB = btnGroup->checkedId();
    disableAllTheWidgetsForInput();
    if(selectedRB != Events::NONE){
        selectedEvent = (Events)selectedRB;
        ui->teDescription->setEnabled(true);
        ui->dateEdit->setEnabled(true);
        switch (selectedEvent) {
        case Events::BUSINESS:
            ui->teStart->setEnabled(true);
            ui->teEnd->setEnabled(true);
            break;
        case Events::TASK:
            ui->teNotification->setEnabled(true);
            ui->teDeadline->setEnabled(true);
            ui->sbNumPriority->setEnabled(true);
            break;
        case Events::MEET:
            ui->teStart->setEnabled(true);
            ui->teEnd->setEnabled(true);
            ui->teNotification->setEnabled(true);
            break;
        case Events::BIRTHDAY:
            ui->teNotification->setEnabled(true);
            break;
        }
    }
}

void AddEventDialog::disableAllTheWidgetsForInput(){
    ui->teDescription->setEnabled(false);
    ui->dateEdit->setEnabled(false);
    ui->teStart->setEnabled(false);
    ui->teEnd->setEnabled(false);
    ui->teNotification->setEnabled(false);
    ui->teDeadline->setEnabled(false);
    ui->sbNumPriority->setEnabled(false);

}



void AddEventDialog::on_AddEventDialog_finished(int result)
{
    if(result == QDialog::Accepted){
        eventDescription = ui->teDescription->toPlainText();
        eventDate = new QDate(ui->dateEdit->date());
        switch (selectedEvent) {
        case Events::BUSINESS:
            eventStart = new QTime(ui->teStart->time());
            eventEnd = new QTime(ui->teEnd->time());
            break;
        case Events::TASK:
            eventNotificationTime = new QTime(ui->teNotification->time());
            eventDeadlineTime = new QTime(ui->teDeadline->time());
            eventNumPriority = ui->sbNumPriority->value();
            break;
        case Events::MEET:
            eventStart = new QTime(ui->teStart->time());
            eventEnd = new QTime(ui->teEnd->time());
            eventNotificationTime = new QTime(ui->teNotification->time());
            break;
        case Events::BIRTHDAY:
            eventNotificationTime = new QTime(ui->teNotification->time());
            break;
        }
    }
}

int AddEventDialog::get_eventNumPriority(){
    return eventNumPriority;
}
Events AddEventDialog::get_selectedEvent(){
    return selectedEvent;
}
QString AddEventDialog::get_eventDescription(){
    return eventDescription;
}
QDate AddEventDialog::get_eventDate(){
    return *eventDate;
}
QTime AddEventDialog::get_eventStartTime(){
    return *eventStart;
}
QTime AddEventDialog::get_eventEndTime(){
    return *eventEnd;
}
QTime AddEventDialog::get_eventNotificationTime(){
    return *eventNotificationTime;
}
QTime AddEventDialog::get_eventDeadlineTime(){
    return *eventDeadlineTime;
}
