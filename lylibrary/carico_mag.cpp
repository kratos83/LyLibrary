#include "carico_mag.h"
#include "itdelegato.h"
#include "print.h"
#include "ui_carico_mag.h"
#include <QMessageBox>
#include <QFileDialog>

carico_mag::carico_mag(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    mod_grid = new QSqlRelationalTableModel();
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ex_csv,SIGNAL(clicked()),this,SLOT(esporta_csv()));
    connect(ex_pdf,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
    connect(pr_st,SIGNAL(clicked()),this,SLOT(stampa_carico()));
    connect(del_rec,SIGNAL(clicked()),this,SLOT(elimina_record_libri()));
    quan->setReadOnly(true);
    tot_car->setReadOnly(true);
    dettagli->setReadOnly(true);
    quan_art();
    lista_libri();
    totale_magazzino();
}

void carico_mag::lista_libri()
{
    mod_grid->setTable("carico_libri");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Codice a barre."));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Titolo."));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Descrizione."));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Autore"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("Lingua"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Categoria"));
    mod_grid->setHeaderData(7,Qt::Horizontal,tr("Collocazione"));
    mod_grid->setHeaderData(8,Qt::Horizontal,QString::fromUtf8(tr("Quantità").toStdString().c_str()));
    mod_grid->setHeaderData(9,Qt::Horizontal,tr("Prezzo unitario"));
    mod_grid->setHeaderData(10,Qt::Horizontal,tr("Prezzo senza iva"));
    mod_grid->setHeaderData(11,Qt::Horizontal,tr("Prezzo con iva"));
    mod_grid->setHeaderData(12,Qt::Horizontal,tr("Totale"));
    mod_grid->setHeaderData(13,Qt::Horizontal,tr("Info editore"));
    mod_grid->setHeaderData(14, Qt::Horizontal, tr("Image"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tab_view->verticalHeader()->hide();
    tab_view->setModel(mod_grid);

    tab_view->setAlternatingRowColors(true);

    tab_view->setItemDelegateForColumn(14, new coldxdelegato(this));
}


void carico_mag::clickgrid()
{
    QString id_new;
    int riga = tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    cerca();
}

void carico_mag::cerca()
{
    QSqlQuery query;
    QString cod_articolo = mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString();
    query.prepare("SELECT * FROM carico_libri where cod_articolo='"+cod_articolo+"'");
        query.exec();

        QString cod_art = tr("Cod. Art.: ");
        QString cod_barre = tr("Codice a barre: ");
        QString title = tr("Titolo: ");
        QString desc = tr("Descrizione: ");
        QString autore = tr("Autore: ");
        QString lingua = tr("Lingua: ");
        QString cat_lb = tr("Categoria libro: ");
        QString info_ad = tr("Info editore: ");
        QString coll = tr("Collocazione: ");
        QString pr_un = tr("Prezzo unitario: ");
        QString pr_s_iva = tr("Prezzo senza iva: ");
        QString p_c_iva = tr("Prezzo con iva: ");
        QString tot = tr("Totale: ");
        if (query.next())
        {
              dettagli->setText(cod_art+query.value(0).toString()+"\n"+
                                cod_barre+query.value(1).toString()+"\n"+
                                title +query.value(2).toString()+"\n"+
                                desc+query.value(3).toString()+"\n"+
                                autore+query.value(4).toString()+"\n"+
                                lingua+query.value(5).toString()+"\n"+
                                cat_lb+query.value(6).toString()+"\n"+
                                info_ad+query.value(13).toString()+"\n"+
                                coll+query.value(7).toString()+"\n"
                                +QString::fromUtf8(tr("Quantità: ").toStdString().c_str())+query.value(8).toString()+"\n"+
                                pr_un+QString::fromUtf8("\u20AC")+" "+query.value(9).toString()+"\n"+
                                pr_s_iva+QString::fromUtf8("\u20AC")+" "+query.value(10).toString()+"\n"+
                                p_c_iva+QString::fromUtf8("\u20AC")+" "+query.value(11).toString()+"\n"+
                                tot+QString::fromUtf8("\u20AC")+" "+query.value(12).toString());
        }
        else
        {
            QMessageBox::warning(this,tr("LyLibrary"),QString::fromUtf8(tr("Impossibile cercare").toStdString().c_str()));
        }
}



void carico_mag::quan_art()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(quantita) from carico_libri");
    query.exec();

    if(query.next()){
        double text = query.value(0).toInt();
        quan->setText(QString::fromUtf8("N°  ")+QString::number(text));
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),QString::fromUtf8(tr("Impossibile instanziare la quantità di articoli presenti in magazzino...").toStdString().c_str()));
    }
}

