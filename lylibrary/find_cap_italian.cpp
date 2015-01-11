#include "find_cap_italian.h"
#include "ui_find_cap_italian.h"
#include <QFile>
#include <QCompleter>
#include <QDebug>

find_cap_italian::find_cap_italian(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::find_cap_italian)
{
    ui->setupUi(this);
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->cerca,SIGNAL(clicked()),this,SLOT(cap_trovato()));
    this->setWindowTitle(tr("Cerca CAP"));
    find_comune();
}

void find_cap_italian::find_comune()
{
    QFile file(":/comuni_cap.txt");

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;

            QTextStream in(&file);
            QStringList list;
            QString com;
            for (int i = 0; i < 8120; i++)
            {
                    QString line = in.readLine();
                    com  = line.section(':', 0, 0);
                    ui->comune->addItem(com);
                    list << com;
            }

            QCompleter *complete = new QCompleter(list,this);
            complete->setCaseSensitivity(Qt::CaseInsensitive);
            ui->comune->setCompleter(complete);

}

void find_cap_italian::vis_cap_comune()
{
    int currentIndex = ui->comune->currentIndex();

    QFile file(":/comuni_cap.txt");

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;

            QTextStream in(&file);

            for (int i = 0; i < currentIndex; i++)
                    in.readLine();

            QString line = in.readLine();
            QString cod = line.section(':', 1, 1);
            cap = cod.left(6);
}

void find_cap_italian::cap_trovato()
{
    vis_cap_comune();

    found = cap;
    ui->vis_cap->clear();
    ui->vis_cap->setText(found);
}

find_cap_italian::~find_cap_italian()
{
    delete ui;
}
