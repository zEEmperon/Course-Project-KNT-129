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
    :QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_is_valid(false)
    , dbm(nullptr)
    , timer(nullptr)
    , sch(nullptr)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());//фіксований розмір вікна
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon

    ui->labelTodayDate->setText("Сьогодні "+QDate::currentDate().toString("dd.MM.yyyy"));
    displayTime();

    dbm = new DBManager();
    if( dbm && !dbm->IsValid() )
        dbm = nullptr;

    if( !dbm  )
        return;

    //timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
    timer->start(1000);

    QDate d = QDate::currentDate();
    QDateTime start = QDateTime::currentDateTime().addYears(-30);
    QDateTime end = QDateTime::currentDateTime().addYears(30);

    sch = new Schedule(dbm->GetStudy(start, end), dbm->GetMeet(start, end), dbm->GetTask(start, end), dbm->GetBirthday(start, end));

    vector<Study> today_study;
    sch->GetStudy(today_study, d);
    vector<Meet> today_meet;
    sch->GetMeet(today_meet, d);
    vector<Birthday> today_bd;
    sch->GetBD(today_bd, d);
    vector<Task> task;
    sch->GetTask(task);
    //university_schedule = dbm->GetLessons();
    dbm->GetLessons(university_schedule);

    ui->tableScheduleUniversity->clearContents();
    for (int i=0; i<int(university_schedule.size()); i++)
    {
        ui->tableScheduleUniversity->setItem(university_schedule[i].week_day-1, university_schedule[i].lesson_number-1, new QTableWidgetItem(university_schedule[i].name));
    }

    vector <Hometask> current_hometask = dbm->GetHometask(ui->dateEdit->date());

    ui->tableHomeWork->clearContents();
    ui->tableHomeWork->model()->removeRows(0, ui->tableHomeWork->rowCount());

    for(int i = 0; i < (int)current_hometask.size();i++){
       ui->tableHomeWork->insertRow ( ui->tableHomeWork->rowCount() );
       ui->tableHomeWork->setItem   ( ui->tableHomeWork->rowCount()-1, 0, new QTableWidgetItem(current_hometask[i].name));
       ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
       QTableWidgetItem* pItem(ui->tableHomeWork->item(ui->tableHomeWork->rowCount()-1,0));
       pItem->setCheckState(Qt::CheckState(current_hometask[i].completed));
   }


    ui->tableHomeWork->setColumnCount(1);
    ui->tableHomeWork->setColumnWidth(0,185);
    ui->tableHomeWork->setHorizontalHeaderLabels({"Завдання"});
    ui->tableHomeWork->horizontalHeader()->setVisible(true);
    ui->tableHomeWork->verticalHeader()->setVisible(false);

    ui->tableTodayPesonalLife->setColumnCount(4);
    ui->tableTodayPesonalLife->setColumnWidth(0,61);
    ui->tableTodayPesonalLife->setColumnWidth(1,61);
    ui->tableTodayPesonalLife->setColumnWidth(2,89);
    ui->tableTodayPesonalLife->setColumnWidth(3,89);
    ui->tableTodayPesonalLife->setHorizontalHeaderLabels({"Початок", "Кінець","Подія","Місце"});
    ui->tableTodayPesonalLife->horizontalHeader()->setVisible(true);
    ui->tableTodayPesonalLife->verticalHeader()->setVisible(false);
    ui->tableTodayPesonalLife->setRowCount(20);

    for (int i=0; i<int(today_meet.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(i, 0, new QTableWidgetItem(today_meet[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayPesonalLife->setItem(i, 1, new QTableWidgetItem(today_meet[i].getTimeEnd().toString("HH:mm")));
        ui->tableTodayPesonalLife->setItem(i, 2, new QTableWidgetItem(today_meet[i].getName()));
        ui->tableTodayPesonalLife->setItem(i, 3, new QTableWidgetItem(today_meet[i].getPlace()));
    }
    for (int i=0; i<int(today_bd.size()); i++)
    {
        ui->tableTodayPesonalLife->setItem(int(today_meet.size())+i, 2, new QTableWidgetItem(today_bd[i].getName()));
    }

    ui->tableTodayBusiness->setColumnCount(4);
    ui->tableTodayBusiness->setColumnWidth(0,61);
    ui->tableTodayBusiness->setColumnWidth(1,61);
    ui->tableTodayBusiness->setColumnWidth(2,89);
    ui->tableTodayBusiness->setColumnWidth(3,89);
    ui->tableTodayBusiness->setHorizontalHeaderLabels({"Початок", "Кінець","Заняття", "Місце"});
    ui->tableTodayBusiness->horizontalHeader()->setVisible(true);
    ui->tableTodayBusiness->verticalHeader()->setVisible(false);

    ui->tableTodayBusiness->setRowCount(20);

    for (int i=0; i<int(today_study.size()); i++)
    {
        ui->tableTodayBusiness->setItem(i, 0, new QTableWidgetItem(today_study[i].getTimeBeg().toString("HH:mm")));
        ui->tableTodayBusiness->setItem(i, 1, new QTableWidgetItem(today_study[i].getTimeEnd().toString("HH:mm")));
        ui->tableTodayBusiness->setItem(i, 2, new QTableWidgetItem(today_study[i].getName()));
        ui->tableTodayBusiness->setItem(i, 3, new QTableWidgetItem(today_study[i].getPlace()));
    }

    ui->tableTasks->setColumnCount(3);
    ui->tableTasks->setColumnWidth(0,210);
    ui->tableTasks->setColumnWidth(1,200);
    ui->tableTasks->setColumnWidth(2,200);
    ui->tableTasks->setHorizontalHeaderLabels({"Задача","Дедлайн","Витрачено часу"});
    ui->tableTasks->horizontalHeader()->setVisible(true);
    ui->tableTasks->verticalHeader()->setVisible(false);
    ui->tableTasks->setRowCount(20); //DB loading ЗАГРУЗКА ЗАДАЧ

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

    ui->tablePersonalLifeEvents->setColumnCount(4);
    ui->tablePersonalLifeEvents->setColumnWidth(0,61);
    ui->tablePersonalLifeEvents->setColumnWidth(1,61);
    ui->tablePersonalLifeEvents->setColumnWidth(2,89);
    ui->tablePersonalLifeEvents->setColumnWidth(3,89);
    ui->tablePersonalLifeEvents->setHorizontalHeaderLabels({"Початок", "Кінець", "Подія", "Місце"});
    ui->tablePersonalLifeEvents->horizontalHeader()->setVisible(true);
    ui->tablePersonalLifeEvents->verticalHeader()->setVisible(false);
    ui->textEdit->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->calendarPersonalLife->setSelectedDate(QDate::currentDate());

    m_is_valid = true;

    QString scrollbarSettings = QString::fromUtf8("QScrollBar:vertical {"
                                                  "    border: 1px solid #ffffff;"
                                                  "    background:white;"
                                                  "    width:10px;    "
                                                  "    margin: 0px 0px 0px 0px;"
                                                  "}"
                                                  "QScrollBar::handle:vertical {"
                                                  "    background: #FFD65E;"
                                                  "    min-height: 0px;"
                                                  "}"
                                                  "QScrollBar::add-line:vertical {"
                                                  "    background: #C4A872;"
                                                  "    height: 0px;"
                                                  "    subcontrol-position: bottom;"
                                                  "    subcontrol-origin: margin;"
                                                  "}"
                                                  "QScrollBar::sub-line:vertical {"
                                                  "    background: #FFD65E;"
                                                  "    height: 0 px;"
                                                  "    subcontrol-position: top;"
                                                  "    subcontrol-origin: margin;"
                                                  "}"
                                                  );
    ui->tableHomeWork->verticalScrollBar()->setStyleSheet(scrollbarSettings);
    ui->tablePersonalLifeEvents->verticalScrollBar()->setStyleSheet(scrollbarSettings);
    ui->tableTasks->verticalScrollBar()->setStyleSheet(scrollbarSettings);
    ui->tableTodayBusiness->verticalScrollBar()->setStyleSheet(scrollbarSettings);
    ui->tableTodayPesonalLife->verticalScrollBar()->setStyleSheet(scrollbarSettings);
}

MainWindow::~MainWindow()
{
    if( sch )
    delete sch;

    if( timer )
        delete timer;

    if( dbm )
    {
        delete dbm;
        dbm = nullptr;
        QSqlDatabase::removeDatabase( "tasklist" );
    }
    delete ui;
}

bool MainWindow::IsValid( void )
{
    return m_is_valid;
}

// Обробник натискання дати у календарі
void MainWindow::on_calendarPersonalLife_clicked(const QDate &date)
{
    ui->textEdit->setText(date.toString("dd.MM.yyyy"));

    ui->tablePersonalLifeEvents->clearContents();

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
        ui->tablePersonalLifeEvents->setItem(i, 1, new QTableWidgetItem(study[i].getTimeEnd().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(i, 2, new QTableWidgetItem(study[i].getName()));
        ui->tablePersonalLifeEvents->setItem(i, 3, new QTableWidgetItem(study[i].getPlace()));
    }
    for (int i=0; i<int(meet.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 0, new QTableWidgetItem(meet[i].getTimeBeg().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 1, new QTableWidgetItem(meet[i].getTimeEnd().toString("HH:mm")));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 2, new QTableWidgetItem(meet[i].getName()));
        ui->tablePersonalLifeEvents->setItem(int(study.size())+i, 3, new QTableWidgetItem(meet[i].getPlace()));
    }
    for (int i=0; i<int(bd.size()); i++)
    {
        ui->tablePersonalLifeEvents->setItem(int(study.size())+int(meet.size())+i, 2, new QTableWidgetItem(bd[i].getName()));
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

   int amount = int(selItemsList.size())/4;
   int current = 0;

   try {

   for (int l = 0; l<amount; l++)
   {
       QString str_time = selItemsList[current]->text();
       QString name = selItemsList[current+2]->text();

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
       if (i == -1) throw Exception(2);

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
   catch (Exception &e)
   {
       e.show();
   }
}

// Обробник додавання події
void MainWindow::on_buttonAddPersonalLife_clicked()
{
    try {
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

               if (eventLocation == "" || eventDescription == "") throw Exception(4);
               if (eventStartTime > eventEndTime) throw Exception(5);

               Study s(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, EVENT_STUDY);
               bool isB = sch->isBusy(eventDate.date(), eventStartTime, eventEndTime);
               if (isB) throw Exception(1);
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

               if (eventDescription == "") throw Exception(4);
               if (eventStartTime > eventEndTime) throw Exception(5);
               if (eventDeadlineTime < QDateTime::currentDateTime()) throw Exception(6);

               Task t(QDateTime(), eventDeadlineTime, weight, eventDescription, eventDateAndNotificationTime, EVENT_TASK, eventNumPriority);

               dbm->AddTask(t);
               sch->AddTask(QDateTime(), eventDeadlineTime, weight, eventDescription, eventDateAndNotificationTime, EVENT_TASK, eventNumPriority);

               vector<Task> task; sch->GetTask(task);
               ui->tableTasks->clearContents();
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

               if (eventLocation == "" || eventDescription == "") throw Exception(4);
               if (eventStartTime > eventEndTime) throw Exception(5);

               Meet m(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, eventDateAndNotificationTime, EVENT_MEET);
               bool isB = sch->isBusy(eventDate.date(), eventStartTime, eventEndTime);
               if (isB) throw Exception(1);
               dbm->AddMeet(m);
               sch->AddMeet(eventDate, eventStartTime, eventEndTime, eventDescription, eventLocation, eventDateAndNotificationTime, EVENT_MEET);
               break;
           }
           case Events::BIRTHDAY:
           {
               eventDate = dialog.get_eventDate();
               eventDateAndNotificationTime = dialog.get_eventDateAndNotificationTime();

               if (eventDescription == "") throw Exception(4);

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
    catch (bad_alloc)
    {
        ErrorDialog errDialog("Allocation failure!");
        errDialog.exec();
    }
    catch (Exception &e)
    {
        e.show();
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
    ui->tableTasks->clearContents();
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
    //if(column == 1){
        Qt::CheckState cur = ui->tableHomeWork->item(row,column)->checkState();
        vector<Hometask> hometasks = dbm->GetHometask(ui->dateEdit->date());
        for(int i = 0; i<int(hometasks.size());i++){
            if(hometasks[i].name == ui->tableHomeWork->item(row,0)->text()){
                if(cur != hometasks[i].completed){
                    hometasks[i].completed = cur;
                    dbm->ModifyHometask(hometasks[i]);
                    break;
                }
            }
        //}
    }
}

void MainWindow::on_buttonRemoveHomeWork_clicked()
{
    QList<QTableWidgetItem*>selItemsList = ui->tableHomeWork->selectedItems();

    QString name = selItemsList[0]->text();
    vector<Hometask> hometasks = dbm->GetHometask(ui->dateEdit->date());
    for(int i = 0; i<int(hometasks.size());i++)
        if(hometasks[i].name == name)
            dbm->DeleteHometask(hometasks[i]);

    ui->tableHomeWork->clearContents();
    ui->tableHomeWork->model()->removeRows(0, ui->tableHomeWork->rowCount());

    //ui->tableHomeWork->setRowCount(current_hometask.size());

    vector <Hometask> current_hometask = dbm->GetHometask(ui->dateEdit->date());
    for(int i = 0; i < (int)current_hometask.size();i++){
        ui->tableHomeWork->insertRow ( ui->tableHomeWork->rowCount() );
        ui->tableHomeWork->setItem   ( ui->tableHomeWork->rowCount()-1, 0, new QTableWidgetItem(current_hometask[i].name));
        ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
        QTableWidgetItem* pItem(ui->tableHomeWork->item(ui->tableHomeWork->rowCount()-1,0));
        pItem->setCheckState(Qt::CheckState(current_hometask[i].completed));
   }
}

void MainWindow::on_buttonAddHomeWork_clicked()
{
   AddHomeWork d;
   if(d.exec()==QDialog::Accepted){
       QString descript = d.get_homework();
       QDate date = d.get_date();
       Hometask hmtsk;
       hmtsk.name= descript;
       hmtsk.delivery_day = date;
       hmtsk.completed = false;
       dbm->AddHometask(hmtsk);

       if (date == ui->dateEdit->date())
       {
           vector <Hometask> current_hometask = dbm->GetHometask(ui->dateEdit->date());

           ui->tableHomeWork->clearContents();
           ui->tableHomeWork->model()->removeRows(0, ui->tableHomeWork->rowCount());

           for(int i = 0; i < (int)current_hometask.size();i++){
              ui->tableHomeWork->insertRow ( ui->tableHomeWork->rowCount() );
              ui->tableHomeWork->setItem   ( ui->tableHomeWork->rowCount()-1, 0, new QTableWidgetItem(current_hometask[i].name));
              ui->tableHomeWork->setCellWidget(i, 1, new QCheckBox);
              QTableWidgetItem* pItem(ui->tableHomeWork->item(ui->tableHomeWork->rowCount()-1,0));
              pItem->setCheckState(Qt::CheckState(current_hometask[i].completed));
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

void MainWindow::exportDB(QString fileExtension)
{
    QString fileExt;

    if(fileExtension=="CSV") fileExt = "CSV file (*.csv)";
    else if(fileExtension == "JSON")fileExt =  "JSON file (*.json)";
    else return;

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save DataBase"), "dataBase",
            fileExt);
    if(fileName.contains(".csv")){
        dbm->ExportData_CSV( fileName, ',' );
    }
    else if(fileName.contains(".json")){
        dbm->ExportData_JSON( fileName);
    }
}


void MainWindow::on_tableScheduleUniversity_cellChanged(int row, int column)
{
    for( unsigned int i = 0; i < university_schedule.size(); i++ )
    {
        if( ( university_schedule[i].week_day == ( row + 1 ) ) &&
            ( university_schedule[i].lesson_number == ( column + 1) ) )
        {
            if( ui->tableScheduleUniversity->item( row, column )->text() == "" )
            {
                dbm->DeleteLesson( university_schedule[i] );
                university_schedule.erase( university_schedule.begin() + i );
                return;
            }
            else
            {
                university_schedule[i].name = ui->tableScheduleUniversity->item( row, column )->text();
                dbm->ModifyLesson( university_schedule[i] );
                university_schedule[i].name = ui->tableScheduleUniversity->item( row, column )->text();
                return;
            }
        }
    }

    Lessons new_lesson;
    new_lesson.name = ui->tableScheduleUniversity->item( row, column )->text();
    new_lesson.week_day = row + 1;
    new_lesson.lesson_number = column + 1;
    dbm->AddLesson( new_lesson );
    university_schedule.push_back( new_lesson );
}

// Оновлення таблиць на першій вкладці
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 0){

        QDate d = QDate::currentDate();
        vector<Study> today_study;
        sch->GetStudy(today_study, d);
        vector<Meet> today_meet;
        sch->GetMeet(today_meet, d);
        vector<Birthday> today_bd;
        sch->GetBD(today_bd, d);
        vector<Task> task;
        sch->GetTask(task);

        ui->tableTodayPesonalLife->clearContents();
        ui->tableTodayBusiness->clearContents();

        for (int i=0; i<int(today_meet.size()); i++)
        {
            ui->tableTodayPesonalLife->setItem(i, 0, new QTableWidgetItem(today_meet[i].getTimeBeg().toString("HH:mm")));
            ui->tableTodayPesonalLife->setItem(i, 1, new QTableWidgetItem(today_meet[i].getTimeEnd().toString("HH:mm")));
            ui->tableTodayPesonalLife->setItem(i, 2, new QTableWidgetItem(today_meet[i].getName()));
            ui->tableTodayPesonalLife->setItem(i, 3, new QTableWidgetItem(today_meet[i].getPlace()));
        }
        for (int i=0; i<int(today_bd.size()); i++)
        {
            ui->tableTodayPesonalLife->setItem(int(today_meet.size())+i, 2, new QTableWidgetItem(today_bd[i].getName()));
        }
        for (int i=0; i<int(today_study.size()); i++)
        {
            ui->tableTodayBusiness->setItem(i, 0, new QTableWidgetItem(today_study[i].getTimeBeg().toString("HH:mm")));
            ui->tableTodayBusiness->setItem(i, 1, new QTableWidgetItem(today_study[i].getTimeEnd().toString("HH:mm")));
            ui->tableTodayBusiness->setItem(i, 2, new QTableWidgetItem(today_study[i].getName()));
            ui->tableTodayBusiness->setItem(i, 3, new QTableWidgetItem(today_study[i].getPlace()));
        }
    }
}

void MainWindow::on_actionExportJSON_triggered()
{
    exportDB("JSON");
}

void MainWindow::on_actionExportCSV_triggered()
{
    exportDB("CSV");
}

void MainWindow::on_actionAboutUs_triggered()
{
    InfoDialog d("ПРО НАС");
    d.exec();
}

void MainWindow::on_actionHelp_triggered()
{
    InfoDialog d("ДОПОМОГА");
    d.exec();

}
