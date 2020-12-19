#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "schedule.h"
#include "dbmanager.h"
#include "addeventdialog.h"
#include <QTimer>
#include "notificationdialog.h"
#include "errordialog.h"
#include "addhomework.h"
#include <QScrollBar>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    bool m_is_valid;
    Schedule* sch;
    DBManager* dbm;
    QTimer*timer;
    vector <Lessons> university_schedule;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool IsValid( void );

private slots:
    void on_calendarPersonalLife_clicked(const QDate &date);
    void on_buttonRemovePersonalLife_clicked();
    void on_buttonAddPersonalLife_clicked();
    void on_actionExport_triggered();
    void slotUpdateDateTime();
    void on_buttonBeginDo_clicked();
    void on_buttonEndDo_clicked();
    void on_buttonRemove_clicked();
    void on_buttonShowHomework_clicked();
    void on_tableHomeWork_cellChanged(int row, int column);
    void on_tableScheduleUniversity_cellChanged(int row, int column);

    void on_tabWidget_tabBarClicked(int index);

    void on_buttonRemoveHomeWork_clicked();

    void on_buttonAddHomeWork_clicked();

private:
    void displayTime();
};
#endif // MAINWINDOW_H
