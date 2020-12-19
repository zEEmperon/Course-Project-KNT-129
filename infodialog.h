#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include <QScrollBar>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QString caption, QWidget *parent = nullptr);
    ~InfoDialog();

private:
    Ui::InfoDialog *ui;
};

#endif // INFODIALOG_H
