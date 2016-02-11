#include "inventario.h"
#include "ui_inventario.h"
#include "export_inv.h"
#include <QMenu>
#include <QFileDialog>

inventario::inventario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inventario)
{
    ui->setupUi(this);
    connect(ui->print_inv,SIGNAL(clicked()),this,SLOT(stampa_inv()));
    connect(ui->print_pdf,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
}

void inventario::stampa_inv()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_prev(QPrinter*)));
    preview.exec();
}

void inventario::preview_prev(QPrinter *printer)
{
    Q_UNUSED(printer);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from carico_libri,carico_prod_dig");

    export_inv *inv = new export_inv(this);
    inv->stampa_inventario(printer,model);

}

void inventario::esporta_pdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Esporta PDF"),
                                                    tr("*.pdf"), tr("Pdf Files(*.pdf);;Tutti i file(*.*)"));

    if (fileName.length() != 0) {
            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".pdf") < 0) {
                fileName += ".pdf";
            }
        }
       QPrinter *printer = new QPrinter(QPrinter::HighResolution);
       printer->setPageSize(QPrinter::A4);
       printer->setOrientation(QPrinter::Landscape);
       printer->setOutputFileName(fileName);
       QSqlQueryModel *model = new QSqlQueryModel(this);
       model->setQuery("select * from carico_libri");
       export_inv *in = new export_inv(this);
       in->stampa_inventario(printer,model);
}

inventario::~inventario()
{
    delete ui;
}
