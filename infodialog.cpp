#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QString caption, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());//фіксований розмір вікна
    this->setWindowIcon(QIcon(":/rc/clockIcon"));//application icon

    ui->textAboutUs->setVisible(false);
    ui->textHelp->setVisible(false);

    if(caption == "ПРО НАС"){
        this->setWindowTitle("About us");
        ui->textAboutUs->setVisible(true);
    }
    else if(caption == "ДОПОМОГА"){
        this->setWindowTitle("Help");
        ui->textHelp->setVisible(true);
        ui->textHelp->verticalScrollBar()->setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                                                               "border: 0px solid #999999;"
                                                                           "border-radius: 4px;"
                                                                               "background:white;"
                                                                               "width:10px;"
                                                                               "margin: 6px 0px 6px 0px;"
                                                                           "}"
                                                                           "QScrollBar::handle:vertical {"
                                                                               "min-height: 0px;"
                                                                               "border: 0px solid transparent;"
                                                                               "border-radius: 4px;"
                                                                               "background-color: #ff911c;"
                                                                           "}"
                                                                           "QScrollBar::add-line:vertical {"
                                                                               "height: 0px;"
                                                                               "subcontrol-position: bottom;"
                                                                               "subcontrol-origin: margin;"
                                                                           "}"
                                                                           "QScrollBar::sub-line:vertical {"
                                                                               "height: 0 px;"
                                                                               "subcontrol-position: top;"
                                                                               "subcontrol-origin: margin;"
                                                                           "}"
                                                                              ));
    }
    else return;
    ui->caption->setText(caption);
    QObject::connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
}

InfoDialog::~InfoDialog()
{
    delete ui;
}
