#include "inventario.h"
#include "ui_inventario.h"
#include "StampaBiblioteca.h"
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
    QTextDocument *doc = new QTextDocument;
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from articoli");
    StampaBiblioteca *in = new StampaBiblioteca(this);
    in->stampaBiblioteca(printer,model,doc);
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
    if(!fileName.isEmpty()){
       QPrinter *printer = new QPrinter(QPrinter::HighResolution);
       printer->setPageSize(QPageSize(QPageSize::A4));
       printer->setPageOrientation(QPageLayout::Landscape);
       printer->setOutputFileName(fileName);
       QTextDocument *doc = new QTextDocument;
       QSqlQueryModel *model = new QSqlQueryModel(this);
       model->setQuery("select * from articoli");
       StampaBiblioteca *in = new StampaBiblioteca(this);
       in->stampaBiblioteca(printer,model,doc);
    }
}

inventario::~inventario()
{
    delete ui;
}
