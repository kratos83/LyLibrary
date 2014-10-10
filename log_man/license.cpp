#include "license.h"
#include "ui_license.h"
#include <fstream>
#include <iostream>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QDate>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QProcess>

using namespace std;

license::license(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::license)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    connect(ui->canc,SIGNAL(clicked()),this,SLOT(esci()));
    connect(ui->next,SIGNAL(pressed()),SLOT(avanti()));
    connect(ui->back,SIGNAL(pressed()),SLOT(indietro()));
    connect(ui->accetto,SIGNAL(clicked(bool)),this,SLOT(button()));
    connect(ui->refur,SIGNAL(clicked(bool)),this,SLOT(page_stack()));
    QIcon ico(":/images/icons32x32.png");
    setWindowIcon(ico);
    setWindowTitle("Configurazione di avvio Lylibrary");
    ui->next->setEnabled(false);
    ui->back->setEnabled(false);
    ui->canc->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(0);
    myprocess = new QProcess(this);


    read_conf();
    load_style(settingsManager->generalValue("Tema/sel_tema",QVariant()).toString());
}

void license::button(){

    if(ui->accetto->isChecked()){
        ui->canc->setEnabled(false);
        ui->next->setEnabled(true);
        ui->canc->setEnabled(false);
        settingsManager->setGeneralValue("main/option1",ui->accetto->isChecked());
        settingsManager->setGeneralValue("main/versione","open source");
    }
    else{
        page_stack();
    }
}

void license::page_fine(){

    int pageNo = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageNo+1);

       if(ui->page_6->isVisible()){
           ui->back->setVisible(false);
           ui->next->setVisible(false);
           ui->canc->setEnabled(true);
       }


}

void license::avanti(){
    page_fine();
}

void license::page_stack(){

    if(ui->refur->isChecked()){
        ui->next->setEnabled(false);
        ui->back->setEnabled(false);
        ui->canc->setEnabled(true);
    }
    else{
        button();
    }
}

void license::indietro(){
    int pageNo = ui->stackedWidget->currentIndex();
       if(pageNo<ui->stackedWidget->count()+1)
           ui->stackedWidget->setCurrentIndex(pageNo-1);
            else ui->stackedWidget->setCurrentIndex(0);
}



void license::read_conf(){
        ui->next->setEnabled(false);
        ui->back->setEnabled(false);
        ui->canc->setEnabled(false);
        settingsManager->generalValue("main/option1",QVariant());
        settingsManager->generalValue("main/versione","open source");
}

void license::esci(){


        #ifdef Q_OS_LINUX
        QProcess *proc = new QProcess();
        proc->start("/opt/lylibrary/lylibrary");
        #endif
        #ifdef Q_OS_WIN
        QProcess *proc = new QProcess();
        proc->start("C:\\Lylibrary\\lylibrary.exe");
        #endif
        #ifdef Q_OS_MACX
        QProcess *proc = new QProcess();
        proc->start("/Applications/lylibrary.app");
        #endif
        exit(0);
}


void license::closeEvent(QCloseEvent *event){
    event->ignore();
}

void license::load_style(const QString &sheetName){

        QFile file(":/qss/" + sheetName.toLower() + ".qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());

        qApp->setStyleSheet(styleSheet);
        qApp->setStyle(styleSheet);
}
license::~license()
{
    delete ui;
}
