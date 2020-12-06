#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QString errorStr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon
    QObject::connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    this->setFixedSize(this->size());
    ui->lineEdit->setText(errorStr);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}
