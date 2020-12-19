#include <QApplication>
#include <QThread>
#include <QDebug>

#include "mainwindow.h"
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if( w.IsValid() )
    {
        w.show();
        return a.exec();
    }
    else
        return 1;
}
