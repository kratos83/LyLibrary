#include "read_pdf.h"
#include "ui_read_pdf.h"
#include <QtGui>
#include <QDebug>
#include <QFile>

static const int PageRole = Qt::UserRole + 1;

read_pdf::read_pdf(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::read_pdf)
{

    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
     scaleFactors << 0.25 << 0.5 << 0.75 << 1. << 1.25 << 1.5 << 2. << 3. << 4.;

    documentWidget = new open_pdf(this);
    ui->scrollArea->setWidget(documentWidget);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pageSpinBox, SIGNAL(valueChanged(int)),
                documentWidget, SLOT(setPage(int)));
        connect(documentWidget, SIGNAL(pageChanged(int)),
                ui->pageSpinBox, SLOT(setValue(int)));
        connect(ui->scaleComboBox, SIGNAL(currentIndexChanged(int)),
                   this, SLOT(zoom_document(int)));

    QString pa = ":/images/manuale_lylibrary.pdf";
    openfile(pa);
}

void read_pdf::openfile(QString path)
{
    if (documentWidget->setDocument(path)) {
        last_path = path;
        valuespin();
        zoom_document(5);
    } else{
        QMessageBox::warning(this, tr("LyLibrary"),
                             tr("Impossibile aprire il file..."));
        close();
    }

}

void read_pdf::zoom_document(int index)
{
    documentWidget->setScale(scaleFactors[index]);
}

void read_pdf::valuespin()
{
    ui->pageSpinBox->setEnabled(true);
    ui->pageSpinBox->setMinimum(1);
    ui->pageSpinBox->setMaximum(documentWidget->document()->numPages());
    ui->pageSpinBox->setValue(1);
}

read_pdf::~read_pdf()
{
    delete ui;
}


void read_pdf::on_indietro_clicked()
{
    if(ui->pageSpinBox->value() == 1)
        return;
    else
    documentWidget->previous_page(ui->pageSpinBox->value()-1);
}

void read_pdf::on_avanti_clicked()
{
    if(ui->pageSpinBox->value() >= documentWidget->document()->numPages())
        return;
    else
    documentWidget->next_page(ui->pageSpinBox->value()+1);
}
