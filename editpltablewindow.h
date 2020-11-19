#ifndef EDITPLTABLEWINDOW_H
#define EDITPLTABLEWINDOW_H

#include <QWidget>

namespace Ui {
class EditPLTableWindow;
}

class EditPLTableWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditPLTableWindow(QWidget *parent = nullptr);
    ~EditPLTableWindow();

private:
    Ui::EditPLTableWindow *ui;
};

#endif // EDITPLTABLEWINDOW_H
