#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editpltablewindow.h"
#include "event.h"
#include "schedule.h"
#include "QFileDialog"
#include<vector>

#include<QDebug>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbm = new DBManager();

    QDate d = QDate::currentDate();

    QDateTime start = QDateTime::currentDateTime().addDays(-30);
    QDateTime end = QDateTime::currentDateTime().addDays(30);

    sch = new Schedule(dbm->GetStudy(start, end), dbm->GetMeet(start, end), dbm->GetTask(start, end), dbm->GetBirthday(start, end));

    vector<Study> today_study;
    sch->GetStudy(today_study, d);
    vector<Meet> today_meet;
    sch->GetMeet(today_meet, d);
    vector<Birthday> today_bd;
    sch->GetBD(today_bd, d);

    ui->tableTodayPesonalLife->setColumnCount(2);
    ui->tableTodayPesonalLife->setColumnWidth(0,125);
    ui->tableTodayPesonalLife->setColumnWidth(1,125);
    ui->tableTodayPesonalLife->setHorizontalHeaderLabels({"Час","Подія"});
    ui->tableTodayPesonalLife->horizontalHeader()->setVisible(true);
    ui->tableTodayPesonalLife->verticalHeader()->setVisible(false);
    ui->tableTodayPesonalLife->setRowCount(int(today_meet.size())+int(today_bd.size())); //DB loading

    for (int i=0; i<int(today_meet.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(i, 0, new QTableWidgetItem(today_meet[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayPesonalLife->setItem(i, 1, new QTableWidgetItem(today_meet[i].getName()));
    }
    for (int i=0; i<int(today_bd.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(int(today_meet.size())+i, 1, new QTableWidgetItem(today_bd[i].getName()));
    }

    ui->tableTodayUniversity->setColumnCount(2);
    ui->tableTodayUniversity->setColumnWidth(0,125);
    ui->tableTodayUniversity->setColumnWidth(1,125);
    ui->tableTodayUniversity->setHorizontalHeaderLabels({"Час","Заняття"});
    ui->tableTodayUniversity->horizontalHeader()->setVisible(true);
    ui->tableTodayUniversity->verticalHeader()->setVisible(false);
    ui->tableTodayUniversity->setRowCount(int(today_study.size())); //DB loading


    for (int i=0; i<int(today_study.size()); i++)
    {
        ui->tableTodayUniversity->setItem(i, 0, new QTableWidgetItem(today_study[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayUniversity->setItem(i, 1, new QTableWidgetItem(today_study[i].getName()));
    }

    ui->tablePersonalLifeEvents->setColumnCount(2);
    ui->tablePersonalLifeEvents->setColumnWidth(0,125);
    ui->tablePersonalLifeEvents->setColumnWidth(1,125);
    ui->tablePersonalLifeEvents->setHorizontalHeaderLabels({"Час","Подія"});
    ui->tablePersonalLifeEvents->horizontalHeader()->setVisible(true);
    ui->tablePersonalLifeEvents->verticalHeader()->setVisible(false);
    ui->textEdit->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    ui->dateEdit->setMinimumDate(QDate::currentDate());

}

MainWindow::~MainWindow()
{
    delete sch;
    if( dbm )
    {
        delete dbm;
        dbm = nullptr;
        QSqlDatabase::removeDatabase( "tasklist" );
    }
    delete ui;
}


void MainWindow::on_calendarPersonalLife_clicked(const QDate &date)
{
    ui->textEdit->setText(date.toString("dd.MM.yyyy"));

    vector<Study> study;
    sch->GetStudy(study, date);
    vector<Meet> meet;
    sch->GetMeet(meet, date);
    vector<Birthday> bd;
    sch->GetBD(bd, date);

    ui->tablePersonalLifeEvents->setRowCount(int(study.size())+int(meet.size())+int(bd.size())); //DB loading

    for (int i=0; i<int(study.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(i, 0, new QTableWidgetItem(study[i].getTimeBeg().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(i, 1, new QTableWidgetItem(study[i].getName()));
    }
    for (int i=0; i<int(meet.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 0, new QTableWidgetItem(meet[i].getTimeBeg().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 1, new QTableWidgetItem(meet[i].getName()));
    }
    for (int i=0; i<int(bd.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+int(meet.size())+i, 1, new QTableWidgetItem(bd[i].getName()));
    }
}

void MainWindow::on_buttonRemovePersonalLife_clicked()
{
   //удаление события из таблицы и базы данных
   /* QList<QTableWidgetItem*>selItemsList = ui->tablePersonalLifeEvents->selectedItems();*/

}

void MainWindow::on_buttonAddPersonalLife_clicked()
{
    EditPLTableWindow *w = new EditPLTableWindow;
    //w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    //добавление события в таблицу и базу данных
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Data Base"), "dataBase",
            tr("Data Base (*.db)"));
//ВЕРОНИКА
//Получаем полный путь для сохранения файла + имя файла с расширением. Теперь нужно окрыть файловый поток и сохранить базу данных
}
