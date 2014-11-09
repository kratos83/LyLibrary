#include "fatt_acq.h"
#include "ui_fatt_acq.h"
#include "fatt_new.h"
#include <QtGui>
#include <QtSql>
#include <QMenu>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include "print.h"
#include "articoli.h"

fatt_acq::fatt_acq(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);


    /**********************************************
     *IMPOSTAZIONE AZIONI PUSHBUTTON
     *********************************************/
    agg_fattura_libri = new QAction("Fattura",this);
    agg_fattura_prod_dig = new QAction("Fattura_prodotti digitali",this);

    /**********************************************
     * Impostazione menu pushbutton
     **********************************************/
    menu = new QMenu(this);
    menu->addAction(agg_fattura_libri);
    menu->addSeparator();
    menu->addAction(agg_fattura_prod_dig);
    Aggiungi->setMenu(menu);

    /*********************************************
     * Connessioni ai diversi metodi
     *********************************************/
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(elimina,SIGNAL(clicked()),this,SLOT(elimina_fatt()));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(tab_view,SIGNAL(viewportEntered()),this,SLOT(clickgrid()));
    connect(tab_view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(aggiorna_fattura()));
    connect(modifica,SIGNAL(clicked()),this,SLOT(aggiorna_fattura()));
    connect(stampa,SIGNAL(clicked()),this,SLOT(stampa_fatt()));
    connect(cerca1,SIGNAL(textEdited(QString)),this,SLOT(cerca_dxt(const QString &)));
    connect(agg_fattura_libri,SIGNAL(triggered()),this,SLOT(nuova_fattura_libri()));
    connect(agg_fattura_prod_dig,SIGNAL(triggered()),this,SLOT(nuova_fattura_prodotti_dig()));
    connect(find_cer,SIGNAL(clicked()),this,SLOT(filtro()));

    /************************************************
     * Impostazione combBox cerca_txt
     ************************************************/
    QStringList list1;
    list1 << tr("Cerca per ID") << tr("Cerca per fornitore") << tr("Cerca per tipo fattura");
    cerca_txt->addItems(list1);

    id->setVisible(false);
    textEdit->setReadOnly(true);
    mod_grid = new QSqlRelationalTableModel;

    model = new QSortFilterProxyModel;

    tab_view->setCurrentIndex(tab_view->currentIndex());
    lista();
    salva_fattura();
}

fatt_acq::~fatt_acq()
{
}

void fatt_acq::cerca_dxt(const QString &){
    if(cerca_txt->currentIndex() == 0){
    QString text1;
    text1 = cerca1->text();
    QStringList list_st;
    QSqlQuery querys("select id from fatture_acq");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list_st,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    cerca1->setCompleter(complete);
    if(text1.length() == 0){
        lista();
    }
    }
    else if(cerca_txt->currentIndex() == 1){
        QString text2;
        text2 = cerca1->text();
        QStringList list_st1;
        QSqlQuery querys1("select fornitore from fatture_acq");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        cerca1->setCompleter(complete1);
        if(text2.length() == 0){
            lista();
        }
    }
    else if(cerca_txt->currentIndex() == 2){
        QString text3;
        text3 = cerca1->text();
        QStringList list_st2;
        QSqlQuery querys2("select tipo_fattura from fatture_acq");
        if(querys2.exec()){
        while(querys2.next()){
            list_st2 << querys2.value(0).toString();
        }
        }
        QCompleter *complete2 = new QCompleter(list_st2,this);
        complete2->setCaseSensitivity(Qt::CaseInsensitive);
        cerca1->setCompleter(complete2);
        if(text3.length() == 0){
            lista();
        }
    }
}

