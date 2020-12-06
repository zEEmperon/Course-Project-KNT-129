#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"
#include "schedule.h"
#include "QFileDialog"
#include<vector>
#include <QCheckBox>
#include<QMessageBox>

#include<QDebug>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());//фіксований розмір вікна
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon

    ui->labelTodayDate->setText("Сьогодні "+QDate::currentDate().toString("dd.MM.yyyy"));
    displayTime();

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
    vector<Task> task;
    sch->GetTask(task);

    ui->tableHomeWork->setColumnCount(2);
    ui->tableHomeWork->setColumnWidth(0,125);
    ui->tableHomeWork->setColumnWidth(1,50);
    ui->tableHomeWork->setHorizontalHeaderLabels({"Завдання","Стан"});
    ui->tableHomeWork->horizontalHeader()->setVisible(true);
    ui->tableHomeWork->verticalHeader()->setVisible(false);

    //vector <Hometask> current_hometask = dbm->GetHometask(ui->dateEdit->date());
//    int rowCount = 4;//DB loading ДОБАВИТЬ КОЛ-ВО ДЗ ИЗ БД
//    ui->tableHomeWork->setRowCount(rowCount);

//    for(int i = 0; i < rowCount; i++){
//        ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
//    }//добавляем check box к каждой ячейке


    ui->tableTodayPesonalLife->setColumnCount(3);
    ui->tableTodayPesonalLife->setColumnWidth(0,100);
    ui->tableTodayPesonalLife->setColumnWidth(1,100);
    ui->tableTodayPesonalLife->setColumnWidth(2,100);
    ui->tableTodayPesonalLife->setHorizontalHeaderLabels({"Час","Подія","Місце"});
    ui->tableTodayPesonalLife->horizontalHeader()->setVisible(true);
    ui->tableTodayPesonalLife->verticalHeader()->setVisible(false);
    //vector <Meet> today_meet dbm->GetMeet( QDateTime(QDateTime::currentDateTime().date()), QDateTime(QDateTime::currentDateTime().date()).addDays(1).addMSecs(-1) );
    //vector <Birthday> today_bd dbm->GetBirthday( QDateTime(QDateTime::currentDateTime().date()), QDateTime(QDateTime::currentDateTime().date()).addDays(1).addMSecs(-1) );
    //ui->tableTodayPesonalLife->setRowCount(int(today_meet.size())+int(today_bd.size())); //DB loading
    ui->tableTodayPesonalLife->setRowCount(8);

    for (int i=0; i<int(today_meet.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(i, 0, new QTableWidgetItem(today_meet[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayPesonalLife->setItem(i, 1, new QTableWidgetItem(today_meet[i].getName()));
        ui->tableTodayPesonalLife->setItem(i, 2, new QTableWidgetItem(today_meet[i].getPlace()));
    }
    for (int i=0; i<int(today_bd.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(int(today_meet.size())+i, 1, new QTableWidgetItem(today_bd[i].getName()));
    }

    ui->tableTodayBusiness->setColumnCount(3);
    ui->tableTodayBusiness->setColumnWidth(0,100);
    ui->tableTodayBusiness->setColumnWidth(1,100);
    ui->tableTodayBusiness->setColumnWidth(2,100);
    ui->tableTodayBusiness->setHorizontalHeaderLabels({"Час","Заняття", "Місце"});
    ui->tableTodayBusiness->horizontalHeader()->setVisible(true);
    ui->tableTodayBusiness->verticalHeader()->setVisible(false);
    //vector <Study> today_study dbm->GetStudy( QDateTime(QDateTime::currentDateTime().date()), QDateTime(QDateTime::currentDateTime().date()).addDays(1).addMSecs(-1) );
    //ui->tableTodayUniversity->setRowCount(int(today_study.size())); //DB loading

    ui->tableTodayBusiness->setRowCount(8);

    for (int i=0; i<int(today_study.size()); i++)
    {
        ui->tableTodayBusiness->setItem(i, 0, new QTableWidgetItem(today_study[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayBusiness->setItem(i, 1, new QTableWidgetItem(today_study[i].getName()));
        ui->tableTodayBusiness->setItem(i, 2, new QTableWidgetItem(today_study[i].getPlace()));
    }


    //vector <Task> today_task dbm->GetTask( start, end ); //я не знаю, какие у тебя ограничения
    ui->tableTasks->setColumnCount(3);
    ui->tableTasks->setColumnWidth(0,210);
    ui->tableTasks->setColumnWidth(1,200);
    ui->tableTasks->setColumnWidth(2,200);
    ui->tableTasks->setHorizontalHeaderLabels({"Задача","Дедлайн","Витрачено часу"});
    ui->tableTasks->horizontalHeader()->setVisible(true);
    ui->tableTasks->verticalHeader()->setVisible(false);
    ui->tableTasks->setRowCount(12); //DB loading ЗАГРУЗКА ЗАДАЧ

    for (int i=0; i<int(task.size()); i++)
    {
        ui->tableTasks->setItem(i, 0, new QTableWidgetItem(task[i].getName()));
        ui->tableTasks->setItem(i, 1, new QTableWidgetItem(task[i].getTimeDeadline().toString("dd.MM.yyyy")));
        int w, h, m;
        w = task[i].getWorkTime();
        h = w/60;
        m = w - 60*h;
        QString w_time = QString::number(h) + QString(":") + QString::number(m);
        ui->tableTasks->setItem(i, 2, new QTableWidgetItem(w_time));
    }

    ui->tablePersonalLifeEvents->setColumnCount(3);
    ui->tablePersonalLifeEvents->setColumnWidth(0,100);
    ui->tablePersonalLifeEvents->setColumnWidth(1,100);
    ui->tablePersonalLifeEvents->setColumnWidth(2,100);
    ui->tablePersonalLifeEvents->setHorizontalHeaderLabels({"Час","Подія", "Місце"});
    ui->tablePersonalLifeEvents->horizontalHeader()->setVisible(true);
    ui->tablePersonalLifeEvents->verticalHeader()->setVisible(false);
    ui->textEdit->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    //ui->dateEdit->setMinimumDate(QDate::currentDate());
     ui->dateEdit->setMinimumDate(QDate(2020,12,3));
    ui->tablePersonalLifeEvents->setRowCount(5);

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

// Обробник натискання дати у календарі
void MainWindow::on_calendarPersonalLife_clicked(const QDate &date)
{
    ui->textEdit->setText(date.toString("dd.MM.yyyy"));

    ui->tablePersonalLifeEvents->clearContents();//тут стирает

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
        ui->tablePersonalLifeEvents->setItem(i, 2, new QTableWidgetItem(study[i].getPlace()));
    }
    for (int i=0; i<int(meet.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 0, new QTableWidgetItem(meet[i].getTimeBeg().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 1, new QTableWidgetItem(meet[i].getName()));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 2, new QTableWidgetItem(meet[i].getPlace()));
    }
    for (int i=0; i<int(bd.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+int(meet.size())+i, 1, new QTableWidgetItem(bd[i].getName()));
    }
}


// Обробник видалення подій из таблиці та з бази даних
void MainWindow::on_buttonRemovePersonalLife_clicked()
{
   QDate date = ui->calendarPersonalLife->selectedDate();

   QList<QTableWidgetItem*>selItemsList = ui->tablePersonalLifeEvents->selectedItems();

   vector <quint64> to_delete;

   if (int(selItemsList.size()) == 1)
   {
       QString name = selItemsList[0]->text();
       int i = sch->FindBD(name);
       to_delete.push_back(sch->DeleteBD(i));
   }

   int amount = int(selItemsList.size())/3;
   int current = 0;

   for (int l = 0; l<amount; l++)
   {
       QString str_time = selItemsList[current]->text();
       QString name = selItemsList[current+1]->text();

       int dot = str_time.indexOf(":", 0);
       QString hour =  str_time.left(dot);
       QString minut =  str_time.mid(dot+1, 2);
       QTime time(hour.toInt(), minut.toInt());

       int i = sch->FindStudy(name, time, date);
       if (i!=-1)
           to_delete.push_back(sch->DeleteStudy(i));
       else
       {
           i = sch->FindMeet(name, time, date);
           if (i!=-1)
               to_delete.push_back(sch->DeleteMeet(i));
       }
       current += 3;
    }

   dbm->DeleteData(to_delete);

   QModelIndexList selectedRows = ui->tablePersonalLifeEvents->selectionModel()->selectedRows();
   while (!selectedRows.empty())
   {
        ui->tablePersonalLifeEvents->removeRow(selectedRows[0].row());
        selectedRows = ui->tablePersonalLifeEvents->selectionModel()->selectedRows();
   }
}

// Обробник додавання події
void MainWindow::on_buttonAddPersonalLife_clicked()
{
    std::vector <PriorityData> priority_data = dbm->GetPriorityList();
    AddEventDialog dialog( priority_data );
    dialog.exec();

    if(dialog.result()==QDialog::Accepted){

       Events newEvent = dialog.get_selectedEvent();
       QDateTime eventDate;
       QString eventDescription = dialog.get_eventDescription();

       QString eventLocation;
       QTime eventStartTime;
       QTime eventEndTime;
       QDateTime eventDateAndNotificationTime;
       QDateTime eventDeadlineTime;
       int eventNumPriority;

       switch (newEvent) {
       case Events::BUSINESS:
       {
           eventDate = dialog.get_eventDate();
           eventStartTime = dialog.get_eventStartTime();
           eventEndTime = dialog.get_eventEndTime();
           eventLocation = dialog.get_eventLocation();

           // ------------ сюда исключение, если (eventLocation == "" || eventDescription == "") - не все поля заполнены ------------
           // ------------ сюда исключение, если eventStartTime > eventEndTime - время начала позже время окончания ------------

           Study s(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, EVENT_STUDY);
           dbm->AddStudy(s);
           sch->AddStudy(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, EVENT_STUDY);
           break;
       }
       case Events::TASK:
       {
           eventDateAndNotificationTime = dialog.get_eventDateAndNotificationTime();
           eventDeadlineTime = dialog.get_eventDeadlineTime();
           eventNumPriority = dialog.get_eventNumPriority();
           quint16 weight = dialog.get_eventWeightPriority();

           // ------------ сюда исключение, если (eventDescription == "") - не все поля заполнены ------------
           // ------------ сюда исключение, если eventStartTime > eventEndTime - время начала позже время окончания ------------
           // ------------ сюда исключение, если eventDeadlineTime < QDateTime::currentDateTime() - время дедлайна раньше сегодняшней даты ------------

           Task t(QDateTime(), eventDeadlineTime, weight, eventDescription, eventDateAndNotificationTime, EVENT_TASK, eventNumPriority);

           dbm->AddTask(t);
           sch->AddTask(QDateTime(), eventDeadlineTime, weight, eventDescription, eventDateAndNotificationTime, EVENT_TASK, eventNumPriority);

           vector<Task> task; sch->GetTask(task);
           ui->tableTasks->clear();
           for (int i=0; i<int(task.size()); i++)
           {
               ui->tableTasks->setItem(i, 0, new QTableWidgetItem(task[i].getName()));
               ui->tableTasks->setItem(i, 1, new QTableWidgetItem(task[i].getTimeDeadline().toString("dd.MM.yyyy")));
               int w, h, m;
               w = task[i].getWorkTime();
               h = w/60;
               m = w - 60*h;
               QString w_time = QString::number(h) + QString(":") + QString::number(m);
               ui->tableTasks->setItem(i, 2, new QTableWidgetItem(w_time));
           }
           break;
       }
       case Events::MEET:
       {
           eventDate = dialog.get_eventDate();
           eventStartTime = dialog.get_eventStartTime();
           eventEndTime = dialog.get_eventEndTime();
           eventDateAndNotificationTime = dialog.get_eventDateAndNotificationTime();
           eventLocation = dialog.get_eventLocation();

           // ------------ сюда исключение, если (eventLocation == "" || eventDescription == "") - не все поля заполнены ------------
           // ------------ сюда исключение, если eventStartTime > eventEndTime - время начала позже время окончания ------------

           Meet m(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, eventDateAndNotificationTime, EVENT_MEET);
           dbm->AddMeet(m);
           sch->AddMeet(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, eventDateAndNotificationTime, EVENT_MEET);
           break;
       }
       case Events::BIRTHDAY:
       {
           eventDate = dialog.get_eventDate();
           eventDateAndNotificationTime = dialog.get_eventDateAndNotificationTime();

           // ------------ сюда исключение, если (eventDescription == "") - не все поля заполнены ------------

           Birthday bd(eventDate, eventDescription, eventDateAndNotificationTime, EVENT_BIRTHDAY);
           dbm->AddBirthday(bd);
           sch->AddBD(eventDate, eventDescription, eventDateAndNotificationTime, EVENT_BIRTHDAY);
           break;
       }
       case Events::NONE:
           break;
       }
    }
}


void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save DataBase"), "dataBase",
            tr("CSV file (*.csv);;JSON file (*.json)"));
    if(fileName.contains(".csv")){
        dbm->ExportData_CSV( fileName, ',' );
    }
    else if(fileName.contains(".json")){
        dbm->ExportData_JSON( fileName);
    }
}
void MainWindow::slotUpdateDateTime(){

    if(timer->isActive()){
        displayTime();
        sch->MeetNotific(QTime::currentTime());
        sch->BDNotific(QTime::currentTime());
        sch->TaskNotific(QTime::currentTime());
    }
}

// Обробник початку роботи над задачею
void MainWindow::on_buttonBeginDo_clicked()
{
    QList<QTableWidgetItem*>selItemsList = ui->tableTasks->selectedItems();

    int amount = int(selItemsList.size())/3;
    int current = 0;

    for (int l = 0; l<amount; l++)
    {
        QString task_name = selItemsList[current]->text();

        QString str_time_deadline = selItemsList[current+1]->text();
        QString day =  str_time_deadline.left(2);
        QString month =  str_time_deadline.mid(3, 2);
        QString year =  str_time_deadline.right(4);
        QDate tdl(year.toInt(), month.toInt(), day.toInt());

        int task_num = sch->FindTask(task_name, tdl);
        dbm->ActivateTask(sch->ActivateTask(task_num));

        current+=3;
    }
}

// Обробник закінчення роботи над задачею
void MainWindow::on_buttonEndDo_clicked()
{
    QList<QTableWidgetItem*>selItemsList = ui->tableTasks->selectedItems();

    int amount = int(selItemsList.size())/3;
    int current = 0;

    for (int l = 0; l<amount; l++)
    {
        QString task_name = selItemsList[current]->text();

        QString str_time_deadline = selItemsList[current+1]->text();
        QString day =  str_time_deadline.left(2);
        QString month =  str_time_deadline.mid(3, 2);
        QString year =  str_time_deadline.right(4);
        QDate tdl(year.toInt(), month.toInt(), day.toInt());

        int task_num = sch->FindTask(task_name, tdl);
        dbm->DeactivateTask(sch->DeactivateTask(task_num));
        current+=3;
    }

    vector<Task> task; sch->GetTask(task);
    ui->tableTasks->clear();
    for (int i=0; i<int(task.size()); i++)
    {
        ui->tableTasks->setItem(i, 0, new QTableWidgetItem(task[i].getName()));
        ui->tableTasks->setItem(i, 1, new QTableWidgetItem(task[i].getTimeDeadline().toString("dd.MM.yyyy")));
        int w, h, m;
        w = task[i].getWorkTime();
        h = w/60;
        m = w - 60*h;
        QString w_time = QString::number(h) + QString(":") + QString::number(m);
        ui->tableTasks->setItem(i, 2, new QTableWidgetItem(w_time));
    }
}

// Обробник видалення задачі
void MainWindow::on_buttonRemove_clicked()
{
    QList<QTableWidgetItem*>selItemsList = ui->tableTasks->selectedItems();

    vector <quint64> to_delete;

    int amount = int(selItemsList.size())/3;
    int current = 0;

   for (int l = 0; l<amount; l++)
    {
        QString name = selItemsList[current]->text();
        QString str_tdl = selItemsList[current+1]->text();

        QString day =  str_tdl.left(2);
        QString month =  str_tdl.mid(3, 2);
        QString year =  str_tdl.right(4);
        QDate tdl(year.toInt(), month.toInt(), day.toInt());

        int i = sch->FindTask(name, tdl);
        if (i!=-1)
            to_delete.push_back(sch->DeleteTask(i));
        current += 3;
     }

    dbm->DeleteData(to_delete);

    QModelIndexList selectedRows = ui->tableTasks->selectionModel()->selectedRows();
    while (!selectedRows.empty())
    {
         ui->tableTasks->removeRow(selectedRows[0].row());
         selectedRows = ui->tableTasks->selectionModel()->selectedRows();
    }
}

void MainWindow::on_buttonShowHomework_clicked()
{
    vector <Hometask> current_hometask = dbm->GetHometask(ui->dateEdit->date());

    ui->tableHomeWork->clearContents();
    ui->tableHomeWork->model()->removeRows(0, ui->tableHomeWork->rowCount());

    //ui->tableHomeWork->setRowCount(current_hometask.size());

   for(int i = 0; i < (int)current_hometask.size();i++){
       ui->tableHomeWork->insertRow ( ui->tableHomeWork->rowCount() );
       ui->tableHomeWork->setItem   ( ui->tableHomeWork->rowCount()-1, 0, new QTableWidgetItem(current_hometask[i].name));
       ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
       QTableWidgetItem* pItem(ui->tableHomeWork->item(ui->tableHomeWork->rowCount()-1,0));
       pItem->setCheckState(Qt::CheckState(current_hometask[i].completed));
   }
}
void MainWindow::on_tableHomeWork_cellChanged(int row, int column)
{
    if(column == 1){
        Qt::CheckState cur = ui->tableHomeWork->item(row,column)->checkState();
        vector<Hometask> hometasks = dbm->GetHometask(ui->dateEdit->date());
        for(int i = 0; i<hometasks.size();i++){
            if(hometasks[i].name == ui->tableHomeWork->item(row,0)->text()){
                if(cur != hometasks[i].completed){
                    hometasks[i].completed = cur;
                    dbm->ModifyHometask(hometasks[i]);
                    break;
                }
            }
        }
    }
}
void MainWindow::displayTime(){

    ui->lcdHoursBIG->display((int)QTime::currentTime().hour()/10);
    ui->lcdHoursLIT->display(QTime::currentTime().hour()%10);
    ui->lcdMinutesBIG->display((int)QTime::currentTime().minute()/10);
    ui->lcdMinutesLIT->display(QTime::currentTime().minute()%10);
    ui->lcdSecondsBIG->display((int)QTime::currentTime().second()/10);
    ui->lcdSecondsLIT->display(QTime::currentTime().second()%10);
}


void MainWindow::on_tableScheduleUniversity_cellChanged(int row, int column)
{
//Вероника, сюда добавь

}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 0){
        //Настя, добавь сюда
    }
}
