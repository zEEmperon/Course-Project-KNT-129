#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{

    ui->setupUi(this);
    selectedEvent = Events::NONE;
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
        //запись в поля
    }
}