void fatt_acq::filtro(){
    if(cerca1->text().length() == 0){
        lista();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle("Lylibrary");
        MsgBox.setText("Avviso");
        MsgBox.setInformativeText(QString::fromUtf8("Inserisci il testo nella casella cerca"));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(cerca_txt->currentIndex() == 0){
        mod_grid->setFilter(QString("id = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(cerca_txt->currentIndex() == 1){
        mod_grid->setFilter(QString("fornitore = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(cerca_txt->currentIndex() == 2){
        mod_grid->setFilter(QString("tipo_fattura = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    clickgrid();
}

void fatt_acq::nuova_fattura_libri(){

    fatt_new *fs = new fatt_new(this);
    QSqlQuery query("select max(id)+1 from fatture_acq");
    query.exec();
    if(query.next()){
        if(query.value(0).toInt() == 0){
            fs->id->setText("1");
            fs->formatta_num();


    connect(fs,SIGNAL(salvafatturanew()),SLOT(lista()));
    fs->f_libri->setChecked(true);
    fs->f_prod_dig->setEnabled(false);
    fs->show();
    }
    else{
            fs->id->setText(query.value(0).toString());
            fs->formatta_num();

            connect(fs,SIGNAL(salvafatturanew()),SLOT(lista()));
            fs->f_libri->setChecked(true);
            fs->f_prod_dig->setEnabled(false);
            fs->show();
        }
    }
}

void fatt_acq::nuova_fattura_prodotti_dig()
{
    fatt_new *fs = new fatt_new(this);
    QSqlQuery query("select max(id)+1 from fatture_acq");
    query.exec();
    if(query.next()){
        if(query.value(0).toInt() == 0){
            fs->id->setText("1");
            fs->formatta_num();


    connect(fs,SIGNAL(salvafatturanew()),SLOT(lista()));
    fs->f_prod_dig->setChecked(true);
    fs->f_libri->setEnabled(false);
    fs->show();
    }
    else{
            fs->id->setText(query.value(0).toString());
            fs->formatta_num();

            connect(fs,SIGNAL(salvafatturanew()),SLOT(lista()));
            fs->f_prod_dig->setChecked(true);
            fs->f_libri->setEnabled(false);
            fs->show();
        }
    }
}

void fatt_acq::salva_fattura(){

    QSqlQuery query,query1;
    query.prepare("select count(id) from fatture_acq");
    query.exec();
    if(query.next()){
        QString text = QString::fromUtf8("Le fatture d'acquisto sono: ") + query.value(0).toString();
        tot_fatt->setText(text);
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Inizializzazione fallita... "+query.lastError().text());
    }

    query1.prepare("select sum(totale) from fatture_acq");
    query1.exec();
    if(query1.next()){
        QString txt = QString::fromUtf8("Il totale delle fatture d'acquisto è: ")+"€"+" "+query1.value(0).toString();
        tot_eur->setText(txt);
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Inizializzazione fallita... "+query1.lastError().text());
    }
}

void fatt_acq::aggiorna_fattura(){

    if(tab_view->selectionModel()->currentIndex().isValid()){
    QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    agg_fatt(newindex);
    }
    else
    {
        QMessageBox::warning(this,"LyLibrary","Seleziona una riga da modificare...");
    }
}

void fatt_acq::agg_fatt(QModelIndex index){

    fatt_new *ft = new fatt_new(this);
        if(index.row() < mod_grid->rowCount()){

        QSqlQuery query;
        query.prepare("select * from fatture_acq_righe where id='"+id->text()+"'");
        query.exec();
        ft->setModal(true);
        QSqlRecord rec = mod_grid->record(index.row());
        ft->id->setText(rec.value("id").toString());
        if(query.next()){
        ft->data_fat->setDate(rec.value("data").toDate());
        ft->forn->setCurrentText(query.value(2).toString());
        ft->sconto_1->setText(query.value(3).toString());
        ft->sconto_2->setText(query.value(4).toString());
        ft->sconto_3->setText(query.value(5).toString());
        ft->sp_tr->setText(query.value(6).toString());
        ft->sp_in->setText(query.value(7).toString());
        ft->t_pagam->setCurrentText(query.value(8).toString());
        ft->n_fatt->setText(query.value(9).toString());
        ft->dateEdit->setDate(query.value(10).toDate());
        ft->textEdit->setText(query.value(11).toString());
        QSqlQuery query1;
        query1.prepare("select tipo_fattura from fatture_acq where id='"+id->text()+"'");
        query1.exec();
        if(query1.next()){
            if(query1.value(0).toString() == "Acquisto libri"){
                ft->f_libri->setChecked(true);
                ft->f_prod_dig->setEnabled(false);
            }
            else{
                ft->f_prod_dig->setChecked(true);
                ft->f_libri->setEnabled(false);
            }
        }
        }
        ft->lista();
        lista();
        ft->tot_imp_iva_rg();
        ft->formatta_num();
        ft->exec();
    }
    salva_fattura();

    lista();
}

void fatt_acq::elimina_fatt(){

    if(tab_view->selectionModel()->currentIndex().isValid()){
        if (!id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM fatture_acq WHERE id = :id LIMIT 1");
            qctrl.bindValue(":id",id->text());
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
                         elimina_riga();
                         box->close();
                         break;
                    case QMessageBox::Cancel:
                        //return
                        box->close();
                        break;
                    }


        }
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare");
    }

}

void fatt_acq::elimina_riga(){
    // Se si passano i controlli di cui sopra si procede all'eliminazione

    QSqlQuery query,query1,query2;

    query.prepare("DELETE FROM fatture_acq WHERE id = :id LIMIT 1");
    query.bindValue(":id",id->text());

    query1.prepare("DELETE FROM fatture_righe_acq_art WHERE id = :id");
    query1.bindValue(":id",id->text());

    query2.prepare("DELETE FROM fatture_acq_righe where id=:id");
    query2.bindValue(":id",id->text());

    if (query.exec() & query1.exec() & query2.exec()) //Se esito OK Eliminato da DB
    {
        id->setText("");
    }
    else
    {
        //scrivere codice per gestione Errore cancellazione
        qWarning()<<query.lastError();
     }
    lista();
    salva_fattura();
    textEdit->setText("");
}



void fatt_acq::cerca(){

    QSqlQuery query;
    query.prepare("select * from fatture_acq where id=:id");
    query.bindValue(":id",id->text());
    query.exec();
    if(query.next()){
        textEdit->setText(QString::fromUtf8("Informazioni della fattura d'acquisto: ")+"\n"
                          +tr("ID:")+query.value(0).toString()+"\n"
                          +tr("Data: ")+query.value(1).toString()+"\n"
                          +tr("Fornitore: ")+query.value(2).toString()+"\n"
                          +tr("Tipo fattura: ")+query.value(3).toString()+"\n"
                          +tr("Totale: ")+QString::fromUtf8("\u20AC")+" "+query.value(4).toString()+"\n");
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile cercare...\n"+query.lastError().text());
    }
}

void fatt_acq::clickgrid(){

    QString id_new;
    int riga = tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());
    id_new = mod_grid->data(elemento).toString();
    id->setText(id_new);
    cerca();
}

void fatt_acq::lista(){

    mod_grid->setTable("fatture_acq");

    model->setSourceModel(mod_grid);
    model->sort(0,Qt::AscendingOrder);

    mod_grid->setHeaderData(0,Qt::Horizontal,"ID");
    mod_grid->setHeaderData(1,Qt::Horizontal,"Data");
    mod_grid->setHeaderData(2,Qt::Horizontal,"Fornitore");
    mod_grid->setHeaderData(3,Qt::Horizontal,"Tipo fattura");
    mod_grid->setHeaderData(4,Qt::Horizontal,"Totale");

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tab_view->verticalHeader()->hide();
    tab_view->setModel(model);
    mod_grid->select();

    salva_fattura();
}

void fatt_acq::stampa_fatt(){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle("Anteprima di stampa.");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
    preview.setWindowIcon(icon);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_view(QPrinter*)));
    preview.exec();
}

void fatt_acq::preview_view(QPrinter *printer){

    Q_UNUSED(printer);
    Stampe *pr = new Stampe();
    pr->stampafattacq(printer,mod_grid);
}
