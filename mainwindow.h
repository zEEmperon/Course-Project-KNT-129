#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "schedule.h"
#include "dbmanager.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Schedule* sch;
    DBManager* dbm;
    QTimer*timer;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calendarPersonalLife_clicked(const QDate &date);

    void on_buttonRemovePersonalLife_clicked();

    void on_buttonAddPersonalLife_clicked();

    void on_actionExport_triggered();

    void slotUpdateDateTime();

    void on_buttonBeginDo_clicked();
    void on_buttonEndDo_clicked();
};
#endif // MAINWINDOW_H
