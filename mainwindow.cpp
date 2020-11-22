#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editpltablewindow.h"
#include "event.h"
#include "schedule.h"
#include "QFileDialog"
#include<vector>
#include <QCheckBox>

#include<QDebug>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon

    ui->labelTodayDate->setText("Сьогодні "+QDate::currentDate().toString("dd.MM.yyyy"));


    ui->lcdHoursBIG->display((int)QTime::currentTime().hour()/10);
    ui->lcdHoursLIT->display(QTime::currentTime().hour()%10);
    ui->lcdMinutesBIG->display((int)QTime::currentTime().minute()/10);
    ui->lcdMinutesLIT->display(QTime::currentTime().minute()%10);
    ui->lcdSecondsBIG->display((int)QTime::currentTime().second()/10);
    ui->lcdSecondsLIT->display(QTime::currentTime().second()%10);

//timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
    timer->start(1000);
    //slotUpdateDateTime();

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

    ui->tableHomeWork->setColumnCount(2);
    ui->tableHomeWork->setColumnWidth(0,125);
    ui->tableHomeWork->setColumnWidth(1,50);
    ui->tableHomeWork->setHorizontalHeaderLabels({"Завдання","Стан"});
    ui->tableHomeWork->horizontalHeader()->setVisible(true);
    ui->tableHomeWork->verticalHeader()->setVisible(false);
    int rowCount = 4;//DB loading ДОБАВИТЬ КОЛ-ВО ДЗ ИЗ БД
    ui->tableHomeWork->setRowCount(rowCount);

    for(int i = 0; i < rowCount; i++){
        ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
    }//добавляем check box к каждой ячейке



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

    ui->tableTasks->setColumnCount(3);
    ui->tableTasks->setColumnWidth(0,209);
    ui->tableTasks->setColumnWidth(1,200);
    ui->tableTasks->setColumnWidth(2,200);
    ui->tableTasks->setHorizontalHeaderLabels({"Задача","Дедлайн","Витрачено часу"});
    ui->tableTasks->horizontalHeader()->setVisible(true);
    ui->tableTasks->verticalHeader()->setVisible(false);
    ui->tableTasks->setRowCount(12); //DB loading ЗАГРУЗКА ЗАДАЧ


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
    delete timer;
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
    EditPLTableWindow *editPLWindow = new EditPLTableWindow;
    //editPLWindow->setAttribute(Qt::WA_DeleteOnClose);
    editPLWindow->show();

    //добавление события в таблицу и базу данных
    delete editPLWindow;
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save DataBase"), "dataBase",
            tr("CSV file (*.csv);;JSON file (*.json)"));
    if(fileName.contains(".csv")){
        //сохранение csv
    }
    else if(fileName.contains(".json")){
        //сохранение json
    }
//ВЕРОНИКА
//Получаем полный путь для сохранения файла + имя файла с расширением. Теперь нужно окрыть файловый поток и сохранить базу данных
}
void MainWindow::slotUpdateDateTime(){

    if(timer->isActive()){

        ui->lcdHoursBIG->display((int)QTime::currentTime().hour()/10);
        ui->lcdHoursLIT->display(QTime::currentTime().hour()%10);
        ui->lcdMinutesBIG->display((int)QTime::currentTime().minute()/10);
        ui->lcdMinutesLIT->display(QTime::currentTime().minute()%10);
        ui->lcdSecondsBIG->display((int)QTime::currentTime().second()/10);
        ui->lcdSecondsLIT->display(QTime::currentTime().second()%10);
    }
}
