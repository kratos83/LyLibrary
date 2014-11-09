/*************************************
 *Intestazione Qt4
 ************************************/
#include <QCompleter>
#include <QMessageBox>

/*************************************
 *Intestazione fattura_rg
 ************************************/
#include "fattura_rg.h"
#include "ui_fattura_rg.h"

#include "user.h"
#include "banche.h"
#include "fornitori.h"
#include "itdelegato.h"
#include "fattura_rg_art.h"
#include "fattura.h"
#include "print.h"


fattura_rg::fattura_rg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(annulla_2,SIGNAL(clicked()),this,SLOT(chiudi()));
    connect(cliente,SIGNAL(clicked()),this,SLOT(open_cliente()));
    connect(fornit,SIGNAL(clicked()),this,SLOT(open_fornitore()));
    connect(tipo_cliente,SIGNAL(currentIndexChanged(int)),this,SLOT(filtro()));
    connect(banca,SIGNAL(clicked()),this,SLOT(bank()));
    connect(vis_banca,SIGNAL(currentIndexChanged(int)),this,SLOT(open_banca(int)));
    connect(n_riga,SIGNAL(clicked()),this,SLOT(fattura_riga_open()));
    connect(md_riga,SIGNAL(clicked()),this,SLOT(modifica_riga()));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(tab_view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(modifica_riga()));
    connect(spes_tr,SIGNAL(textEdited(const QString &)),this,SLOT(spesa_trasp(const QString &)));
    connect(spes_in,SIGNAL(textEdited(const QString &)),this,SLOT(spesa_in(const QString &)));
    connect(sconto1,SIGNAL(textEdited(const QString &)),this,SLOT(sconto_1t(const QString &)));
    connect(sconto2,SIGNAL(textEdited(const QString &)),this,SLOT(sconto_2t(const QString &)));
    connect(sconto3,SIGNAL(textEdited(const QString &)),this,SLOT(sconto_3t(const QString &)));
    connect(agg_totali_2,SIGNAL(clicked()),this,SLOT(tot_imp_iva()));
    connect(el_riga,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(save_2,SIGNAL(clicked()),this,SLOT(fatt_inserisci()));
    connect(agg_fatt_2,SIGNAL(clicked()),this,SLOT(fatt_aggiorna()));
    connect(pagam,SIGNAL(currentIndexChanged(int)),this,SLOT(tipo_pagamento()));
    cliente->setToolTip("Inserisci cliente");
    fornit->setToolTip("Inserisci fornitore");
    QStringList lis;
    lis << "Cliente " << "Fornitore";
    tipo_cliente->addItems(lis);

    //Setting data
    data_fatt->setCalendarPopup(true);
    data_fatt->setDate(QDate::currentDate());

    /************************
     * Avviare le istanze da lavorare
     ************************/
    mod_grid = new QSqlQueryModel(this);
    local_settings = new QLocale(QLocale::system());
    //Enable combobox text
    add_banca();
    filtro_banca();
    filtro();
    pagamento_fatt();
    lista_libri();
    if(f_libri_2->isChecked()){
    lista_libri();
    }
    else if(f_prod_dig_2->isChecked()){
        lista_prod_dig();
        f_prod_dig_2->setChecked(true);
    }

    visual_scadenze();
}

void fattura_rg::formatta_num(){

    QString sc1 = local_settings->toString(sconto1->text().toDouble(),'f',2);
    sconto1->setText(sc1);
    QString sc2 = local_settings->toString(sconto2->text().toDouble(),'f',2);
    sconto2->setText(sc2);
    QString sc3 = local_settings->toString(sconto3->text().toDouble(),'f',2);
    sconto3->setText(sc3);
    QString spese_tr = local_settings->toString(spes_tr->text().toDouble(),'f',2);
    spes_tr->setText(spese_tr);
    QString spese_in = local_settings->toString(spes_in->text().toDouble(),'f',2);
    spes_in->setText(spese_in);
    QString tot = local_settings->toString(totale_2->text().toDouble(),'f',2 );
    totale_2->setText(tot);
    QString iva = local_settings->toString(iva_ft_2->text().toDouble(),'f',2 );
    iva_ft_2->setText(iva);
    QString imp = local_settings->toString(imponibile_2->text().toDouble(),'f',2);
    imponibile_2->setText(imp);
}

void fattura_rg::filtro(){
    if(tipo_cliente->currentIndex() == 0){
        connect(pers,SIGNAL(currentIndexChanged(int)),this,SLOT(open_cl(int)));
        cliente_add();
        fornit->setEnabled(false);
        cliente->setEnabled(true);
    }
    else if(tipo_cliente->currentIndex() == 1){
        connect(pers,SIGNAL(currentIndexChanged(int)),this,SLOT(open_fl(int)));
        combo_fornitore();
        cliente->setEnabled(false);
        fornit->setEnabled(true);
    }
}

void fattura_rg::filtro_banca(){
        connect(vis_banca,SIGNAL(currentIndexChanged(int)),this,SLOT(open_banca(int)));
        add_banca();
}

void fattura_rg::open_cliente(){
    user *utente = new user(this);
    utente->setModal(true);

    connect(utente,SIGNAL(clienti_rel()),this,SLOT(cliente_add()));
    utente->exec();
}

void fattura_rg::bank(){
    banche *bc = new banche(this);
    bc->setModal(true);

    connect(bc,SIGNAL(banca_add()),this,SLOT(add_banca()));
    bc->exec();
}

