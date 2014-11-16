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
    QMenu *menu = new QMenu(this);
    /******************************************************
     * Menu stampa inventario
     ******************************************************/
    QAction *st_inv_libri = new QAction(tr("Stampa inventario libri"),this);
    QAction *st_inv_prod = new QAction(tr("Stampa inventario prodotti digitali"),this);

    //Connetti pulsanti
    connect(st_inv_libri,SIGNAL(triggered()),this,SLOT(stampa_inv()));
    connect(st_inv_prod,SIGNAL(triggered()),this,SLOT(st_prod()));

    //Visualizza menu
    menu->addAction(st_inv_libri);
    menu->addSeparator();
    menu->addAction(st_inv_prod);
    ui->print_inv->setMenu(menu);

    /******************************************************
     * Menu esporta_pdf(); esporta_prod();
     ******************************************************/

    QMenu *menu_prod = new QMenu(this);

    /*******************************************************
     * Azioni di esportazione pdf
     *******************************************************/
    QAction *action_pdf_inv_libri = new QAction(tr("Esporta inventario libri in pdf"),this);
    QAction *action_pdf_inv_prod = new QAction(tr("Esporta inventario prodotti digitali in pdf"),this);

    //Connetti azioni
    connect(action_pdf_inv_libri,SIGNAL(triggered()),this,SLOT(esporta_pdf()));
    connect(action_pdf_inv_prod,SIGNAL(triggered()),this,SLOT(esporta_prod()));

    menu_prod->addAction(action_pdf_inv_libri);
    menu_prod->addSeparator();
    menu_prod->addAction(action_pdf_inv_prod);
    ui->print_pdf->setMenu(menu_prod);
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

void inventario::st_prod()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(ant_st_prod(QPrinter*)));
    preview.exec();
}

void inventario::ant_st_prod(QPrinter *printer)
{
    Q_UNUSED(printer);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from carico_prod_dig");

    export_inv *inv = new export_inv(this);
    inv->st_inv_prod(printer,model);
}

void inventario::esporta_prod()
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
       model->setQuery("select * from carico_prod_dig");
       export_inv *in = new export_inv(this);
       in->st_inv_prod(printer,model);
}

inventario::~inventario()
{
    delete ui;
}