void carico_mag::totale_magazzino()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(totale) from carico_libri");
    query.exec();

    if(query.next()){
        double prezzo = query.value(0).toDouble();
        tot_car->setText(QString::fromUtf8("\u20AC")+" "+QString::number(prezzo));
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),QString::fromUtf8(tr("Impossibile instanziare il prezzo totale degli articoli presenti in magazzino...").toStdString().c_str()));
    }
}

void carico_mag::esporta_csv()
{
     if(tab_view->selectionModel()->currentIndex().isValid()){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Esporta CSV"),
                                                    "*.csv", tr("CSV(*.csv);;Tutti i file(*.*)"));

    if (fileName.length() != 0) {
            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".csv") < 0) {
                fileName += ".csv";
            }
        }

    int x = 0;
    QString exportdata;

    int counthidden=0, jint = 0;

    while (jint < mod_grid->columnCount()) {

    counthidden+=tab_view->isColumnHidden(jint);
    jint++;
    }


    int w = 1;
    while (x < mod_grid->columnCount()){

    if (!tab_view->isColumnHidden(x)) {


    exportdata.append(mod_grid->headerData(x,Qt::Horizontal,Qt::DisplayRole).toString());


    if (mod_grid->columnCount() - w > counthidden)
    exportdata.append(";");
    else {
    exportdata.append("\n");

    }
    w++;
    }
    x++;

    }

    int z = 0;

    w = 1;
    while (z < mod_grid->rowCount()) {

    x = 0;

    w = 1;
    while (x < mod_grid->columnCount()) {
    if (!tab_view->isColumnHidden(x)) {


    exportdata.append(mod_grid->data(mod_grid->index(z,x),Qt::DisplayRole).toString());

    if (mod_grid->columnCount() - w > counthidden)
    exportdata.append(";");
    else
    exportdata.append("\n");

    w++;
    }
    x++;

    }

    z++;
    }





       QFile file;
       if (!fileName.isEmpty()) {
     file.setFileName(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
    }
    QByteArray ttext;
    ttext.append(exportdata);
    file.write(ttext);
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da esportare in csv..."));
    }
}

void carico_mag::esporta_pdf()
{
    if(tab_view->selectionModel()->currentIndex().isValid()){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Esporta PDF"),
                                                    "*.pdf", tr("Pdf Files(*.pdf);;Tutti i file(*.*)"));

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
       Stampe *st = new Stampe();
       st->esporta_pdf(printer,mod_grid);
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da esportare in pdf..."));
    }
}

void carico_mag::elimina_record_libri()
{


        // Si controlla che il cd_voce non sia usato in Spese o Budget
    if(tab_view->selectionModel()->currentIndex().isValid()){
        QSqlQuery qctrl;
	QString cod_articolo = mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString();
        qctrl.prepare("SELECT * FROM carico_libri where cod_articolo='"+cod_articolo+"'");
        qctrl.exec();


            QMessageBox *box= new QMessageBox(this);
            box->setWindowTitle(tr("Lylibrary"));
            box->setInformativeText(tr("Vuoi eliminare veramente \n il record selezionato?...."));
            box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            box->setDefaultButton(QMessageBox::Ok);
            int ret = box->exec();
            switch(ret){
            case QMessageBox::Ok:
                 //Close
                 elimina_riga_libri();
                 box->close();
                 break;
            case QMessageBox::Cancel:
                //return
                box->close();
                break;
            }
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
}

void carico_mag::elimina_riga_libri()
{
    QSqlQuery query,query1;
    QString cod_articolo = mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString();
    query.prepare("DELETE FROM carico_libri where cod_articolo='"+cod_articolo+"'");
    query1.prepare("DELETE FROM articoli where cod_articolo='"+cod_articolo+"'");
    
    if (query.exec() && query1.exec()) //Se esito OK Eliminato da DB
    {
        lista_libri();
    }
    else
    {
        QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle(tr("Lylibrary"));
        box->setInformativeText(tr("Impossibile eliminare \n il record selezionato?...."));
        box->exec();
     }
    quan_art();
    totale_magazzino();
}


void carico_mag::stampa_carico()
{
    //Aggiornamento query
   QPrinter printer(QPrinter::HighResolution);
   QPrintPreviewDialog preview(&printer);
   preview.setWindowFlags(Qt::Window);
   preview.setWindowTitle(tr("Anteprima di stampa."));
   QIcon icon;
   icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
   preview.setWindowIcon(icon);
   connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(print_carico(QPrinter*)));
   preview.exec();
}

void carico_mag::print_carico(QPrinter *printer)
{
    Q_UNUSED(printer);
    Stampe *st = new Stampe();
    st->esporta_pdf(printer,mod_grid);
}

carico_mag::~carico_mag()
{
}
