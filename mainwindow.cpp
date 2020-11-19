#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editpltablewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableTodayPesonalLife->setColumnCount(2);
    ui->tableTodayPesonalLife->setColumnWidth(0,125);
    ui->tableTodayPesonalLife->setColumnWidth(1,125);
    ui->tableTodayPesonalLife->setHorizontalHeaderLabels({"Час","Подія"});
    ui->tableTodayPesonalLife->horizontalHeader()->setVisible(true);
    ui->tableTodayPesonalLife->verticalHeader()->setVisible(false);
    ui->tableTodayPesonalLife->setRowCount(8/*тут должно быть указано число, которое представляет кол-во событий личной жизни в данный день*/); //DB loading

    ui->tableTodayUniversity->setColumnCount(2);
    ui->tableTodayUniversity->setColumnWidth(0,125);
    ui->tableTodayUniversity->setColumnWidth(1,125);
    ui->tableTodayUniversity->setHorizontalHeaderLabels({"Час","Пара"});
    ui->tableTodayUniversity->horizontalHeader()->setVisible(true);
    ui->tableTodayUniversity->verticalHeader()->setVisible(false);
    ui->tableTodayUniversity->setRowCount(8/*тут должно быть указано число, которое представляет кол-во событий личной жизни в данный день*/); //DB loading

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
    delete ui;
}


void MainWindow::on_calendarPersonalLife_clicked(const QDate &date)
{
    ui->textEdit->setText(date.toString("dd.MM.yyyy"));
    ui->tablePersonalLifeEvents->setRowCount(5/*тут должно быть указано число, которое представляет кол-во событий личной жизни в данный день*/); //DB loading
}

void MainWindow::on_buttonRemovePersonalLife_clicked()
{
   //удаление события из таблицы и базы данных
   /* QList<QTableWidgetItem*>selItemsList =*/ ui->tablePersonalLifeEvents->selectedItems();

}

void MainWindow::on_buttonAddPersonalLife_clicked()
{
    EditPLTableWindow *w = new EditPLTableWindow;
    //w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    //добавление события в таблицу и базу данных
}
