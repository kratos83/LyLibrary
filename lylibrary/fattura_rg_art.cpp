/******************************************************
 * Intestazioni Qt
 * ***************************************************/
#include <QMessageBox>
#include <QCompleter>
#include <QFileDialog>

#include "fattura_rg_art.h"
#include "ui_fattura_rg_art.h"

/******************************************************
 * Classe fattura_rg_art.
 * Questa classe serve a gestire i dati per l'inserimento
 * nelle righe della fattura.
 * @autor Angelo Scarna
 ******************************************************/

fattura_rg_art *rg_art = 0;

fattura_rg_art::fattura_rg_art(QString id,QWidget *parent) :
    QDialog(parent)
{
    if(rg_art)
        delete rg_art;
    rg_art = this;

    setupUi(this);

    //Inizializzazione fattura
    fatt = new fattura_rg;
    id_fatt_ven = id;
    //Imposto local_setting di sistema
    local_settings = new QLocale(QLocale::system());
    /*********************************************
     *Imposto i dati in virgola mobile
     *********************************************/
    p_s_iva->setValidator(new QDoubleValidator(-9999999999.99,
                                                   9999999999.99,2,p_s_iva));
    p_c_iva->setValidator(new QDoubleValidator(-9999999999.99,
                                                   9999999999.99,2,p_c_iva));
    totale->setValidator(new QDoubleValidator(-9999999999.99,
                                                  9999999999.99,2,totale));

    sconto->setValidator(new QDoubleValidator(-9999999999.99,
                                                  9999999999.99,2,sconto));

    /**************************************
     * Disabilita pulsanti
     **************************************/
    found = false;
    image_dir->setVisible(false);

    /************************************
     * Avvio i metodi per la
     * visualizzazione dei dati
     ************************************/
    formattanum();
    comboiva();
    comboum();
    combo_categ();

    /************************************************
     * Connessione dei pulsanti
     ************************************************/
    connect(exit_art,SIGNAL(clicked()),this,SLOT(close()));
    connect(agg_art,SIGNAL(clicked()),this,SLOT(inserisci_riga_art()));
    connect(fatt->tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(pr_unit,SIGNAL(textEdited(const QString &)),this,SLOT(text(const QString &)));
    connect(quant,SIGNAL(textEdited(const QString &)),this,SLOT(prezzosiva(const QString &)));
    connect(quant,SIGNAL(textEdited(QString)),this,SLOT(complete_tot(const QString &)));
    connect(sconto,SIGNAL(textEdited(const QString)),this,SLOT(sconto_tot(const QString &)));
    connect(comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(text(QString)));
    connect(p_c_iva,SIGNAL(textChanged(QString)),this,SLOT(complete_tot(QString)));
    connect(aggiorna,SIGNAL(clicked()),this,SLOT(aggiorna_riga()));
    connect(ins_image,SIGNAL(clicked()),this,SLOT(image_but()));
    connect(cod_art,SIGNAL(textChanged(const QString &)),this,SLOT(cerca_codart(const QString &)));
}

/*****************************************************
 * La funzione clear_Testo() serve a cancellare i dati
 * presenti nella finestra di dialogo a causa del non
 * ritrovamento del prodotto
 *****************************************************/
void fattura_rg_art::clear_Testo()
{
    QImage img("");
    dir_image->setPixmap(QPixmap::fromImage(img));
    cod_barre->setText("");
    art_nom->setText("");
    descrizione->setText("");
    autore->setText("");
    lingua->setText("");
    textEdit->setText("");
    scaffale->setText("");
    quant->setText("");
    image_dir->setText("");
}

/***********************************************************
 * Imposta l'immagine da stampare
 **********************************************************/
void fattura_rg_art::image_but()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Lylibrary"),
                                     tr("Impossibile aprire %1.").arg(fileName));
            return;
        }
        dir_image->setPixmap(QPixmap::fromImage(image));
        image_dir->setText(fileName);
    }
}

/******************************************
 * Imposta tutte le qlineedit in text() == 0
 *******************************************/

void fattura_rg_art::nuovo()
{
    cod_art->setText("");
    art_nom->setText("");
    descrizione->setText("");
    sconto->setText("");
    comboBox->clear();
    p_s_iva->setText("");
    p_c_iva->setText("");
    quant->setText("");
    totale->setText("");
    formattanum();
    comboiva();
    comboum();
    combo_categ();
}

