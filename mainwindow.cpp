#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setText(QDate::currentDate().toString("dd.MM.yyyy"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calendarPersonalLife_clicked(const QDate &date)
{
    ui->textEdit->setText(date.toString("dd.MM.yyyy"));
}
