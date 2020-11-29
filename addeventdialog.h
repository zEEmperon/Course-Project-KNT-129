#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "eventsEnum.h"

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEventDialog(QWidget *parent = nullptr);
    ~AddEventDialog();

private slots:
    void on_rb_clicked();

    void on_AddEventDialog_finished(int result);

private:
    QButtonGroup* btnGroup;
    Ui::AddEventDialog *ui;
    Events selectedEvent;

    void disableAllTheWidgetsForInput();
};

#endif // ADDEVENTDIALOG_H
