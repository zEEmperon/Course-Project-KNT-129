#ifndef NOTIFICATIONDIALOG_H
#define NOTIFICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class NotificationDialog;
}

class NotificationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotificationDialog(QString notificationStr, QWidget *parent = nullptr);
    ~NotificationDialog();

private:
    Ui::NotificationDialog *ui;
};

#endif // NOTIFICATIONDIALOG_H
