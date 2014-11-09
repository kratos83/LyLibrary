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
    connect(car_lb,SIGNAL(clicked(bool)),this,SLOT(clicca_check()));
    connect(car_pr,SIGNAL(clicked(bool)),this,SLOT(clicca_true()));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ex_csv,SIGNAL(clicked()),this,SLOT(esporta_csv()));
    connect(ex_pdf,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
    connect(pr_st,SIGNAL(clicked()),this,SLOT(stampa_carico()));
    connect(del_rec,SIGNAL(clicked()),this,SLOT(elimina()));
    car_lb->setChecked(true);
    quan->setReadOnly(true);
    tot_car->setReadOnly(true);
    dettagli->setReadOnly(true);
    quan_art();
    lista_libri();
    totale_magazzino();
}

bool carico_mag::clicca_true()
{
        if(car_pr->isChecked())
        {
            car_lb->setChecked(false);
            lista_prod_dig();
            dettagli->clear();
        }

        return car_lb->isChecked();
}

bool carico_mag::clicca_check()
{
    if(car_lb->isChecked())
    {
        car_pr->setChecked(false);
        lista_libri();
        dettagli->clear();
    }

    return car_pr->isChecked();
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
    mod_grid->setHeaderData(8,Qt::Horizontal,QString::fromUtf8("Quantità"));
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

void carico_mag::lista_prod_dig()
{
    mod_grid->setTable("carico_prod_dig");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Codice a barre."));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Titolo."));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Descrizione."));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Autore"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("Lingua"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Categoria"));
    mod_grid->setHeaderData(7,Qt::Horizontal,tr("Collocazione"));
    mod_grid->setHeaderData(8,Qt::Horizontal,QString::fromUtf8("Quantità"));
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
    if(car_lb->isChecked() == true){
    cerca();
    }
    else if(car_lb->isChecked() == false){
        cerca_pr_dig();
    }
    else if(car_pr->isChecked() == true){
        cerca_pr_dig();
    }
    else if(car_pr->isChecked() == false){
        cerca();
    }
}

void carico_mag::cerca()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM carico_libri where cod_articolo=:cod_articolo");
        query.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
        query.exec();


        if (query.next())
        {
            dettagli->setText("Cod. Art.: "+query.value(0).toString()+"\n"
                               "Codice a barre: "+query.value(1).toString()+"\n"
                                  "Titolo: "+query.value(2).toString()+"\n"
                                  "Descrizione: "+query.value(3).toString()+"\n"
                                  "Autore: "+query.value(4).toString()+"\n"
                                  "Lingua: "+query.value(5).toString()+"\n"
                                  "Categoria libro: "+query.value(6).toString()+"\n"
                              "Info editore: "+query.value(13).toString()+"\n"
                              "Collocazione: "+query.value(7).toString()+"\n"
                              +QString::fromUtf8("Quantità: ")+query.value(8).toString()+"\n"
                          "Prezzo unitario: "+QString::fromUtf8("\u20AC")+" "+query.value(9).toString()+"\n"
                          "Prezzo senza iva: "+QString::fromUtf8("\u20AC")+" "+query.value(10).toString()+"\n"
                          "Prezzo con iva: "+QString::fromUtf8("\u20AC")+" "+query.value(11).toString()+"\n"
                          "Totale: "+QString::fromUtf8("\u20AC")+" "+query.value(12).toString());
        }
        else
        {
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

        }
}

void carico_mag::cerca_pr_dig()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM carico_prod_dig where cod_articolo=:cod_articolo");
        query.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
        query.exec();


        if (query.next())
        {
            dettagli->setText("Cod. Art.: "+query.value(0).toString()+"\n"
                               "Codice a barre: "+query.value(1).toString()+"\n"
                                  "Titolo: "+query.value(2).toString()+"\n"
                                  "Descrizione: "+query.value(3).toString()+"\n"
                                  "Autore: "+query.value(4).toString()+"\n"
                                  "Lingua: "+query.value(5).toString()+"\n"
                                  "Categoria libro: "+query.value(6).toString()+"\n"
                                  "Info editore: "+query.value(13).toString()+"\n"
                                  "Collocazione: "+query.value(7).toString()+"\n"
                              +QString::fromUtf8("Quantità: ")+query.value(8).toString()+"\n"
                          "Prezzo unitario: "+QString::fromUtf8("\u20AC")+" "+query.value(9).toString()+"\n"
                          "Prezzo senza iva: "+QString::fromUtf8("\u20AC")+" "+query.value(10).toString()+"\n"
                          "Prezzo con iva: "+QString::fromUtf8("\u20AC")+" "+query.value(11).toString()+"\n"
                          "Totale: "+QString::fromUtf8("\u20AC")+" "+query.value(12).toString());

        }
        else
        {
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

        }
}

