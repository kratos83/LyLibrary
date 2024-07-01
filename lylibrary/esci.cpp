#include "esci.h"
#include "ui_esci.h"

esci::esci(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::esci)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&esci::close);
    connect(ui->pushButton_2,&QPushButton::clicked,qApp,QApplication::quit);
}

esci::~esci()
{
    delete ui;
}
