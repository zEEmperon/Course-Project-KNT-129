#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tablePersonalLifeEvents->setColumnCount(2);
    ui->tablePersonalLifeEvents->setColumnWidth(0,125);
    ui->tablePersonalLifeEvents->setColumnWidth(1,125);
    ui->tablePersonalLifeEvents->setHorizontalHeaderLabels({"Час","Подія"});
    ui->tablePersonalLifeEvents->horizontalHeader()->setVisible(true);
    ui->tablePersonalLifeEvents->verticalHeader()->setVisible(false);


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
    //добавление события в таблицу и базу данных
}