/***************************************************
 * Questo metodo formatta i numeri in virgola mobile
 * per essere più comprensibili
 * Es: 53.2
 **************************************************/
void fattura_rg_art::formattanum()
{
    QString text = local_settings->toString(p_c_iva->text().toDouble(),'f',2);
    p_c_iva->setText(text);
    QString ps = local_settings->toString(p_s_iva->text().toDouble(),'f',2);
    p_s_iva->setText(ps);
    QString tot = local_settings->toString(totale->text().toDouble(),'f',2);
    totale->setText(tot);
}

/****************************************************
 * Questo metodo imposta la combobox dell'iva
 ****************************************************/
void fattura_rg_art::comboiva()
{
    QStringList list;
    QSqlQuery query("select iva from aliquota");
    if(query.exec()){
    while(query.next()){
        list << query.value(0).toString();
    }
    complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(comboBox->model());
    comboBox->setCompleter(complete);
    comboBox->addItems(list);
    }
}

/****************************************************
 * Questo metodo imposta la combobox dell'unità
 * di misura.
 ****************************************************/
void fattura_rg_art::comboum()
{
    QStringList lista;
    QSqlQuery query("select unita from misura");
    if(query.exec()){
        while(query.next()){
           lista << query.value(0).toString();
        }
    }
    complete = new QCompleter(lista,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    complete->setModel(um->model());
    um->setCompleter(complete);
    um->addItems(lista);
}

void fattura_rg_art::combo_categ()
{
    comboBox_2->clear();
    QStringList list;
    QSqlQuery query("select tipo_categ from categoria");
    if(query.exec()){
    while(query.next()){
        list << query.value(0).toString();
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    comboBox_2->setCompleter(complete);
    comboBox_2->addItems(list);
    }

    if(comboBox_2->currentText().length() == 0){
       fattura_rg_art::comboiva();
    }
}

void fattura_rg_art::cerca_codart(const QString &)
{
    QSqlQuery query("SELECT * from carico_libri where cod_articolo='"+cod_art->text()+"'");
    QStringList list;
    query.exec();
    if(query.next()){
        found = true;
        testo_show->setText(tr("Articolo trovato..."));
        list << query.value(0).toString();
        QImage img(query.value(14).toString());
        dir_image->setPixmap(QPixmap::fromImage(img));
        cod_barre->setText(query.value(1).toString());
        art_nom->setText(query.value(2).toString());
        descrizione->setText(query.value(3).toString());
        autore->setText(query.value(4).toString());
        lingua->setText(query.value(5).toString());
        textEdit->setText(query.value(14).toString());
        scaffale->setText(query.value(7).toString());
        quant->setText(query.value(8).toString());
        pr_unit->setText(query.value(9).toString());
        p_s_iva->setText(query.value(10).toString());
        p_c_iva->setText(query.value(11).toString());
        totale->setText(query.value(12).toString());
        image_dir->setText(query.value(14).toString());
        if(query.value(0).toString() != cod_art->text()){
                found = false;
                testo_show->setText(tr("Articolo non trovato..."));
                clear_Testo();
         }
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    cod_art->setCompleter(complete);
    if(cod_art->text() == ""){
        clear_Testo();
    }
    
}

void fattura_rg_art::text(const QString &){
    double iva_art = comboBox->currentText().toDouble();
    double p_iva = pr_unit->text().toDouble();
    double perc = (((p_iva*iva_art)/100)+p_iva);
    p_c_iva->setText(QString::number(perc,'f',2));
}

void fattura_rg_art::sconto_tot(const QString &)
{
    double iva_art = comboBox->currentText().toDouble();
    double ps_iva = pr_unit->text().toDouble();
    double quan = quant->text().toDouble();
    double tot = totale->text().toDouble();
    double perc = ((ps_iva*iva_art)/100)+ps_iva;
    tot = (perc*quan);
    double sc_tot =sconto->text().toDouble();
    double applica = ((tot*sc_tot)/100);
    double set = tot -applica;
    totale->setText(QString::number(set,'f',2));
}

void fattura_rg_art::prezzosiva(const QString &){
    double pr_un = pr_unit->text().toDouble();
    double quan = quant->text().toDouble();
    double p_s_q = pr_un*quan;
    p_s_iva->setText(QString::number(p_s_q,'f',2));
}

void fattura_rg_art::complete_tot(const QString &){
    double iva_art = comboBox->currentText().toDouble();
    double ps_iva = pr_unit->text().toDouble();
    double quan = quant->text().toDouble();
    double tot = totale->text().toDouble();
    double perc = ((ps_iva*iva_art)/100)+ps_iva;
    tot = (perc*quan);
    totale->setText(QString::number(tot,'f',2));
}

void fattura_rg_art::p_iv(double dg){
    p_s_iva->setText(QString::number(dg));
}

void fattura_rg_art::cerca()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM articoli where cod_art='"+cod_art->text()+"'");
        query.exec();

        if (query.next())
        {   art_nom->setText(query.value(1).toString());
            descrizione->setText(query.value(2).toString());
            um->setCurrentText(query.value(3).toString());
            p_s_iva->setText(query.value(4).toString());
            p_c_iva->setText(query.value(5).toString());
            comboBox->setCurrentText(query.value(6).toString());
            quant->setText(query.value(7).toString());
            sconto->setText(query.value(8).toString());
            totale->setText(query.value(9).toString());
            image_dir->setText(query.value(10).toString());
            QImage img(query.value(10).toString());
            dir_image->setPixmap(QPixmap::fromImage(img));
        }
        else
        {
            clear_Testo();
        }
}

void fattura_rg_art::clickgrid()
{
    QString id_new;
    int riga = fatt->tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = fatt->mod_grid->index(riga,0,QModelIndex());

    id_new = fatt->mod_grid->data(elemento).toString();
    cod_art->setText(id_new);
    cerca();
}

void fattura_rg_art::prezzo_senza_iva()
{
        QSqlQuery Query1;
        Query1.prepare("select sum(prezzo_s_iva) from fatture_righe_vendita_art where id ='"+id_fatt_ven+"'");
        Query1.exec();
        if(Query1.next()){
            fatt->imponibile_2->setText(Query1.value(0).toString());
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile visualizzare il prezzo senza iva... ")+Query1.lastError().text());
        }
}

void fattura_rg_art::iva_sum()
{
        QSqlQuery Query2;
        Query2.prepare("select sum(prezzo_c_iva-prezzo_s_iva) from fatture_righe_vendita_art where id='"+id_fatt_ven+"'");
        Query2.exec();
        if(Query2.next()){
            fatt->iva_ft_2->setText(Query2.value(0).toString());
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile visualizzare il prezzo senza iva... ")+Query2.lastError().text());
        }
}

void fattura_rg_art::prezzo_con_iva()
{
        QSqlQuery Query3;
        Query3.prepare("select sum(totale) from fatture_righe_vendita_art where id ='"+id_fatt_ven+"'");
        Query3.exec();
        if(Query3.next()){
        fatt->totale_2->setText(Query3.value(0).toString());
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile visualizzare il prezzo senza iva... ")+Query3.lastError().text());
        }
}

void fattura_rg_art::inserisci_riga_art()
{
    QSqlQuery Qctrl;
    Qctrl.prepare("SELECT * FROM fatture_righe_vendita_art WHERE cod_art='"+cod_art->text()+"'");
    Qctrl.exec();

    if (Qctrl.next()) //Se esiste già
    {
        inserisci();
        close();
    }
    else
        {
         inserisci();
         close();
        }
}

void fattura_rg_art::inserisci()
{
                     QSqlQuery Query;
                     Query.prepare("INSERT INTO fatture_righe_vendita_art (id,cod_art,codbarre,nome_articolo,descrizione,unita,prezzo_unit,prezzo_s_iva,prezzo_c_iva,iva,quantita,sconto,totale)"
                                   " VALUES (:id, :cod_art, :codbarre, :nome_articolo, :descrizione,:unita,:prezzo_unit,:prezzo_s_iva,:prezzo_c_iva,:iva,:quantita,:sconto,:totale)"
                                 );
                     Query.bindValue(":id",id_fatt_ven);
                     Query.bindValue(":cod_art",cod_art->text());
                     Query.bindValue(":cod_barre",cod_barre->text());
                     Query.bindValue(":nome_articolo", art_nom->text());
                     Query.bindValue(":descrizione",descrizione->toPlainText());
                     Query.bindValue(":unita",um->currentText());
                     double iva_art = comboBox->currentText().toDouble();
                     Query.bindValue(":iva",iva_art);
                     double scon = sconto->text().toDouble();
                     Query.bindValue(":sconto",scon);
                     double quan = quant->text().toDouble();
                     double pr_un = pr_unit->text().toDouble();
                     Query.bindValue(":prezzo_unit",pr_un);
                     double ps_iva = p_s_iva->text().toDouble();
                     Query.bindValue(":prezzo_s_iva",ps_iva);
                     double perc = ((pr_un*iva_art)/100)+pr_un;
                     Query.bindValue(":prezzo_c_iva",p_c_iva->text());
                     Query.bindValue(":quantita",quan);
                     double tot = totale->text().toDouble();
                     tot = (perc*quan);
                     double applica = ((tot*scon)/100);
                     double set = tot -applica;
                     formattanum();
                     Query.bindValue(":totale",QString::number(set));
                     QSqlQuery query("select quantita,pr_unit,pr_s_iva,pr_c_iva,totale from carico_libri where cod_articolo='"+cod_art->text()+"'");
                     QSqlQuery query1("select quantita,pr_unit,pr_s_iva,pr_c_iva,totale from scarico_libri where cod_articolo='"+cod_art->text()+"'");
                     query.exec();
                     query1.exec();
                     if(query.next()){
                     found = true;
                         double qa = query.value(0).toDouble();
                         double pr = query.value(1).toDouble();
                         double pr_s = query.value(2).toDouble();
                         double pr_c = query.value(3).toDouble();
                         double tot_ = query.value(4).toDouble();
                             update_articoli(qa);
                             update_carico_libri(qa,pr,pr_s,pr_c,tot_);
                             if(query1.next()){
                                 double qa = query1.value(0).toDouble();
                                 double pr = query1.value(1).toDouble();
                                 double pr_s = query1.value(2).toDouble();
                                 double pr_c = query1.value(3).toDouble();
                                 double tot_ = query1.value(4).toDouble();
                                 update_scarico_libri(qa,pr,pr_s,pr_c,tot_);
                             }
                             else{
                                 insert_scarico_libri();
                             }
                     }

                         if (Query.exec()) //Se esito OK inserimento DB
                         {
			    clear_Testo();
                          }
                         else
                         {
                                 QMessageBox MsgBox;
                                 MsgBox.setText(tr("La voce suddetta non si puo inserire"));
                                 MsgBox.setInformativeText(tr("Impossibile inserire ")+Query.lastError().text());
                                 MsgBox.setIcon(QMessageBox::Warning);
                                 MsgBox.exec();
                          }
                         emit save_fatt();
                         fatt->lista_libri();
                         fatt->tot_imp_iva();
                         prezzo_senza_iva();
                         iva_sum();
                         prezzo_con_iva();
                         close();
}

void fattura_rg_art::insert_scarico_libri()
{
    QSqlQuery Query1;
    Query1.prepare("INSERT INTO scarico_libri (cod_articolo,codbarre,nome_articolo,descrizione,autore,lingua,infoeditore,categ,scaffale,quantita,pr_unit,pr_s_iva,pr_c_iva,totale,image)"
                   " VALUES (:cod_articolo,:cod_barre,:nome_articolo,:descrizione,:autore,:lingua,:infoeditore,:categ,:scaffale,:quantita,:pr_unit,:pr_s_iva,:pr_c_iva,:totale,:image)");
    Query1.bindValue(":cod_articolo",QString::fromUtf8(cod_art->text().toStdString().c_str()));
    Query1.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
    Query1.bindValue(":nome_articolo",QString::fromUtf8(art_nom->text().toStdString().c_str()));
    Query1.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
    Query1.bindValue(":categ",QString::fromUtf8(comboBox_2->currentText().toStdString().c_str()));
    Query1.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
    Query1.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
    Query1.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
    Query1.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
    double quan = quant->text().toDouble();
    Query1.bindValue(":quantita",quan);
    double p_unit = pr_unit->text().toDouble();
    Query1.bindValue(":pr_unit",p_unit);
    double ps_iva = p_s_iva->text().toDouble();
    Query1.bindValue(":pr_s_iva",ps_iva);
    double pc_iva = p_c_iva->text().toDouble();
    Query1.bindValue(":pr_c_iva",pc_iva);
    double tot = totale->text().toDouble();
    Query1.bindValue(":totale",tot);
    Query1.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

    if(Query1.exec()){
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Errore nell'inserimento... ")+Query1.lastError().text());
    }
}

void fattura_rg_art::aggiorna_riga()
{
        QSqlQuery Query;
        Query.prepare("UPDATE fatture_righe_vendita_art SET  id = :id, nome_articolo=:nome_articolo, "
                      "prezzo_s_iva=:prezzo_s_iva, descrizione=:descrizione, unita=:unita, prezzo_unit=:prezzo_unit, "
                      "prezzo_c_iva=:prezzo_c_iva, iva=:iva, quantita=:quantita, sconto=:sconto, totale=:totale "
                      "WHERE cod_art = :cod_art");


        Query.bindValue(":id",id_fatt_ven);
        Query.bindValue(":cod_art",cod_art->text());
        Query.bindValue(":nome_articolo", art_nom->text());
        Query.bindValue(":descrizione",descrizione->toPlainText());
        Query.bindValue(":unita",um->currentText());
        double iva_art = comboBox->currentText().toDouble();
        Query.bindValue(":iva",iva_art);
        double scon = sconto->text().toDouble();
        Query.bindValue(":sconto",scon);
        double quan = quant->text().toDouble();
        double pr_un = pr_unit->text().toDouble();
        Query.bindValue(":prezzo_unit",pr_un);
        double ps_iva = p_s_iva->text().toDouble();
        Query.bindValue(":prezzo_s_iva",ps_iva);
        double perc = ((pr_un*iva_art)/100)+pr_un;
        Query.bindValue(":prezzo_c_iva",p_c_iva->text());
        Query.bindValue(":quantita",quan);
        double tot = totale->text().toDouble();
        tot = (perc*quan);
        double applica = ((tot*scon)/100);
        double set = tot -applica;
        formattanum();
        Query.bindValue(":totale",QString::number(set));

    QSqlQuery query("select quantita,pr_unit,pr_s_iva,pr_c_iva,totale from carico_libri where cod_articolo='"+cod_art->text()+"'");
    query.exec();
    if(query.next()){
        found = true;
        double qa = query.value(0).toDouble();
        double pr = query.value(1).toDouble();
        double pr_s = query.value(2).toDouble();
        double pr_c = query.value(3).toDouble();
        double tot_ = query.value(4).toDouble();
            update_articoli(qa);
            update_scarico_libri(qa,pr,pr_s,pr_c,tot_);
            update_carico_libri(qa,pr,pr_s,pr_c,tot_);
    }
    if (Query.exec())
    {
        clear_Testo();
    }
    else
    {
            QMessageBox MsgBox;
            MsgBox.setText(tr("Voce non aggiornabile"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare ")+Query.lastError().text());
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
    }
    emit save_fatt();
    fatt->lista_libri();
    fatt->tot_imp_iva();
    prezzo_senza_iva();
    iva_sum();
    prezzo_con_iva();
    close();
}

void fattura_rg_art::update_articoli(double qua){
    QSqlQuery Query1;
    Query1.prepare("UPDATE articoli SET  codbarre=:codbarre, nome_articolo=:nome_articolo, "
                  " descrizione=:descrizione, autore=:autore, lingua=:lingua, infoeditore=:infoeditore, "
                  "categ=:categ, scaffale=:scaffale, quantita=:quantita, image=:image "
                  "WHERE cod_articolo = :cod_articolo");


    Query1.bindValue(":cod_articolo",QString::fromUtf8(cod_art->text().toStdString().c_str()));
    Query1.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
    Query1.bindValue(":nome_articolo",QString::fromUtf8(art_nom->text().toStdString().c_str()));
    Query1.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
    Query1.bindValue(":categ",QString::fromUtf8(comboBox_2->currentText().toStdString().c_str()));
    Query1.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
    Query1.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
    Query1.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
    Query1.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
    double quan = quant->text().toDouble();
    double somma = qua-quan;
    Query1.bindValue(":quantita",somma);
    Query1.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

    if(Query1.exec()){

    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile aggiornare... ")+Query1.lastError().text());
    }
}

void fattura_rg_art::update_carico_libri(double ca, double _pr_u, double _p_siva, double _p_civa, double _tot)
{
    QSqlQuery Query2;
    Query2.prepare("UPDATE carico_libri SET  codbarre=:codbarre, nome_articolo=:nome_articolo, "
                  " descrizione=:descrizione, autore=:autore, lingua=:lingua, infoeditore=:infoeditore, "
                  "categ=:categ, scaffale=:scaffale, quantita=:quantita, pr_unit=:pr_unit, pr_s_iva=:pr_s_iva, "
                   "pr_c_iva=:pr_c_iva, totale=:totale, image=:image "
                  "WHERE cod_articolo = :cod_articolo");

    Query2.bindValue(":cod_articolo",QString::fromUtf8(cod_art->text().toStdString().c_str()));
    Query2.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
    Query2.bindValue(":nome_articolo",QString::fromUtf8(art_nom->text().toStdString().c_str()));
    Query2.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
    Query2.bindValue(":categ",QString::fromUtf8(comboBox_2->currentText().toStdString().c_str()));
    Query2.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
    Query2.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
    Query2.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
    Query2.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
    double quan = quant->text().toDouble();
    double somma = ca-quan;
    Query2.bindValue(":quantita",somma);
    double p_unit = pr_unit->text().toDouble();
    //double sum_pr_unit = _pr_u-p_unit;
    Query2.bindValue(":pr_unit",p_unit);
    double ps_iva = p_s_iva->text().toDouble();
    double sum_pr_siva = _p_siva-ps_iva;
    Query2.bindValue(":pr_s_iva",sum_pr_siva);
    double pc_iva = p_c_iva->text().toDouble();
    //double sum_pr_civa = _p_civa-pc_iva;
    Query2.bindValue(":pr_c_iva",pc_iva);
    double tot = totale->text().toDouble();
    double sum_tot = _tot-tot;
    Query2.bindValue(":totale",sum_tot);
    Query2.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

    if(Query2.exec()){
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile aggiornare... ")+Query2.lastError().text());
    }
}

void fattura_rg_art::update_scarico_libri(double ca, double _pr_u, double _p_siva, double _p_civa, double _tot)
{
    QSqlQuery Query2;
    Query2.prepare("UPDATE scarico_libri SET  codbarre=:codbarre, nome_articolo=:nome_articolo, "
                  " descrizione=:descrizione, autore=:autore, lingua=:lingua, infoeditore=:infoeditore, "
                  "categ=:categ, scaffale=:scaffale, quantita=:quantita, pr_unit=:pr_unit, pr_s_iva=:pr_s_iva, "
                   "pr_c_iva=:pr_c_iva, totale=:totale, image=:image "
                  "WHERE cod_articolo = :cod_articolo");

    Query2.bindValue(":cod_articolo",QString::fromUtf8(cod_art->text().toStdString().c_str()));
    Query2.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
    Query2.bindValue(":nome_articolo",QString::fromUtf8(art_nom->text().toStdString().c_str()));
    Query2.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
    Query2.bindValue(":categ",QString::fromUtf8(comboBox_2->currentText().toStdString().c_str()));
    Query2.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
    Query2.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
    Query2.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
    Query2.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
    double quan = quant->text().toDouble();
    double somma = ca+quan;
    Query2.bindValue(":quantita",somma);
    double p_unit = pr_unit->text().toDouble();
    //double sum_pr_unit = _pr_u-p_unit;
    Query2.bindValue(":pr_unit",p_unit);
    double ps_iva = p_s_iva->text().toDouble();
    double sum_pr_siva = _p_siva+ps_iva;
    Query2.bindValue(":pr_s_iva",sum_pr_siva);
    double pc_iva = p_c_iva->text().toDouble();
    //double sum_pr_civa = _p_civa-pc_iva;
    Query2.bindValue(":pr_c_iva",pc_iva);
    double tot = totale->text().toDouble();
    double sum_tot = _tot+tot;
    Query2.bindValue(":totale",sum_tot);
    Query2.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

    if(Query2.exec()){
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile aggiornare... ")+Query2.lastError().text());
    }
}
fattura_rg_art::~fattura_rg_art()
{

}
