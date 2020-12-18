#include "addhomework.h"
#include "ui_addhomework.h"

AddHomeWork::AddHomeWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddHomeWork)
{
    ui->setupUi(this);
    ui->dateEdit->setMinimumDate(QDate::currentDate().addDays(1));
    this->setFixedSize(this->size());
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon
    QObject::connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    date = nullptr;
}

QDate AddHomeWork::get_date()
{
    return *date;
}

QString AddHomeWork::get_homework()
{
    return homeWork;
}

AddHomeWork::~AddHomeWork()
{
    if(date) delete date;
    delete ui;
}

void AddHomeWork::on_AddHomeWork_finished(int result)
{
    if(result == QDialog::Accepted){
        date = new QDate(ui->dateEdit->date());
        homeWork = ui->leHomeWork->text();

    }
}
