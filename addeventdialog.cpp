#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog( const std::vector <PriorityData>& priority_data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog), priorities(priority_data)
{

    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    selectedEvent = Events::NONE;
    eventDate = nullptr;
    eventStart = nullptr;
    eventEnd = nullptr;
    eventDateAndNotificationTime = nullptr;
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

    for( size_t i = 0; i < priorities.size(); i++ )
    {
        ui->weight_comboBox->addItem( priorities[i].name, priorities[i].id );
    }
}

AddEventDialog::~AddEventDialog()
{
    if(eventDate) delete eventDate;
    if(eventStart) delete eventStart;
    if(eventEnd) delete eventEnd;
    if(eventDateAndNotificationTime) delete eventDateAndNotificationTime;
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
            ui->leLocation->setEnabled(true);
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
            ui->leLocation->setEnabled(true);
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
    ui->leLocation->setEnabled(false);

}



void AddEventDialog::on_AddEventDialog_finished(int result)
{
    if(result == QDialog::Accepted){
        eventDescription = ui->teDescription->toPlainText();
        eventDate = new QDateTime(ui->dateEdit->dateTime());
        switch (selectedEvent) {
        case Events::BUSINESS:
            eventStart = new QTime(ui->teStart->time());
            eventEnd = new QTime(ui->teEnd->time());
            eventLocation = ui->leLocation->text();
            break;
        case Events::TASK:
            eventDateAndNotificationTime = new QDateTime();
            eventDateAndNotificationTime->setDate(ui->dateEdit->date());
            eventDateAndNotificationTime->setTime(ui->teNotification->time());
            eventDeadlineTime = new QTime(ui->teDeadline->time());
            eventNumPriority = ui->sbNumPriority->value();
            break;
        case Events::MEET:
            eventStart = new QTime(ui->teStart->time());
            eventEnd = new QTime(ui->teEnd->time());
            eventLocation = ui->leLocation->text();
            eventDateAndNotificationTime = new QDateTime();
            eventDateAndNotificationTime->setDate(ui->dateEdit->date());
            eventDateAndNotificationTime->setTime(ui->teNotification->time());
            break;
        case Events::BIRTHDAY:
            eventDateAndNotificationTime = new QDateTime();
            eventDateAndNotificationTime->setDate(ui->dateEdit->date());
            eventDateAndNotificationTime->setTime(ui->teNotification->time());
            break;
        }
    }
}

quint64 AddEventDialog::get_eventNumPriority(){
    if( ui->weight_comboBox->currentIndex() != -1 )
    {
        eventNumPriority = ui->weight_comboBox->currentData().toULongLong();
    }
    else
        eventNumPriority = ui->weight_comboBox->itemData(0).toULongLong();

    return eventNumPriority;
}

quint16 AddEventDialog::get_eventWeightPriority(){
    quint64 id = ui->weight_comboBox->currentData().toULongLong();
    int i = 0;
    while( priorities[i].id != id)
        i++;
    return priorities[i].weight;
}

Events AddEventDialog::get_selectedEvent(){
    return selectedEvent;
}
QString AddEventDialog::get_eventDescription(){
    return eventDescription;
}
QString AddEventDialog::get_eventLocation(){
    return eventLocation;
}
QDateTime AddEventDialog::get_eventDate(){
    return *eventDate;
}
QTime AddEventDialog::get_eventStartTime(){
    return *eventStart;
}
QTime AddEventDialog::get_eventEndTime(){
    return *eventEnd;
}
QDateTime AddEventDialog::get_eventDateAndNotificationTime(){
    return *eventDateAndNotificationTime;
}
QTime AddEventDialog::get_eventDeadlineTime(){
    return *eventDeadlineTime;
}
