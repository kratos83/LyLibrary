#include "fatt_new.h"
#include "ui_fatt_new.h"
#include "fatt_acq_art.h"
#include "fatt_acq.h"
#include "fornitori.h"
#include "itdelegato.h"
#include "impo_iva_toto.h"
#include <QCompleter>
#include <QtGui>
#include <QtSql>
#include <QMessageBox>

fatt_new::fatt_new(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(n_riga,SIGNAL(clicked()),this,SLOT(fattura_riga_open()));
    connect(annulla,SIGNAL(clicked()),this,SLOT(chiudi()));
    connect(md_riga,SIGNAL(clicked()),this,SLOT(modifica_riga()));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(tab_view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(modifica_riga()));
    connect(fornitore_fatt,SIGNAL(clicked()),this,SLOT(open_fornitore()));
    connect(sp_tr,SIGNAL(textEdited(QString)),this,SLOT(spesa_trasp(const QString &)));
    connect(sp_in,SIGNAL(textEdited(QString)),this,SLOT(spesa_in(const QString &)));
    connect(sconto_1,SIGNAL(textEdited(QString)),this,SLOT(sconto_1t(const QString &)));
    connect(sconto_2,SIGNAL(textEdited(QString)),this,SLOT(sconto_2t(const QString &)));
    connect(sconto_3,SIGNAL(textEdited(QString)),this,SLOT(sconto_3t(const QString &)));
    connect(agg_totali,SIGNAL(clicked()),this,SLOT(tot_imp_iva()));
    connect(el_riga,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(save,SIGNAL(clicked()),this,SLOT(fatt_inserisci()));
    connect(agg_fatt,SIGNAL(clicked()),this,SLOT(fatt_aggiorna()));
    //Setting data
    data_fat->setCalendarPopup(true);
    data_fat->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());

    mod_grid = new QSqlTableModel(this);
    local_settings = new QLocale(QLocale::system());

    //Avvio combox
    combo_fornitori();
    combo_pagamenti();
    lista();
}

fatt_new::~fatt_new()
{
}