void fattura_rg::add_banca(){


    vis_banca->clear();
    QStringList list;
    QSqlQuery query("select * from banca");
    if(query.exec()){
    while(query.next()){
        list << query.value(2).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(vis_banca->model());
    vis_banca->setCompleter(complete);
    vis_banca->addItems(list);

}

int fattura_rg::open_banca(int index)
{
    /******************************************************
     * Impostare index=-1
     * in modo da visualizzare correttamente la combobox
     ******************************************************/
    if(index== -1)
    {
        vis_banca->addItem("");
        vis_banca->removeItem(1);
        iban->setText("");
    }
    else{
    /**********************************************************
    * Visualizza i dati nella regione
    * antistante a destra.
    **********************************************************/
    QSqlQuery query("select * from banca");
    if(query.exec()){
    while(query.next()){
        if(index == 0){
            iban->setText("");
        }
        else if(index == query.value(0).toInt()){
        iban->setText(query.value(1).toString());
    }
    }
    }
    }
    return index;
}

void fattura_rg::cliente_add(){

    pers->clear();
    QStringList list;
    QSqlQuery query("select * from clienti");
    if(query.exec()){
    while(query.next()){
        list << query.value(1).toString() + " " + query.value(2).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(pers->model());
    pers->setCompleter(complete);
    pers->addItems(list);

}

int fattura_rg::open_cl(int index){

    /******************************************************
     * Impostare index=-1
     * in modo da visualizzare correttamente la combobox
     ******************************************************/
    if(index == -1){
        pers->addItem("");
        pers->removeItem(1);
        QString ragione = "";
        rag_soc->setText(ragione);
        indirizzo->setText("");
        tel->setText("");
        paese->setText("");
        cell->setText("");
    }
    else{
    /**********************************************************
     * Visualizza i dati nella regione
     * antistante a destra.
     **********************************************************/
    QSqlQuery query("select * from clienti");
    if(query.exec()){
    while(query.next()){

        if(index == 0)
        {
            QString ragione = "";
            rag_soc->setText(ragione);
            indirizzo->setText("");
            tel->setText("");
            paese->setText("");
            cell->setText("");
        }
        else if(index == query.value(0).toInt()){
        QString ragione = query.value(1).toString() +" "+query.value(2).toString();
        rag_soc->setText(ragione);
        indirizzo->setText(query.value(3).toString());
        tel->setText(query.value(4).toString());
        paese->setText(query.value(6).toString()+" "+query.value(7).toString());
        cell->setText(query.value(8).toString());
    }
    }
    }
    }

    return index;
}

void fattura_rg::open_fornitore(){
    fornitori *fo = new fornitori(this);
    fo->setModal(true);

    connect(fo,SIGNAL(vis_forn()),this,SLOT(combo_fornitore()));
    fo->exec();
}

void fattura_rg::combo_fornitore(){

    pers->clear();
    QStringList list;
    QSqlQuery query("select * from fornitori");
    if(query.exec()){
    while(query.next()){
        list << query.value(1).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(pers->model());
    pers->setCompleter(complete);
    pers->addItems(list);
}

int fattura_rg::open_fl(int index)
{

    /******************************************************
     * Impostare index=-1
     * in modo da visualizzare correttamente la combobox
     ******************************************************/
    if(index == -1){
        pers->addItem("");
        pers->removeItem(1);
        QString ragione = "";
        rag_soc->setText(ragione);
        indirizzo->setText("");
        tel->setText("");
        paese->setText("");
        cell->setText("");
    }
    else{
    /**********************************************************
     * Visualizza i dati nella regione
     * antistante a destra.
     **********************************************************/

    QSqlQuery query("select * from fornitori");
    if(query.exec()){
    while(query.next()){

        if(index == 0)
        {
            QString ragione = "";
            rag_soc->setText(ragione);
            indirizzo->setText("");
            tel->setText("");
            paese->setText("");
            cell->setText("");
        }
        else if(index == query.value(0).toInt()){
        rag_soc->setText(query.value(1).toString());
        indirizzo->setText(query.value(4).toString());
        cap->setText(query.value(5).toString());
        local->setText(query.value(6).toString());
        prov->setText(query.value(7).toString());
        tel->setText(query.value(8).toString());
        cell->setText(query.value(9).toString());
        paese->setText(query.value(2).toString());
        }
    }
    }
    }
    return index;
}

void fattura_rg::pagamento_fatt()
{
    pagam->clear();
    QStringList list;
    QSqlQuery query("select * from pagam");
    if(query.exec()){
    while(query.next()){
        list << query.value(1).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(pagam->model());
    pagam->setCompleter(complete);
    pagam->addItems(list);
}

void fattura_rg::lista_libri()
{
    mod_grid->setQuery("select cod_art,nome_articolo,descrizione,prezzo_unit,prezzo_s_iva,iva,prezzo_c_iva,quantita,totale from fatture_righe_vendita_art where id='"+id1->text()+"'");
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("Codice"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Prodotto"));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Descrizione"));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Prezzo unitario"));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Prezzo S. IVA"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("IVA"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Prezzo C. IVA"));
    mod_grid->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Quantità"));
    mod_grid->setHeaderData(8,Qt::Horizontal,tr("Totale"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tab_view->verticalHeader()->hide();
    tab_view->setModel(mod_grid);

    tab_view->setAlternatingRowColors(true);

    tab_view->setItemDelegateForColumn(7, new coldxdelegato(this));
}

void fattura_rg::lista_prod_dig()
{
    mod_grid->setQuery("select cod_art,nome_articolo,descrizione,prezzo_unit,prezzo_s_iva,iva,prezzo_c_iva,quantita,totale from fatture_righe_vendita_prod_dig where id='"+id1->text()+"'");
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("Codice"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Prodotto"));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Descrizione"));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Prezzo unitario"));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Prezzo S. IVA"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("IVA"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Prezzo C. IVA"));
    mod_grid->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Quantità"));
    mod_grid->setHeaderData(8,Qt::Horizontal,tr("Totale"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tab_view->verticalHeader()->hide();
    tab_view->setModel(mod_grid);

    tab_view->setAlternatingRowColors(true);

    tab_view->setItemDelegateForColumn(7, new coldxdelegato(this));
}

void fattura_rg::clickgrid()
{
    QString id_new;
    int riga = tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
}

void fattura_rg::fattura_riga_open()
{

    if(f_libri_2->isChecked() == true){
    fattura_rg_art *ft = new fattura_rg_art;
    ft->setModal(true);
    ft->setWindowTitle("Inserisci articoli fattura libri");
    connect(ft,SIGNAL(save_fatt()),this,SLOT(lista_libri()));
    connect(ft,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
    ft->id_fatt->setText(id1->text());
    ft->f_libri->setChecked(true);
    ft->exec();
    lista_libri();
    }

    if(f_prod_dig_2->isChecked() == true){
        fattura_rg_art *ft = new fattura_rg_art(this);
        ft->setModal(true);
        ft->setWindowTitle("Inserisci articoli fattura prodotti digitali");
        connect(ft,SIGNAL(save_fatt()),this,SLOT(lista_prod_dig()));
        connect(ft,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
        ft->id_fatt->setText(id1->text());
        ft->f_prod_dig->setChecked(true);
        ft->exec();
        lista_prod_dig();
    }
}

void fattura_rg::fatt_aggiorna()
{
    if(f_libri_2->isChecked())
    {
        if((pers->currentText()=="") ){
            QMessageBox MsgBox;
            MsgBox.setWindowTitle("LyLibrary");
            MsgBox.setText("Errore");
            MsgBox.setInformativeText(QString::fromUtf8("Impossibile inserire la fattura,\n poichè non hai inserito i dati correttamente..."));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
            pers->setStyleSheet("background-color: rgb(249, 22, 5)");
        }
        else{
            QSqlQuery query;
            query.prepare("UPDATE fatture_vendita_righe SET data=:data,fornitore=:fornitore,tipo_cliente=:tipo_cliente,sconto_1=:sconto_1,"
                          " sconto_2=:sconto_2,sconto_3=:sconto_3,spese_trasporto=:spese_trasporto,spese_incasso=:spese_incasso,annotazioni=:annotazioni,vs_ordine=:vs_ordine,tipo_pagamento=:tipo_pagamento,"
                          " note_pagamento=:note_pagamento,rag_soc=:rag_soc,indirizzo=:indirizzo,cap=:cap,localita=:localita,prov=:prov,telefono=:telefono,"
                          " fax=:fax, piva_cfis=:piva_cfis,banca=:banca,iban=:iban,agente=:agente,provvigione=:provvigione,prezzo_s_iva=:prezzo_s_iva,"
                          " iva=:iva,totale=:totale,scadenza_1=:scadenza_1,scadenza_2=:scadenza_2,scadenza_3=:scadenza_3,importo_1=:importo_1,importo_2=:importo_2,importo_3=:importo_3 "
                          " WHERE id =:id");

            query.bindValue(":id",id1->text());
            query.bindValue(":data",data_fatt->date());
            query.bindValue(":fornitore",pers->currentText());
            query.bindValue(":tipo_cliente",tipo_cliente->currentIndex());
            query.bindValue(":sconto_1",sconto1->text());
            query.bindValue(":sconto_2",sconto2->text());
            query.bindValue(":sconto_3",sconto3->text());
            query.bindValue(":spese_trasporto",spes_tr->text());
            query.bindValue(":spese_incasso",spes_in->text());
            query.bindValue(":annotazioni",ann->toPlainText());
            query.bindValue(":vs_ordine",ordine->text());
            query.bindValue(":tipo_pagamento",pagam->currentText());
            query.bindValue(":note_pagamento",lineEdit->text());
            query.bindValue(":rag_soc",rag_soc->text());
            query.bindValue(":indirizzo",indirizzo->text());
            query.bindValue(":cap",cap->text());
            query.bindValue(":localita",local->text());
            query.bindValue(":prov",prov->text());
            query.bindValue(":telefono",tel->text());
            query.bindValue(":fax",cell->text());
            query.bindValue(":piva_cfis",paese->text());
            query.bindValue(":banca",vis_banca->currentText());
            query.bindValue(":iban",iban->text());
            query.bindValue(":agente",lineEdit_2->text());
            query.bindValue(":provvigione",lineEdit_3->text());
            query.bindValue(":prezzo_s_iva",imponibile_2->text());
            query.bindValue(":iva",iva_ft_2->text());
            query.bindValue(":totale",totale_2->text());

            if(scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
            query.bindValue(":scadenza_1",scadenza_1->date());
            query.bindValue(":importo_1",totale_2->text());
            }
            else if(scadenza_1->isVisible() && scadenza_2->isVisible() && !scadenza_3->isVisible()){
                query.bindValue(":scadenza_1",scadenza_1->date());
                query.bindValue(":scadenza_2",scadenza_2->date());
                double div = totale_2->text().toDouble()/2;
                query.bindValue(":importo_1",div);
                query.bindValue(":importo_2",div);
            }
            else if(scadenza_1->isVisible() && scadenza_2->isVisible() && scadenza_3->isVisible()){
                query.bindValue(":scadenza_1",scadenza_1->date());
                query.bindValue(":scadenza_2",scadenza_2->date());
                query.bindValue(":scadenza_3",scadenza_3->date());
                double div = totale_2->text().toDouble()/3;
                query.bindValue(":importo_1",div);
                query.bindValue(":importo_2",div);
                query.bindValue(":importo_3",div);
            }
            else if(!scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
                query.bindValue(":scadenza_1","");
                query.bindValue(":scadenza_2","");
                query.bindValue(":scadenza_3","");
                query.bindValue(":importo_1","");
                query.bindValue(":importo_2","");
                query.bindValue(":importo_3","");
            }

            update_f_libri();

            if( query.exec()){
                QMessageBox::information(this,"LyLibrary","Aggiornamento effettuato correttamente...");
            }
            else{
                QMessageBox MsgBox;
                MsgBox.setText("La voce suddetta non si puo aggiornare");
                MsgBox.setInformativeText("Impossibile aggiornare "+query.lastError().text());
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
            }

            lista_libri();
            emit salvafattura();
            close();
            }
    }
    else if(f_prod_dig_2->isChecked())
    {
        if((pers->currentText()=="") ){
            QMessageBox MsgBox;
            MsgBox.setWindowTitle("LyLibrary");
            MsgBox.setText("Errore");
            MsgBox.setInformativeText(QString::fromUtf8("Impossibile inserire la fattura,\n poichè non hai inserito i dati correttamente..."));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
            pers->setStyleSheet("background-color: rgb(249, 22, 5)");
        }
        else{
            QSqlQuery query;
            query.prepare("UPDATE fatture_vendita_righe_dig SET data=:data,fornitore=:fornitore,tipo_cliente=:tipo_cliente,sconto_1=:sconto_1,"
                          " sconto_2=:sconto_2,sconto_3=:sconto_3,spese_trasporto=:spese_trasporto,spese_incasso=:spese_incasso,annotazioni=:annotazioni,vs_ordine=:vs_ordine,tipo_pagamento=:tipo_pagamento,"
                          " note_pagamento=:note_pagamento,rag_soc=:rag_soc,indirizzo=:indirizzo,cap=:cap,localita=:localita,prov=:prov,telefono=:telefono,"
                          " fax=:fax, piva_cfis=:piva_cfis,banca=:banca,iban=:iban,agente=:agente,provvigione=:provvigione,prezzo_s_iva=:prezzo_s_iva,"
                          " iva=:iva,totale=:totale,scadenza_1=:scadenza_1,scadenza_2=:scadenza_2,scadenza_3=:scadenza_3,importo_1=:importo_1,importo_2=:importo_2,importo_3=:importo_3 "
                          " WHERE id =:id");

            query.bindValue(":id",id1->text());
            query.bindValue(":data",data_fatt->date());
            query.bindValue(":fornitore",pers->currentText());
            query.bindValue(":tipo_cliente",tipo_cliente->currentIndex());
            query.bindValue(":sconto_1",sconto1->text());
            query.bindValue(":sconto_2",sconto2->text());
            query.bindValue(":sconto_3",sconto3->text());
            query.bindValue(":spese_trasporto",spes_tr->text());
            query.bindValue(":spese_incasso",spes_in->text());
            query.bindValue(":annotazioni",ann->toPlainText());
            query.bindValue(":vs_ordine",ordine->text());
            query.bindValue(":tipo_pagamento",pagam->currentText());
            query.bindValue(":note_pagamento",lineEdit->text());
            query.bindValue(":rag_soc",rag_soc->text());
            query.bindValue(":indirizzo",indirizzo->text());
            query.bindValue(":cap",cap->text());
            query.bindValue(":localita",local->text());
            query.bindValue(":prov",prov->text());
            query.bindValue(":telefono",tel->text());
            query.bindValue(":fax",cell->text());
            query.bindValue(":piva_cfis",paese->text());
            query.bindValue(":banca",vis_banca->currentText());
            query.bindValue(":iban",iban->text());
            query.bindValue(":agente",lineEdit_2->text());
            query.bindValue(":provvigione",lineEdit_3->text());
            query.bindValue(":prezzo_s_iva",imponibile_2->text());
            query.bindValue(":iva",iva_ft_2->text());
            query.bindValue(":totale",totale_2->text());

            if(scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
            query.bindValue(":scadenza_1",scadenza_1->date());
            query.bindValue(":importo_1",totale_2->text());
            }
            else if(scadenza_1->isVisible() && scadenza_2->isVisible() && !scadenza_3->isVisible()){
                query.bindValue(":scadenza_1",scadenza_1->date());
                query.bindValue(":scadenza_2",scadenza_2->date());
                double div = totale_2->text().toDouble()/2;
                query.bindValue(":importo_1",div);
                query.bindValue(":importo_2",div);
            }
            else if(scadenza_1->isVisible() && scadenza_2->isVisible() && scadenza_3->isVisible()){
                query.bindValue(":scadenza_1",scadenza_1->date());
                query.bindValue(":scadenza_2",scadenza_2->date());
                query.bindValue(":scadenza_3",scadenza_3->date());
                double div = totale_2->text().toDouble()/3;
                query.bindValue(":importo_1",div);
                query.bindValue(":importo_2",div);
                query.bindValue(":importo_3",div);
            }
            else if(!scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
                query.bindValue(":scadenza_1","");
                query.bindValue(":scadenza_2","");
                query.bindValue(":scadenza_3","");
                query.bindValue(":importo_1","");
                query.bindValue(":importo_2","");
                query.bindValue(":importo_3","");
            }

            update_f_prod_dig();

            if( query.exec()){
                QMessageBox::information(this,"LyLibrary","Aggiornamento effettuato correttamente...");
            }
            else{
                QMessageBox MsgBox;
                MsgBox.setText("La voce suddetta non si puo aggiornare");
                MsgBox.setInformativeText("Impossibile aggiornare "+query.lastError().text());
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
            }

            lista_prod_dig();
            emit salvafattura();
            close();
        }
    }
}

void fattura_rg::fatt_inserisci()
{

    if(f_libri_2->isChecked()){
    if((pers->currentText()=="") ){
        QMessageBox MsgBox;
        MsgBox.setWindowTitle("LyLibrary");
        MsgBox.setText("Errore");
        MsgBox.setInformativeText(QString::fromUtf8("Impossibile inserire la fattura,\n poichè non hai inserito i dati correttamente..."));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
        pers->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    QSqlQuery query;
    query.prepare("INSERT INTO fatture_vendita_righe (id,data,fornitore,tipo_cliente,sconto_1,sconto_2,sconto_3,spese_trasporto,spese_incasso,annotazioni,vs_ordine,tipo_pagamento,note_pagamento,rag_soc,indirizzo,cap,localita,prov,telefono,fax,piva_cfis,banca,iban,agente,provvigione,prezzo_s_iva,iva,totale,scadenza_1,scadenza_2,scadenza_3,importo_1,importo_2,importo_3)"
                  " VALUES(:id,:data,:fornitore,:tipo_cliente,:sconto_1,:sconto_2,:sconto_3,:spese_trasporto,:spese_incasso,:annotazioni,:vs_ordine,:tipo_pagamento,:note_pagamento,:rag_soc,:indirizzo,:cap,:localita,:prov,:telefono,:fax,:piva_cfis,:banca,:iban,:agente,:provvigione,:prezzo_s_iva,:iva,:totale,:scadenza_1,:scadenza_2,:scadenza_3,:importo_1,:importo_2,:importo_3) ");

    query.bindValue(":id",id1->text());
    query.bindValue(":data",data_fatt->date());
    query.bindValue(":fornitore",pers->currentText());
    query.bindValue(":tipo_cliente",tipo_cliente->currentIndex());
    query.bindValue(":sconto_1",sconto1->text());
    query.bindValue(":sconto_2",sconto2->text());
    query.bindValue(":sconto_3",sconto3->text());
    query.bindValue(":spese_trasporto",spes_tr->text());
    query.bindValue(":spese_incasso",spes_in->text());
    query.bindValue(":annotazioni",ann->toPlainText());
    query.bindValue(":vs_ordine",ordine->text());
    query.bindValue(":tipo_pagamento",pagam->currentText());
    query.bindValue(":note_pagamento",lineEdit->text());
    query.bindValue(":rag_soc",rag_soc->text());
    query.bindValue(":indirizzo",indirizzo->text());
    query.bindValue(":cap",cap->text());
    query.bindValue(":localita",local->text());
    query.bindValue(":prov",prov->text());
    query.bindValue(":telefono",tel->text());
    query.bindValue(":fax",cell->text());
    query.bindValue(":piva_cfis",paese->text());
    query.bindValue(":banca",vis_banca->currentText());
    query.bindValue(":iban",iban->text());
    query.bindValue(":agente",lineEdit_2->text());
    query.bindValue(":provvigione",lineEdit_3->text());
    query.bindValue(":prezzo_s_iva",imponibile_2->text());
    query.bindValue(":iva",iva_ft_2->text());
    query.bindValue(":totale",totale_2->text());

    if(scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
    query.bindValue(":scadenza_1",scadenza_1->date());
    query.bindValue(":importo_1",totale_2->text());
    }
    else if(scadenza_1->isVisible() && scadenza_2->isVisible() && !scadenza_3->isVisible()){
        query.bindValue(":scadenza_1",scadenza_1->date());
        query.bindValue(":scadenza_2",scadenza_2->date());
        double div = totale_2->text().toDouble()/2;
        query.bindValue(":importo_1",div);
        query.bindValue(":importo_2",div);
    }
    else if(scadenza_1->isVisible() && scadenza_2->isVisible() && scadenza_3->isVisible()){
        query.bindValue(":scadenza_1",scadenza_1->date());
        query.bindValue(":scadenza_2",scadenza_2->date());
        query.bindValue(":scadenza_3",scadenza_3->date());
        double div = totale_2->text().toDouble()/3;
        query.bindValue(":importo_1",div);
        query.bindValue(":importo_2",div);
        query.bindValue(":importo_3",div);
    }
    else if(!scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
        query.bindValue(":scadenza_1","");
        query.bindValue(":scadenza_2","");
        query.bindValue(":scadenza_3","");
        query.bindValue("importo_1","");
        query.bindValue("importo_2","");
        query.bindValue("importo_3","");
    }


        insert_f_libri();
        if(query.exec()){

        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText("La voce suddetta non si puo inserire");
            MsgBox.setInformativeText("Impossibile inserire "+query.lastError().text());
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        emit salvafattura();
        tot_imp_iva();
        close();
        }

    }
    else if(f_prod_dig_2->isChecked()){
        if((pers->currentText()=="") ){
            QMessageBox MsgBox;
            MsgBox.setWindowTitle("LyLibrary");
            MsgBox.setText("Errore");
            MsgBox.setInformativeText(QString::fromUtf8("Impossibile inserire la fattura,\n poichè non hai inserito i dati correttamente..."));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
            pers->setStyleSheet("background-color: rgb(249, 22, 5)");
        }
        else{
        QSqlQuery query;
        query.prepare("INSERT INTO fatture_vendita_righe_dig (id,data,fornitore,tipo_cliente,sconto_1,sconto_2,sconto_3,spese_trasporto,spese_incasso,annotazioni,vs_ordine,tipo_pagamento,note_pagamento,rag_soc,indirizzo,cap,localita,prov,telefono,fax,piva_cfis,banca,iban,agente,provvigione,prezzo_s_iva,iva,totale,scadenza_1,scadenza_2,scadenza_3,importo_1,importo_2,importo_3)"
                      " VALUES(:id,:data,:fornitore,:tipo_cliente,:sconto_1,:sconto_2,:sconto_3,:spese_trasporto,:spese_incasso,:annotazioni,:vs_ordine,:tipo_pagamento,:note_pagamento,:rag_soc,:indirizzo,:cap,:localita,:prov,:telefono,:fax,:piva_cfis,:banca,:iban,:agente,:provvigione,:prezzo_s_iva,:iva,:totale,:scadenza_1,:scadenza_2,:scadenza_3,:importo_1,:importo_2,:importo_3) ");

        query.bindValue(":id",id1->text());
        query.bindValue(":data",data_fatt->date());
        query.bindValue(":fornitore",pers->currentText());
        query.bindValue(":tipo_cliente",tipo_cliente->currentIndex());
        query.bindValue(":sconto_1",sconto1->text());
        query.bindValue(":sconto_2",sconto2->text());
        query.bindValue(":sconto_3",sconto3->text());
        query.bindValue(":spese_trasporto",spes_tr->text());
        query.bindValue(":spese_incasso",spes_in->text());
        query.bindValue(":annotazioni",ann->toPlainText());
        query.bindValue(":vs_ordine",ordine->text());
        query.bindValue(":tipo_pagamento",pagam->currentText());
        query.bindValue(":note_pagamento",lineEdit->text());
        query.bindValue(":rag_soc",rag_soc->text());
        query.bindValue(":indirizzo",indirizzo->text());
        query.bindValue(":cap",cap->text());
        query.bindValue(":localita",local->text());
        query.bindValue(":prov",prov->text());
        query.bindValue(":telefono",tel->text());
        query.bindValue(":fax",cell->text());
        query.bindValue(":piva_cfis",paese->text());
        query.bindValue(":banca",vis_banca->currentText());
        query.bindValue(":iban",iban->text());
        query.bindValue(":agente",lineEdit_2->text());
        query.bindValue(":provvigione",lineEdit_3->text());
        query.bindValue(":prezzo_s_iva",imponibile_2->text());
        query.bindValue(":iva",iva_ft_2->text());
        query.bindValue(":totale",totale_2->text());

        if(scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
        query.bindValue(":scadenza_1",scadenza_1->date());
        query.bindValue(":importo_1",totale_2->text());
        }
        else if(scadenza_1->isVisible() && scadenza_2->isVisible() && !scadenza_3->isVisible()){
            query.bindValue(":scadenza_1",scadenza_1->date());
            query.bindValue(":scadenza_2",scadenza_2->date());
            double div = totale_2->text().toDouble()/2;
            query.bindValue(":importo_1",div);
            query.bindValue(":importo_2",div);
        }
        else if(scadenza_1->isVisible() && scadenza_2->isVisible() && scadenza_3->isVisible()){
            query.bindValue(":scadenza_1",scadenza_1->date());
            query.bindValue(":scadenza_2",scadenza_2->date());
            query.bindValue(":scadenza_3",scadenza_3->date());
            double div = totale_2->text().toDouble()/3;
            query.bindValue(":importo_1",div);
            query.bindValue(":importo_2",div);
            query.bindValue(":importo_3",div);
        }
        else if(!scadenza_1->isVisible() && !scadenza_2->isVisible() && !scadenza_3->isVisible()){
            query.bindValue(":scadenza_1","");
            query.bindValue(":scadenza_2","");
            query.bindValue(":scadenza_3","");
            query.bindValue("importo_1","");
            query.bindValue("importo_2","");
            query.bindValue("importo_3","");

    }
        insert_f_prod_dig();
        if(query.exec()){

        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText("La voce suddetta non si puo inserire");
            MsgBox.setInformativeText("Impossibile inserire "+query.lastError().text());
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        emit salvafattura();
        tot_imp_iva();
        close();
        }
        }



}

void fattura_rg::insert_f_libri()
{
    QSqlQuery query1;
    query1.prepare("INSERT INTO fattura_vendita (id,data,fornitore,totale,tipo_fattura)"
                   " VALUES(:id,:data,:fornitore,:totale,:tipo_fattura)");

    query1.bindValue(":id",id1->text());
    query1.bindValue(":data",data_fatt->date());
    query1.bindValue(":fornitore",pers->currentText());
    query1.bindValue(":totale",totale_2->text());
    query1.bindValue(":tipo_fattura","Vendita libri");

    if(query1.exec()){
        QMessageBox::information(this,"LyLibrary","Inserimento effettuato correttamente...");
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile inserire "+query1.lastError().text());
    }
}

void fattura_rg::insert_f_prod_dig()
{
    QSqlQuery query1;
    query1.prepare("INSERT INTO fattura_vendita (id,data,fornitore,totale,tipo_fattura)"
                   " VALUES(:id,:data,:fornitore,:totale,:tipo_fattura)");

    query1.bindValue(":id",id1->text());
    query1.bindValue(":data",data_fatt->date());
    query1.bindValue(":fornitore",pers->currentText());
    query1.bindValue(":tipo_fattura","Prodotti digitali");
    query1.bindValue(":totale",totale_2->text());

    if(query1.exec()){
        QMessageBox::information(this,"LyLibrary","Inserimento effettuato correttamente...");
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile inserire "+query1.lastError().text());
    }
}

////////////////////////////////////////////////////////////////////////
void fattura_rg::update_f_libri()
{
    QSqlQuery query1;
    query1.prepare("UPDATE fattura_vendita SET data=:data,fornitore=:fornitore,totale=:totale,tipo_fattura=:tipo_fattura WHERE id=:id");
    query1.bindValue(":id",id1->text());
    query1.bindValue(":data",data_fatt->date());
    query1.bindValue(":fornitore",pers->currentText());
    query1.bindValue(":totale",totale_2->text());
    QString txt = tr("Vendita libri");
    query1.bindValue(":tipo_fattura",txt);

    if(query1.exec()){
        QMessageBox::information(this,"LyLibrary","Aggiornamento effettuato correttamente...");
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile inserire "+query1.lastError().text());
    }
}
/////////////////////////////////////////////////////////////////////////
void fattura_rg::update_f_prod_dig()
{
    QSqlQuery query1;
    query1.prepare("UPDATE fattura_vendita SET data=:data,fornitore=:fornitore,totale=:totale,tipo_fattura=:tipo_fattura WHERE id=:id");
    query1.bindValue(":id",id1->text());
    query1.bindValue(":data",data_fatt->date());
    query1.bindValue(":fornitore",pers->currentText());
    query1.bindValue(":totale",totale_2->text());
    QString txt = tr("Prodotti digitali");
    query1.bindValue(":tipo_fattura",txt);

    if(query1.exec()){
        QMessageBox::information(this,"LyLibrary","Aggiornamento effettuato correttamente...");
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile inserire "+query1.lastError().text());
    }
}

void fattura_rg::modifica_riga()
{
    if(tab_view->selectionModel()->currentIndex().isValid()){
    QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    modifica_riga_one(newindex);
    }
    else
    {
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da modificare...");
    }
}

void fattura_rg::modifica_riga_one(QModelIndex index)
{
    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){
        fattura_rg_art *gt = new fattura_rg_art(this);
        gt->setModal(true);
        QSqlRecord record = mod_grid->record(index.row());
        if(!record.isEmpty()){
            gt->id_fatt->setText(id1->text());
            gt->cod_art->setText(record.value("cod_art").toString());
            gt->art_nom->setText(record.value("nome_articolo").toString());
            gt->descrizione->setText(record.value("descrizione").toString());
            gt->pr_unit->setText(record.value("prezzo_unit").toString());
            gt->p_s_iva->setText(record.value("prezzo_s_iva").toString());
            gt->p_c_iva->setText(record.value("prezzo_c_iva").toString());
            gt->quant->setText(record.value("quantita").toString());
            gt->totale->setText(record.value("totale").toString());
            QSqlQuery query,query1,query2;
            query.prepare("select tipo_fattura from fatture_vendita");
            query1.exec("select * from articoli");
            query2.exec("select * from prodotti_dvd");
            query.exec();
            if(query.next()){
                if(query.value(0).toString() == "Vendita libri"){
                    gt->f_libri->setChecked(true);
                    if(query1.next()){
                     gt->cod_barre->setText(query1.value(1).toString());
                     gt->autore->setText(query1.value(4).toString());
                     gt->comboBox_2->setCurrentText(query1.value(6).toString());
                     gt->scaffale->setText(query1.value(7).toString());
                     gt->lingua->setText(query1.value(5).toString());
                     gt->textEdit->setText(query1.value(9).toString());
                     gt->image_dir->setText(query1.value(10).toString());
                     QImage img(query1.value(10).toString());
                     gt->dir_image->setPixmap(QPixmap::fromImage(img));
                     connect(gt,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
                     connect(gt,SIGNAL(save_fatt()),this,SLOT(lista_libri()));
                     lista_libri();
                    }
                }
                else if(query.value(0).toString() == "Prodotti digitali"){
                    gt->f_prod_dig->setChecked(true);
                if(query2.next()){
                 gt->cod_barre->setText(query2.value(1).toString());
                 gt->autore->setText(query2.value(4).toString());
                 gt->comboBox_2->setCurrentText(query2.value(6).toString());
                 gt->scaffale->setText(query2.value(7).toString());
                 gt->lingua->setText(query2.value(5).toString());
                 gt->textEdit->setText(query2.value(9).toString());
                 gt->image_dir->setText(query2.value(10).toString());
                 QImage image(query2.value(10).toString());
                 gt->dir_image->setPixmap(QPixmap::fromImage(image));
                 connect(gt,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
                 connect(gt,SIGNAL(save_fatt()),this,SLOT(lista_prod_dig()));
                 lista_prod_dig();
                }
                }
            }
        }
        connect(gt,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
        gt->exec();
    }
}

void fattura_rg::elimina()
{
    if(tab_view->selectionModel()->currentIndex().isValid()){
        if(f_libri_2->isChecked()){
        if (!id1->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM fatture_righe_vendita_art WHERE id = :id");
            qctrl.bindValue(":id",id1->text());
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
        else if(f_prod_dig_2->isChecked()){
            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM fatture_righe_vendita_prod_dig WHERE id = :id");
            qctrl.bindValue(":id",id1->text());
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

void fattura_rg::elimina_riga()
{
    if(f_libri_2->isChecked())
    {
        QSqlQuery query,query1;
        query.prepare("DELETE FROM fatture_righe_vendita_art WHERE id = :id ");
        query1.prepare("DELETE FROM fatture_vendita_righe WHERE id = :id ");
        query.bindValue(":id",id1->text());
        query1.bindValue(":id",id1->text());

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
        emit salvafattura();
        lista_libri();
        tot_imp_iva();
    }
    else if(f_prod_dig_2->isChecked())
    {
        QSqlQuery query,query1;
        query.prepare("DELETE FROM fatture_righe_vendita_prod_dig WHERE id = :id ");
        query1.prepare("DELETE FROM fatture_vendita_righe_dig WHERE id = :id ");
        query.bindValue(":id",id1->text());
        query1.bindValue(":id",id1->text());

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
        emit salvafattura();
        lista_prod_dig();
        tot_imp_iva();
    }
}

void fattura_rg::sconto_1t(const QString &)
{
    double sptr = local_settings->toDouble(sconto1->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile_2->text().toDouble()-((imponibile_2->text().toDouble()*sptr)/100);
    imponibile_2->setText(QString::number(sp));
    sp1 = iva_ft_2->text().toDouble()-((iva_ft_2->text().toDouble()*sptr)/100);
    iva_ft_2->setText(QString::number(sp1));
    sp2 = totale_2->text().toDouble()-((totale_2->text().toDouble()*sptr)/100);
    totale_2->setText(QString::number(sp2));

    if(sconto1->text().length() == 0){
        sconto1->setText("0.00");
        tot_imp_iva();
    }
}

void fattura_rg::sconto_2t(const QString &)
{
    double sptr = local_settings->toDouble(sconto2->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile_2->text().toDouble()-((imponibile_2->text().toDouble()*sptr)/100);
    imponibile_2->setText(QString::number(sp));
    sp1 = iva_ft_2->text().toDouble()-((iva_ft_2->text().toDouble()*sptr)/100);
    iva_ft_2->setText(QString::number(sp1));
    sp2 = totale_2->text().toDouble()-((totale_2->text().toDouble()*sptr)/100);
    totale_2->setText(QString::number(sp2));

    if(sconto2->text().length() == 0){
        sconto2->setText("0.00");
        tot_imp_iva();
    }
}

void fattura_rg::sconto_3t(const QString &)
{
    double sptr = local_settings->toDouble(sconto3->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile_2->text().toDouble()-((imponibile_2->text().toDouble()*sptr)/100);
    imponibile_2->setText(QString::number(sp));
    sp1 = iva_ft_2->text().toDouble()-((iva_ft_2->text().toDouble()*sptr)/100);
    iva_ft_2->setText(QString::number(sp1));
    sp2 = totale_2->text().toDouble()-((totale_2->text().toDouble()*sptr)/100);
    totale_2->setText(QString::number(sp2));

    if(sconto3->text().length() == 0){
        sconto3->setText("0.00");
        tot_imp_iva();
    }
}

void fattura_rg::spesa_trasp(const QString &)
{
    double sptr = local_settings->toDouble(spes_tr->text());
    double sp = 0,sp1 = 0,sp2=0; //(sptr*20)/100;

    sp = imponibile_2->text().toDouble()+sptr;
    imponibile_2->setText(QString::number(sp));
    sp1 = iva_ft_2->text().toDouble()+((sptr*20)/100);
    iva_ft_2->setText(QString::number(sp1));
    sp2 = totale_2->text().toDouble()+sptr-((sptr*20)/100);
    totale_2->setText(QString::number(sp2));

    if(spes_tr->text().length()==0){
        tot_imp_iva();
        spes_tr->setText("0.00");
    }
}

void fattura_rg::spesa_in(const QString &)
{
    double sptr = local_settings->toDouble(spes_in->text());
    double sp = 0,sp1 = 0,sp2=0; //(sptr*20)/100;

    sp = imponibile_2->text().toDouble()+sptr;
    imponibile_2->setText(QString::number(sp));
    sp1 = iva_ft_2->text().toDouble()+((sptr*20)/100);
    iva_ft_2->setText(QString::number(sp1));
    sp2 = totale_2->text().toDouble()+sptr-((sptr*20)/100);
    totale_2->setText(QString::number(sp2));

    if(spes_in->text().length()==0){
        tot_imp_iva();
        spes_in->setText("0.00");
    }
}

void fattura_rg::tot_imp_iva()
{
    if(f_libri_2->isChecked()){
    QSqlQuery Query1,Query2,Query3;
    QSqlQuery query;
    query.prepare("select id from fatture_righe_vendita_art where id= '"+id1->text()+"'");
    query.exec();
    if(query.next()){

        Query1.prepare("select sum(prezzo_s_iva) from fatture_righe_vendita_art where id='"+id1->text()+"'");
    Query1.exec();
    if(Query1.next()){
        imponibile_2->setText(QString::number(Query1.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il prezzo senza iva... "+Query1.lastError().text());
    }

    Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_righe_vendita_art where id='"+id1->text()+"'");
    Query2.exec();
    if(Query2.next()){
        iva_ft_2->setText(QString::number(Query2.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare l'iva... "+Query2.lastError().text());
    }


    Query3.prepare("select sum(totale) from fatture_righe_vendita_art where id ='"+id1->text()+"'");
    Query3.exec();
    if(Query3.next()){
        totale_2->setText(QString::number(Query3.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il totale... "+Query3.lastError().text());
    }
    }
    }
    else if(f_prod_dig_2->isChecked()){
        QSqlQuery Query1,Query2,Query3;
        QSqlQuery query;
        query.prepare("select id from fatture_righe_vendita_prod_dig where id= '"+id1->text()+"'");
        query.exec();
        if(query.next()){

            Query1.prepare("select sum(prezzo_s_iva) from fatture_righe_vendita_prod_dig where id='"+id1->text()+"'");
        Query1.exec();
        if(Query1.next()){
            imponibile_2->setText(QString::number(Query1.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il prezzo senza iva... "+Query1.lastError().text());
        }

        Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_righe_vendita_prod_dig where id='"+id1->text()+"'");
        Query2.exec();
        if(Query2.next()){
            iva_ft_2->setText(QString::number(Query2.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare l'iva... "+Query2.lastError().text());
        }


        Query3.prepare("select sum(totale) from fatture_righe_vendita_prod_dig where id ='"+id1->text()+"'");
        Query3.exec();
        if(Query3.next()){
            totale_2->setText(QString::number(Query3.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il totale... "+Query3.lastError().text());
        }
        }
    }
    emit salvafattura();
}



void fattura_rg::chiudi(){
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle("Lylibrary");
    box->setInformativeText("Sei sicuro di voler uscire dalla fattura?\n I dati non verranno salvati....");
    box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Close);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Close
         save_2->setChecked(true);
         save_2->setStyleSheet("background-color: rgb(249, 22, 5)");
         eli_riga();
         box->close();
         close();
         break;
    case QMessageBox::Cancel:
        //return
        box->close();
        break;
    case QMessageBox::Close:
        box->close();
        close();
    }
}

void fattura_rg::eli_riga()
{
    if(f_libri_2->isChecked())
    {
        QSqlQuery query,query1,query2;
        query.prepare("DELETE FROM fatture_righe_vendita_art WHERE id = :id");
        query1.prepare("DELETE FROM fatture_vendita_righe WHERE id = :id");
        query2.prepare("DELETE FROM fatture_vendita where id=:id");
        query.bindValue(":id",id1->text());
        query1.bindValue(":id",id1->text());
        query2.bindValue(":id",id1->text());

        if (query.exec() && query1.exec() && query2.exec()) //Se esito OK Eliminato da DB
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
        emit salvafattura();
        lista_libri();
        tot_imp_iva();
    }
    else if(f_prod_dig_2->isChecked())
    {
        QSqlQuery query,query1,query2;
        query.prepare("DELETE FROM fatture_righe_vendita_prod_dig WHERE id = :id");
        query1.prepare("DELETE FROM fatture_vendita_righe_dig WHERE id = :id");
        query2.prepare("DELETE FROM fatture_vendita_pr_dg where id=:id");
        query.bindValue(":id",id1->text());
        query1.bindValue(":id",id1->text());
        query2.bindValue(":id",id1->text());

        if (query.exec() && query1.exec() && query2.exec()) //Se esito OK Eliminato da DB
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
        emit salvafattura();
        lista_prod_dig();
        tot_imp_iva();
    }
}

void fattura_rg::tot_imp_iva_pr()
{
    if(f_libri_2->isChecked()){
    QSqlQuery Query1,Query2,Query3;
    QSqlQuery query;
    query.prepare("select id from fatture_vendita_righe where id= '"+id1->text()+"'");
    query.exec();
    if(query.next()){

        Query1.prepare("select sum(prezzo_s_iva) from fatture_vendita_righe where id='"+id1->text()+"'");
    Query1.exec();
    if(Query1.next()){
        imponibile_2->setText(QString::number(Query1.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il prezzo senza iva... "+Query1.lastError().text());
    }

    Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_vendita_righe where id='"+id1->text()+"'");
    Query2.exec();
    if(Query2.next()){
        iva_ft_2->setText(QString::number(Query2.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare l'iva... "+Query2.lastError().text());
    }


    Query3.prepare("select sum(totale) from fatture_vendita_righe where id ='"+id1->text()+"'");
    Query3.exec();
    if(Query3.next()){
        totale_2->setText(QString::number(Query3.value(0).toDouble()));
    }
    else{
        QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il totale... "+Query3.lastError().text());
    }
    }
    }
    else if(f_prod_dig_2->isChecked()){
        QSqlQuery Query1,Query2,Query3;
        QSqlQuery query;
        query.prepare("select id from fatture_vendita_righe_dig where id= '"+id1->text()+"'");
        query.exec();
        if(query.next()){

            Query1.prepare("select sum(prezzo_s_iva) from fatture_vendita_righe_dig where id='"+id1->text()+"'");
        Query1.exec();
        if(Query1.next()){
            imponibile_2->setText(QString::number(Query1.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il prezzo senza iva... "+Query1.lastError().text());
        }

        Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_vendita_righe_dig where id='"+id1->text()+"'");
        Query2.exec();
        if(Query2.next()){
            iva_ft_2->setText(QString::number(Query2.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare l'iva... "+Query2.lastError().text());
        }


        Query3.prepare("select sum(totale) from fatture_vendita_righe_dig where id ='"+id1->text()+"'");
        Query3.exec();
        if(Query3.next()){
            totale_2->setText(QString::number(Query3.value(0).toDouble()));
        }
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile instanziare il totale... "+Query3.lastError().text());
        }
        }
    }
    emit salvafattura();
}

void fattura_rg::visual_scadenze()
{
    //Disalibilito visualizzazione scadenze
    title_scadenze->setVisible(false);
    scadenza_1->setVisible(false);
    scadenza_2->setVisible(false);
    scadenza_3->setVisible(false);
}

void fattura_rg::tipo_pagamento()
{
    for(int i=0;i < 3;++i){
        visual_scadenze();
    }

    if(pagam->currentIndex()==4 || pagam->currentIndex()==5)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(120);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
    else if(pagam->currentIndex()==6 || pagam->currentIndex()==7)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(150);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);

    }
    else if(pagam->currentIndex()==8 || pagam->currentIndex()==9)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(180);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
    else if(pagam->currentIndex()==10 || pagam->currentIndex()==11)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(30);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
    else if(pagam->currentIndex()==12 || pagam->currentIndex()==13)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        scadenza_2->setVisible(true);
        QDate data,data1;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(30);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
        data1.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data1 = data1.addDays(60);
        scadenza_2->setCalendarPopup(true);
        scadenza_2->setDate(data1);
    }
    else if(pagam->currentIndex()==14 || pagam->currentIndex()==15)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        scadenza_2->setVisible(true);
        scadenza_3->setVisible(true);
        QDate data,data1,data2;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(30);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
        data1.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data1 = data1.addDays(60);
        scadenza_2->setCalendarPopup(true);
        scadenza_2->setDate(data1);
        data2.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data2 = data2.addDays(90);
        scadenza_3->setCalendarPopup(true);
        scadenza_3->setDate(data2);
    }
    else if(pagam->currentIndex()==16 || pagam->currentIndex()==17)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(60);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
    else if(pagam->currentIndex()==18 || pagam->currentIndex()==19)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        scadenza_2->setVisible(true);
        QDate data,data1;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(60);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
        data1.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data1 = data1.addDays(90);
        scadenza_2->setCalendarPopup(true);
        scadenza_2->setDate(data1);
    }
    else if(pagam->currentIndex()==20 || pagam->currentIndex()==21)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(90);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
    else if(pagam->currentIndex()==22 || pagam->currentIndex()==23)
    {
        title_scadenze->setVisible(true);
        scadenza_1->setVisible(true);
        QDate data;
        data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
        data = data.addDays(210);
        scadenza_1->setCalendarPopup(true);
        scadenza_1->setDate(data);
    }
}

fattura_rg::~fattura_rg()
{

}
