#ifndef ADDHOMEWORK_H
#define ADDHOMEWORK_H

#include <QDialog>

namespace Ui {
class AddHomeWork;
}

class AddHomeWork : public QDialog
{
    Q_OBJECT

public:
    explicit AddHomeWork(QWidget *parent = nullptr);
    QDate get_date();
    QString get_homework();
    ~AddHomeWork();

private slots:
    void on_AddHomeWork_finished(int result);

private:
    Ui::AddHomeWork *ui;
    QString homeWork;
    QDate* date;
};

#endif // ADDHOMEWORK_H