void fatt_new::fattura_riga_open(){

    fatt_acq_art *ft = new fatt_acq_art(id->text(),this);
    ft->setModal(true);
    ft->setWindowTitle(tr("Inserisci articoli fattura"));
    connect(ft,SIGNAL(save_fatt()),this,SLOT(lista()));
    connect(ft,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
    ft->exec();
    lista();
}

double fatt_new::s_iva(double psiva){

    cl = psiva;
    return psiva;
}

double fatt_new::c_iva(double pciva){

    ts = pciva;
    return pciva;
}

double fatt_new::iva_tot(double ivatot){

    cn = ivatot;
    return ivatot;
}

void fatt_new::clear_testo()
{
    id->setText("");
    data_fat->clear();
    forn->clear();
    sconto_1->setText("");
    sconto_2->setText("");
    sconto_3->setText("");
    sp_in->setText("");
    sp_tr->setText("");
    t_pagam->clear();
    n_fatt->setText("");
    dateEdit->clear();
    textEdit->setText("");
    imponibile->setText("");
    iva_ft->setText("");
    totale->setText("");
}


void fatt_new::formatta_num(){

    QString sc1 = local_settings->toString(sconto_1->text().toDouble(),'f',2);
    sconto_1->setText(sc1);
    QString sc2 = local_settings->toString(sconto_2->text().toDouble(),'f',2);
    sconto_2->setText(sc2);
    QString sc3 = local_settings->toString(sconto_3->text().toDouble(),'f',2);
    sconto_3->setText(sc3);
    QString spese_tr = local_settings->toString(sp_tr->text().toDouble(),'f',2);
    sp_tr->setText(spese_tr);
    QString spese_in = local_settings->toString(sp_in->text().toDouble(),'f',2);
    sp_in->setText(spese_in);
    QString tot = local_settings->toString(totale->text().toDouble(),'f',2 );
    totale->setText(tot);
    QString iva = local_settings->toString(iva_ft->text().toDouble(),'f',2 );
    iva_ft->setText(iva);
    QString imp = local_settings->toString(imponibile->text().toDouble(),'f',2);
    imponibile->setText(imp);
}
void fatt_new::fatt_inserisci(){

    if((n_fatt->text() =="") & (forn->currentText()=="") || (n_fatt->text() =="") ){
    QMessageBox MsgBox;
    MsgBox.setWindowTitle(tr("LyLibrary"));
    MsgBox.setText(tr("Errore"));
    MsgBox.setInformativeText(QString::fromUtf8(tr("Impossibile inserire la fattura,\n poichè non hai inserito i dati correttamente...").toStdString().c_str()));
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.exec();
    n_fatt->setStyleSheet("background-color: rgb(249, 22, 5)");
    forn->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    QSqlQuery query;
    query.prepare("INSERT INTO fatture_acq_righe (id,data,fornitore,sconto_1,sconto_2,sconto_3,spese_trasporto,spese_incasso,tipo_pagamento,numero_fattura,data_fattura,note_fattura,prezzo_s_iva,iva,totale)"
          " VALUES(:id,:data,:fornitore,:sconto_1,:sconto_2,:sconto_3,:spese_trasporto,:spese_incasso,:tipo_pagamento,:numero_fattura,:data_fattura,:note_fattura,:prezzo_s_iva,:iva,:totale)");

    query.bindValue(":id",id->text());
    query.bindValue(":data",data_fat->date());
    query.bindValue(":fornitore",forn->currentText());
    query.bindValue(":sconto_1",sconto_1->text());
    query.bindValue(":sconto_2",sconto_2->text());
    query.bindValue(":sconto_3",sconto_3->text());
    query.bindValue(":spese_trasporto",sp_tr->text());
    query.bindValue(":spese_incasso",sp_in->text());
    query.bindValue(":tipo_pagamento",t_pagam->currentText());
    query.bindValue(":numero_fattura",n_fatt->text());
    query.bindValue(":data_fattura",dateEdit->date());
    query.bindValue(":note_fattura",textEdit->toPlainText());
    query.bindValue(":prezzo_s_iva",imponibile->text());
    query.bindValue(":iva",iva_ft->text());
    query.bindValue(":totale",totale->text());

    insert_f_libri();

    if(query.exec()){
      clear_testo();
    }
    else{
    QMessageBox MsgBox;
    MsgBox.setText(tr("La voce suddetta non si puo inserire"));
    MsgBox.setInformativeText(tr("Impossibile inserire ")+query.lastError().text());
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.exec();
    }
    emit salvafatturanew();
    tot_imp_iva();
    close();
    }
}

void fatt_new::insert_f_libri(){
    QSqlQuery query1;
    query1.prepare("INSERT INTO fatture_acq (id,data,fornitore,totale,tipo_fattura)"
           " VALUES(:id,:data,:fornitore,:totale,:tipo_fattura)");

    query1.bindValue(":id",id->text());
    query1.bindValue(":data",data_fat->date());
    query1.bindValue(":fornitore",forn->currentText());
    query1.bindValue(":totale",totale->text());
    query1.bindValue(":tipo_fattura",tr("Acquisti"));

    if(query1.exec()){
	QMessageBox::information(this,tr("LyLibrary"),tr("Inserimento effettuato correttamente..."));
    }
    else{
	QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile inserire ")+query1.lastError().text());
    }
}

void fatt_new::fatt_aggiorna(){

    if((n_fatt->text() =="") & (forn->currentText()=="") || (n_fatt->text() =="") ){
    QMessageBox MsgBox;
    MsgBox.setText("Errore");
    MsgBox.setInformativeText(QString::fromUtf8(tr("Impossibile aggiornare la fattura,\n poichè non hai inserito i dati correttamente...").toStdString().c_str()));
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.exec();
    n_fatt->setStyleSheet("background-color: rgb(249, 22, 5)");
    forn->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    QSqlQuery query;
    query.prepare("UPDATE fatture_acq_righe SET data=:data,fornitore=:fornitore,sconto_1=:sconto_1, "
           " sconto_2=:sconto_2,sconto_3=:sconto_3,spese_trasporto=:spese_trasporto,spese_incasso=:spese_incasso,tipo_pagamento=:tipo_pagamento, "
           "numero_fattura=:numero_fattura,data_fattura=:data_fattura,note_fattura=:note_fattura,prezzo_s_iva=:prezzo_s_iva,iva=:iva,totale=:totale "
           " WHERE id =:id");
    query.bindValue(":id",id->text());
    query.bindValue(":data",data_fat->date());
    query.bindValue(":fornitore",forn->currentText());
    query.bindValue(":sconto_1",sconto_1->text());
    query.bindValue(":sconto_2",sconto_2->text());
    query.bindValue(":sconto_3",sconto_3->text());
    query.bindValue(":spese_trasporto",sp_tr->text());
    query.bindValue(":spese_incasso",sp_in->text());
    query.bindValue(":tipo_pagamento",t_pagam->currentText());
    query.bindValue(":numero_fattura",n_fatt->text());
    query.bindValue(":data_fattura",dateEdit->date());
    query.bindValue(":note_fattura",textEdit->toPlainText());
    query.bindValue(":prezzo_s_iva",imponibile->text());
    query.bindValue(":iva",iva_ft->text());
    query.bindValue(":totale",totale->text());

    update_f_libri();

    if( query.exec()){
	clear_testo();
    }
    else{
    QMessageBox MsgBox;
    MsgBox.setText(tr("La voce suddetta non si puo aggiornare"));
    MsgBox.setInformativeText(tr("Impossibile aggiornare ")+query.lastError().text());
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.exec();
    }
    emit salvafatturanew();
    close();
    }
}

void fatt_new::update_f_libri(){

    QSqlQuery query1;
    query1.prepare("UPDATE fatture_acq SET data=:data,fornitore=:fornitore,tipo_fattura=:tipo_fattura,totale=:totale WHERE id=:id");
    query1.bindValue(":id",id->text());
    query1.bindValue(":data",data_fat->date());
    query1.bindValue(":fornitore",forn->currentText());
    QString txt = tr("Acquisti");
    query1.bindValue(":tipo_fattura",txt);
    query1.bindValue(":totale",totale->text());

    if(query1.exec()){
    QMessageBox::information(this,tr("LyLibrary"),tr("Aggiornamento effettuato correttamente..."));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile inserire ")+query1.lastError().text());
    }
}

void fatt_new::open_fornitore(){

    fornitori *fo = new fornitori(this);
    fo->setModal(true);

    connect(fo,SIGNAL(vis_forn()),this,SLOT(combo_fornitori()));
    fo->exec();
}

void fatt_new::elimina(){
    if(tab_view->selectionModel()->currentIndex().isValid()){
    if (!id->text().isEmpty())
    {
        QSqlQuery qctrl;
        qctrl.prepare("SELECT * FROM fatture_righe_acq_art WHERE id='"+id->text()+"'");
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
	QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare"));
    }
}

void fatt_new::elimina_riga(){

    QSqlQuery query,query1;
    query.prepare("DELETE FROM fatture_righe_acq_art WHERE id='"+id->text()+"'");
    query1.prepare("DELETE FROM fatture_acq_righe WHERE id='"+id->text()+"'");
    query.bindValue(":id",id->text());
    query1.bindValue(":id",id->text());

    if (query.exec() && query1.exec()) //Se esito OK Eliminato da DB
    {
    lista();
    }
    else
    {
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle(tr("Lylibrary"));
    box->setInformativeText(tr("Impossibile eliminare \n il record selezionato?...."));
    box->exec();
     }
    emit salvafatturanew();
    lista();
    tot_imp_iva();
}

void fatt_new::clickgrid(){

    QString id_new;
    int riga = tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
}

void fatt_new::salva_fattura_ric(){

    emit salvafatturanew();
    lista();
}

void fatt_new::modifica_riga(){

    if(tab_view->selectionModel()->currentIndex().isValid()){
    QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    modifica_riga_one(newindex);
    }
    else
    {
    QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da modificare..."));
    }
}

void fatt_new::modifica_riga_one(QModelIndex index){
    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){
    fatt_acq_art *gt = new fatt_acq_art(id->text(),this);
    gt->setModal(true);
    QSqlRecord record = mod_grid->record(index.row());
    if(!record.isEmpty()){
        gt->cod_art->setText(record.value("cod_art").toString());
        gt->art_nom->setText(record.value("nome_articolo").toString());
        gt->descrizione->setText(record.value("descrizione").toString());
        gt->pr_unit->setText(record.value("prezzo_unit").toString());
        gt->p_s_iva->setText(record.value("prezzo_s_iva").toString());
        gt->p_c_iva->setText(record.value("prezzo_c_iva").toString());
        gt->quant->setText(record.value("quantita").toString());
        gt->totale->setText(record.value("totale").toString());
        QSqlQuery query,query1,query2;
        query.prepare("select tipo_fattura from fatture_acq");
        query1.exec("select * from articoli");
        query.exec();
        if(query.next()){
        if(query.value(0).toString() == tr("Acquisti")){
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
            }
        }
        }
    }
    connect(gt,SIGNAL(save_fatt()),this,SLOT(lista()));
    connect(gt,SIGNAL(save_fatt()),this,SLOT(tot_imp_iva()));
    gt->exec();
    }
    lista();
}


