#include "notificationdialog.h"
#include "ui_notificationdialog.h"

NotificationDialog::NotificationDialog(QString notificationStr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotificationDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon
    QObject::connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    this->setFixedSize(this->size());
    ui->lineEdit->setText(notificationStr);
}

NotificationDialog::~NotificationDialog()
{
    delete ui;
}
