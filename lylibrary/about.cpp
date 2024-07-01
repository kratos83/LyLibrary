#include "about.h"
#include "ui_about.h"
#include <QSizePolicy>
#include <QUrl>
#include <QDesktopServices>
#include <QSettings>
#include "settingsmanager.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setVisible(true);
    connect(ui->cancella,SIGNAL(clicked()),this,SLOT(close_ab()));
    QString str;
    str = tr("Lylibrary ")+settingsManager->generalValue("Version/version",QVariant()).toString();
    QFont font("Arial",26,75);
    ui->label_2->setFont(font);
    ui->label_2->setText(str);
}

void about::close_ab(){
    this->close();
}

about::~about()
{
    delete ui;
}


