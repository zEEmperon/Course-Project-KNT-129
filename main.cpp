#include <QApplication>
#include <QThread>
#include <QDebug>

#include "mainwindow.h"
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DBManager db;
    vector <Task> item1( db.GetTask( QDateTime::currentDateTime().addDays(-50), QDateTime::currentDateTime().addDays(50) ) );

    qDebug() << "BEFORE ACT: wt=" << item1[0].getWorkTime() << " dt=" << item1[0].getDate();
    item1[0].Activate();
    qDebug() << "AFTER  ACT: wt=" << item1[0].getWorkTime() << " dt=" << item1[0].getDate();
    db.ActivateTask(item1[0]);

    QThread::sleep( 62 );

    qDebug() << "BEFORE DEACT: wt=" << item1[0].getWorkTime() << " dt=" << item1[0].getDate();
    item1[0].Deactivate();
    qDebug() << "AFTER  DEACT: wt=" << item1[0].getWorkTime() << " dt=" << item1[0].getDate();
    db.DeactivateTask(item1[0]);

    return a.exec();
}