void fatt_new::sconto_1t(const QString &){

    double sptr = local_settings->toDouble(sconto_1->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile->text().toDouble()-((imponibile->text().toDouble()*sptr)/100);
    imponibile->setText(QString::number(sp));
    sp1 = iva_ft->text().toDouble()-((iva_ft->text().toDouble()*sptr)/100);
    iva_ft->setText(QString::number(sp1));
    sp2 = totale->text().toDouble()-((totale->text().toDouble()*sptr)/100);
    totale->setText(QString::number(sp2));

    if(sconto_1->text().length() == 0){
    sconto_1->setText("0.00");
    tot_imp_iva();
    }
}

void fatt_new::sconto_2t(const QString &){

    double sptr = local_settings->toDouble(sconto_2->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile->text().toDouble()-((imponibile->text().toDouble()*sptr)/100);
    imponibile->setText(QString::number(sp));
    sp1 = iva_ft->text().toDouble()-((iva_ft->text().toDouble()*sptr)/100);
    iva_ft->setText(QString::number(sp1));
    sp2 = totale->text().toDouble()-((totale->text().toDouble()*sptr)/100);
    totale->setText(QString::number(sp2));

    if(sconto_2->text().length() == 0){
    sconto_2->setText("0.00");
    tot_imp_iva();
    }
}

void fatt_new::sconto_3t(const QString &){

    double sptr = local_settings->toDouble(sconto_3->text());
    double sp =0,sp1=0,sp2 =0;

    sp = imponibile->text().toDouble()-((imponibile->text().toDouble()*sptr)/100);
    imponibile->setText(QString::number(sp));
    sp1 = iva_ft->text().toDouble()-((iva_ft->text().toDouble()*sptr)/100);
    iva_ft->setText(QString::number(sp1));
    sp2 = totale->text().toDouble()-((totale->text().toDouble()*sptr)/100);
    totale->setText(QString::number(sp2));

    if(sconto_3->text().length() == 0){
    sconto_3->setText("0.00");
    tot_imp_iva();
    }
}

void fatt_new::combo_fornitori(){

    forn->clear();
    QStringList list;
    QSqlQuery query("select ragione_sociale from fornitori");
    if(query.exec()){
    while(query.next()){
    list << query.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(forn->model());
    forn->setCompleter(complete);
    forn->addItems(list);
}

void fatt_new::combo_pagamenti(){

    t_pagam->clear();
    QStringList list;
    QSqlQuery query("select codice from pagam");
    if(query.exec()){
    while(query.next()){
    list << query.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(t_pagam->model());
    t_pagam->setCompleter(complete);
    t_pagam->addItems(list);
}

void fatt_new::spesa_trasp(const QString &){

    double sptr = local_settings->toDouble(sp_tr->text());
    double sp = 0,sp1 = 0,sp2=0; //(sptr*20)/100;

    sp = imponibile->text().toDouble()+sptr;
    imponibile->setText(QString::number(sp));
    sp1 = iva_ft->text().toDouble()+((sptr*20)/100);
    iva_ft->setText(QString::number(sp1));
    sp2 = totale->text().toDouble()+sptr-((sptr*20)/100);
    totale->setText(QString::number(sp2));

    if(sp_tr->text().length()==0){
    tot_imp_iva();
    sp_tr->setText("0.00");
    }
}

void fatt_new::spesa_in(const QString &){

    double sptr = local_settings->toDouble(sp_in->text());
    double sp = 0,sp1 = 0,sp2=0; //(sptr*20)/100;

    sp = imponibile->text().toDouble()+sptr;
    imponibile->setText(QString::number(sp));
    sp1 = iva_ft->text().toDouble()+((sptr*20)/100);
    iva_ft->setText(QString::number(sp1));
    sp2 = totale->text().toDouble()+sptr-((sptr*20)/100);
    totale->setText(QString::number(sp2));

    if(sp_in->text().length()==0){
    tot_imp_iva();
    sp_in->setText("0.00");
    }
}

void fatt_new::lista(){

    mod_grid->setQuery("select cod_art,nome_articolo,descrizione,prezzo_unit,prezzo_s_iva,iva,prezzo_c_iva,quantita,totale from fatture_righe_acq_art where id='"+id->text()+"'");
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("Codice"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Prodotto"));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Descrizione"));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Prezzo unitario"));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Prezzo S. IVA"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("IVA"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Prezzo C. IVA"));
    mod_grid->setHeaderData(7, Qt::Horizontal, QString::fromUtf8(tr("Quantita'").toStdString().c_str()));
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

void fatt_new::tot_imp_iva(){

    QSqlQuery Query1,Query2,Query3;
    QSqlQuery query;
    query.prepare("select id from fatture_righe_acq_art where id= '"+id->text()+"'");
    query.exec();
    if(query.next()){

    Query1.prepare("select sum(prezzo_s_iva) from fatture_righe_acq_art where id='"+id->text()+"'");
    Query1.exec();
    if(Query1.next()){
    imponibile->setText(QString::number(Query1.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare il prezzo senza iva... ")+Query1.lastError().text());
    }

    Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_righe_acq_art where id='"+id->text()+"'");
    Query2.exec();
    if(Query2.next()){
    iva_ft->setText(QString::number(Query2.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare l'iva... ")+Query2.lastError().text());
    }


    Query3.prepare("select sum(totale) from fatture_righe_acq_art where id ='"+id->text()+"'");
    Query3.exec();
    if(Query3.next()){
    totale->setText(QString::number(Query3.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare il totale... ")+Query3.lastError().text());
    }
    }
    emit salvafatturanew();
}

void fatt_new::tot_imp_iva_rg()
{
    QSqlQuery Query1,Query2,Query3;
    QSqlQuery query;
    query.prepare("select id from fatture_acq_righe where id= '"+id->text()+"'");
    query.exec();
    if(query.next()){

    Query1.prepare("select sum(prezzo_s_iva) from fatture_acq_righe where id='"+id->text()+"'");
    Query1.exec();
    if(Query1.next()){
    imponibile->setText(QString::number(Query1.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare il prezzo senza iva... ")+Query1.lastError().text());
    }

    Query2.prepare("select sum(totale-prezzo_s_iva) from fatture_acq_righe where id='"+id->text()+"'");
    Query2.exec();
    if(Query2.next()){
    iva_ft->setText(QString::number(Query2.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare l'iva... ")+Query2.lastError().text());
    }


    Query3.prepare("select sum(totale) from fatture_acq_righe where id ='"+id->text()+"'");
    Query3.exec();
    if(Query3.next()){
    totale->setText(QString::number(Query3.value(0).toDouble()));
    }
    else{
    QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile instanziare il totale... ")+Query3.lastError().text());
    }
    }
    emit salvafatturanew();
}

void fatt_new::chiudi()
{
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle(tr("Lylibrary"));
    box->setInformativeText(tr("Sei sicuro di voler uscire dalla fattura?\n I dati non verranno salvati...."));
    box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Close);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
     //Close
     save->setChecked(true);
     save->setStyleSheet("background-color: rgb(249, 22, 5)");
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

void fatt_new::eli_riga()
{
    QSqlQuery query,query1,query2;
    query.prepare("DELETE FROM fatture_righe_acq_art WHERE id='"+id->text()+"'");
    query1.prepare("DELETE FROM fatture_acq_righe WHERE id='"+id->text()+"'");
    query2.prepare("SELECT * from fatture_acq WHERE id='"+id->text()+"'");

    if (query.exec() && query1.exec() && query2.exec()) //Se esito OK Eliminato da DB
    {
    lista();
    }
    else
    {
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle(tr("Lylibrary"));
    box->setInformativeText(tr("Impossibile eliminare \n il record selezionato?...."));
    box->exec();
     }
    emit salvafatturanew();
    lista();
    tot_imp_iva();
}