void carico_mag::quan_art()
{
    QSqlQuery query,query1;
    query.prepare("SELECT SUM(A.quantita) from carico_libri=A");
    query1.prepare("SELECT SUM(B.quantita) from carico_prod_dig=B");
    query.exec();
    query1.exec();

    if(query.next() && query1.next()){
        double text = query.value(0).toInt()+query1.value(0).toInt();
        quan->setText(QString::fromUtf8("N°  ")+QString::number(text));
    }
    else{
        QMessageBox::warning(this,"LyLibrary",QString::fromUtf8("Impossibile instanziare la quantità di articoli presenti in magazzino..."));
    }
}

void carico_mag::totale_magazzino()
{
    QSqlQuery query,query1;
    query.prepare("SELECT SUM(A.totale) from carico_libri=A");
    query1.prepare("SELECT SUM(B.totale) from carico_prod_dig=B");
    query.exec();
    query1.exec();

    if(query.next() && query1.next()){
        double prezzo = query.value(0).toDouble()+query1.value(0).toDouble();
        tot_car->setText(QString::fromUtf8("\u20AC")+" "+QString::number(prezzo));
    }
    else{
        QMessageBox::warning(this,"LyLibrary",QString::fromUtf8("Impossibile instanziare il prezzo totale degli articoli presenti in magazzino..."));
    }
}

void carico_mag::esporta_csv()
{
     if(tab_view->selectionModel()->currentIndex().isValid()){
    QString fileName = QFileDialog::getSaveFileName(this, "Esporta CSV",
                                                    "*.csv", "CSV(*.csv);;Tutti i file(*.*)");

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
    QMessageBox::warning(this,"LyLibrary","Selezionare una riga da esportare in csv...");
    }
}

void carico_mag::esporta_pdf()
{
    if(tab_view->selectionModel()->currentIndex().isValid()){
    QString fileName = QFileDialog::getSaveFileName(this, "Esporta PDF",
                                                    "*.pdf", "Pdf Files(*.pdf);;Tutti i file(*.*)");

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
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da esportare in pdf...");
    }
}

void carico_mag::elimina()
{
    if(car_lb->isChecked()){
        elimina_record_libri();
    }
    else if(car_pr->isChecked())
    {
        elimina_record_prodotti();
    }
}

void carico_mag::elimina_record_libri()
{


        // Si controlla che il cd_voce non sia usato in Spese o Budget
    if(tab_view->selectionModel()->currentIndex().isValid()){
        QSqlQuery qctrl;
        qctrl.prepare("SELECT * FROM carico_libri where cod_articolo=:cod_articolo");
        qctrl.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
        qctrl.exec();


            QMessageBox *box= new QMessageBox(this);
            box->setWindowTitle("Lylibrary");
            box->setInformativeText("Vuoi eliminare veramente \n il record selezionato?....");
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
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare...");
    }
}

void carico_mag::elimina_riga_libri()
{
    QSqlQuery query,query1;
    query.prepare("DELETE FROM carico_libri where cod_articolo=:cod_articolo LIMIT 1");
    query1.prepare("DELETE FROM articoli where cod_articolo=:cod_articolo LIMIT 1");
    query.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
    query1.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());

    if (query.exec() && query1.exec()) //Se esito OK Eliminato da DB
    {
        lista_libri();
    }
    else
    {
        QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle("Lylibrary");
        box->setInformativeText("Impossibile eliminare \n il record selezionato?....");
        box->exec();
     }
    quan_art();
    totale_magazzino();
}

void carico_mag::elimina_record_prodotti()
{
        // Si controlla che il cd_voce non sia usato in Spese o Budget
    if(tab_view->selectionModel()->currentIndex().isValid()){
        QSqlQuery qctrl;
        qctrl.prepare("SELECT * FROM carico_prod_dig where cod_articolo=:cod_articolo LIMIT 1");
        qctrl.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
        qctrl.exec();


            QMessageBox *box= new QMessageBox(this);
            box->setWindowTitle("Lylibrary");
            box->setInformativeText("Vuoi eliminare veramente \n il record selezionato?....");
            box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            box->setDefaultButton(QMessageBox::Ok);
            int ret = box->exec();
            switch(ret){
            case QMessageBox::Ok:
                 //Close
                 elimina_riga_prodotti();
                 box->close();
                 break;
            case QMessageBox::Cancel:
                //return
                box->close();
                break;
            }
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare...");
    }

}

void carico_mag::elimina_riga_prodotti()
{
    QSqlQuery query,query1;
    query.prepare("DELETE FROM carico_prod_dig where cod_articolo=:cod_articolo LIMIT 1");
    query1.prepare("DELETE FROM prodotti_dvd where cod_prodotto=:cod_prodotto LIMIT 1");
    query.bindValue(":cod_articolo",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());
    query1.bindValue(":cod_prodotto",mod_grid->data(mod_grid->index(tab_view->selectionModel()->currentIndex().row(),tab_view->selectionModel()->currentIndex().column()),Qt::DisplayRole).toString());

    if (query.exec() && query1.exec()) //Se esito OK Eliminato da DB
    {
        lista_prod_dig();
    }
    else
    {
        QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle("Lylibrary");
        box->setInformativeText("Impossibile eliminare \n il record selezionato?....");
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
   preview.setWindowTitle("Anteprima di stampa.");
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
    //delete ui;
}
