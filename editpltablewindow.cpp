#include "editpltablewindow.h"
#include "ui_editpltablewindow.h"

EditPLTableWindow::EditPLTableWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPLTableWindow)
{
    ui->setupUi(this);
}

EditPLTableWindow::~EditPLTableWindow()
{
    delete ui;
}
