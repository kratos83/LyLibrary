#include "prodotti_dvd.h"
#include "ui_prodotti_dvd.h"
#include "itdelegato.h"
#include <math.h>
#include "print.h"
#include <QMessageBox>
#include <QCompleter>
#include <QStringList>
#include <QVariant>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintPreviewDialog>
#include <QtSql/QtSql>
#include "settingsmanager.h"

prodotti_dvd *digital = 0;

prodotti_dvd::prodotti_dvd(QWidget *parent) :QDialog(parent)
{
    digital = this;
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(exit_art,SIGNAL(clicked()),this,SLOT(close_art()));
    exit_art->setShortcut(tr("Ctrl+E"));
    connect(del_art,SIGNAL(clicked()),this,SLOT(delete_art()));
    //Chiamata della tastiera per eliminare dati dal db
    del_art->setShortcut(tr("Ctrl+D"));
    connect(agg_art,SIGNAL(clicked()),this,SLOT(aggiungi()));
    //Chiamata della tastiera per salvare dati sul db
    agg_art->setShortcut(tr("Ctrl+S"));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(tab_view,SIGNAL(pressed(QModelIndex)),this,SLOT(clickgrid()));
    connect(print_art,SIGNAL(clicked()),this,SLOT(stampa()));
    //Chiamata della tastiera per stampare
    print_art->setShortcut(tr("Ctrl+P"));
    connect(nuovo1,SIGNAL(clicked()),this,SLOT(nuovo()));
    //Chiamata della tastiera per inserire un nuovo articolo
    nuovo1->setShortcut(tr("Ctrl+N"));
    connect(modifica,SIGNAL(clicked()),this,SLOT(agg_ass()));
    //Chiamata della tastiera per modificare l'articolo
    modifica->setShortcut(tr("Ctrl+M"));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(filtro()));
    //Chiamata della tastiera per cercare gli prodotti_dvd
    pushButton->setShortcut(tr("Ctrl+C"));
    connect(cerca1,SIGNAL(textEdited(QString)),this,SLOT(cerca_libri(QString)));
    connect(ins_image,SIGNAL(clicked()),this,SLOT(image_but()));
    codice_barre = new barcode(this);
    cod_barre->setMaxLength(13);
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(ist_codbarre()));
    connect(print_barcode,SIGNAL(clicked()),this,SLOT(stampacodbarre()));
    connect(st_pdf,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
    connect(st_cvs,SIGNAL(clicked()),this,SLOT(esporta_cvs()));
    mod_grid = new QSqlRelationalTableModel;
    local_settings = new QLocale(QLocale::system());
    dockWidget->setFloating(false);
    dockWidget->setTitleBarWidget(new QWidget);
    dockWidget->setObjectName("Top");
    dockWidget_2->setFloating(false);
    dockWidget_2->setTitleBarWidget(new QWidget);
    dockWidget_2->setObjectName("Top");
    descrizione->adjustSize();
    image_dir->setVisible(false);
    label_11->setVisible(false);
    cod_art->setEnabled(false);
    dockWidget->setMaximumWidth(385);
    QStringList list1;
    list1 << QObject::tr("Cerca per codice a barre") << QObject::tr("Cerca per prodotto") << QObject::tr("Cerca per autore");
    comboBox_2->addItems(list1);
    lista();
    clickgrid();
    combo_iva();
    vis_codbarre();
}

prodotti_dvd::~prodotti_dvd()
{
}

void prodotti_dvd::close_art(){
    close();
}

void prodotti_dvd::cerca_libri(const QString &){

    if(comboBox_2->currentIndex() == 0){
    QString text1;
    text1 = cerca1->text();
    QStringList list_st;
    QSqlQuery querys("select codbarre from prodotti_dvd");
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
    else if(comboBox_2->currentIndex() == 1){
        QString text2;
        text2 = cerca1->text();
        QStringList list_st1;
        QSqlQuery querys1("select nome_prodotto from prodotti_dvd");
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
    else if(comboBox_2->currentIndex() == 2){
        QString text3;
        text3 = cerca1->text();
        QStringList list_st2;
        QSqlQuery querys2("select autore from prodotti_dvd");
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

void prodotti_dvd::filtro(){
    if(cerca1->text().length() == 0){
        lista();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle(tr("LyLibrary"));
        MsgBox.setText(tr("Avviso"));
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca")));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(comboBox_2->currentIndex() == 1){
        mod_grid->setFilter(QString("nome_prodotto = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(comboBox_2->currentIndex() == 0){
        mod_grid->setFilter(QString("codbarre = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(comboBox_2->currentIndex() == 2){
        mod_grid->setFilter(QString("autore = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
}

void prodotti_dvd::nuovo(){

    QSqlQuery query;
    query.exec("SELECT max(cod_prodotto+1) from prodotti_dvd");
    QString id_cl;
    if(query.next()){
        if(query.value(0).toInt() == 0){
                    cod_art->setText("1");
                }
                else{
                id_cl = query.value(0).toString();
                cod_art->setText(id_cl);
                }
    }
    art_nom->setText("");
    cod_barre->setText("");
    descrizione->setText("");
    comboBox->clear();
    quant1->setText("");
    combo_iva();
}

void prodotti_dvd::combo_iva(){

    comboBox->clear();
    QStringList list;
    QSqlQuery query("select tipo_categ from categoria");
    if(query.exec()){
    while(query.next()){
        list << query.value(0).toString();
    }
    QCompleter *complete = new QCompleter(list,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    complete->setCompletionMode(QCompleter::PopupCompletion);
    comboBox->setCompleter(complete);
    comboBox->addItems(list);
    }

    if(comboBox->currentText().length() == 0){
        prodotti_dvd::combo_iva();
    }
}


void prodotti_dvd::delete_art(){

    QString flag_controllo = "NO";
    if(!tab_view->selectionModel()->currentIndex().isValid()){

        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else        if (!cod_art->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM prodotti_dvd WHERE cod_prodotto = :cod_prodotto");
            qctrl.bindValue(":cod_prodotto",cod_art->text());
            qctrl.exec();

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";


                    QMessageBox MsgBox;
                    MsgBox.setText(tr("Voce non eliminabile"));
                    MsgBox.setInformativeText(tr("E' una voce utilizzata in anagrafica clienti"));
                    MsgBox.setIcon(QMessageBox::Warning);
                    MsgBox.exec();

                    return;


             }

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QSqlQuery query;

            query.prepare("DELETE FROM prodotti_dvd WHERE cod_prodotto=:cod_prodotto LIMIT 1");
            query.bindValue(":cod_prodotto",cod_art->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                cod_art->setText("");
                cod_barre->setText("");
                art_nom->setText("");
                descrizione->setText("");
                scaffale->setText("");
                comboBox->clear();
                quant1->setText("");
                textEdit->setText("");
                autore->setText("");
                lingua->setText("");
            }
            else
            {
                //scrivere codice per gestione Errore cancellazione
                QMessageBox MsgBox;
                MsgBox.setText(tr("Voce non eliminabile"));
                MsgBox.setInformativeText(tr("Impossibile eliminare..."));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }


        }
        lista();
        combo_iva();
        emit realodprodotti();
}

void prodotti_dvd::aggiungi(){


        /* Salva il record nel DB. Il salvataggio avviene solo se il valore del campo cd_voce
        presente nella maschera non è nullo (è un campo chiave).
        Se il record esiste già in archivio è effettuato solo l'aggiornamento altrimenti viene
        inserito un record nuovo.
        Il DB presenta come chiave primaria univoca la colonna cd_voce. L'applicativo esegue
        i controlli prima del salvataggio per non incorrere in errori SQL di inserimento chiave
        doppia.
        */

         //Si controlla se il record esiste già sul DB
         QSqlQuery Qctrl;
         Qctrl.prepare("SELECT * FROM prodotti_dvd WHERE cod_prodotto = :cod_prodotto");
         Qctrl.bindValue(":cod_prodotto",cod_art->text());

         Qctrl.exec();

         if (Qctrl.next()) //Se esiste già
         {
             inserisci();
         }
         else
             {
              inserisci();
             }
             this->lista();
         emit realodprodotti();
}

void prodotti_dvd::agg_ass(){

    if(!tab_view->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da modificare..."));
    }
    else{
    QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    tab_view->selectRow(riga);
    aggiorna(newindex);
    }
}

void prodotti_dvd::aggiorna(QModelIndex index){

    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){
    QSqlQuery Query;
    Query.prepare("UPDATE prodotti_dvd SET  codbarre=:codbarre, nome_prodotto=:nome_prodotto, "
                  " descrizione=:descrizione, autore=:autore, lingua=:lingua, infoeditore=:infoeditore, "
                  "categ=:categ, scaffale=:scaffale, quantita=:quantita, image=:image "
                  "WHERE cod_prodotto = :cod_prodotto");

    Query.bindValue(":cod_prodotto",QString::fromUtf8(cod_art->text().toStdString().c_str()));
    Query.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
    Query.bindValue(":nome_prodotto",QString::fromUtf8(art_nom->text().toStdString().c_str()));
    Query.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
    Query.bindValue(":categ",QString::fromUtf8(comboBox->currentText().toStdString().c_str()));
    Query.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
    Query.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
    Query.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
    Query.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
    double quant = local_settings->toDouble(quant1->text().toStdString().c_str());
    Query.bindValue(":quantita",quant);
    Query.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

    if (Query.exec())
    {
        // Aggiornamento effettuato
        cod_art->setText("");
        cod_barre->setText("");
        art_nom->setText("");
        descrizione->setText("");
        quant1->setText("");
        textEdit->setText("");
        autore->setText("");
        lingua->setText("");
        image_dir->setText("");
    }
    else
    {
        // Errore Aggiornamento
        // scrivere codice per per gestione dell'errore

            QMessageBox MsgBox;
            MsgBox.setText(tr("Voce non aggiornabile"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare"));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        qWarning() << Query.lastError();

    }
    }
    lista();
    emit realodprodotti();
}

void prodotti_dvd::inserisci(){

    //Tentativo di inserimento record perché nuovo codice voce

    if(!cod_barre->text().length() && !art_nom->text().length() && !scaffale->text().length() && !descrizione->toPlainText().length()
            && !autore->text().length() && !lingua->text().length() && !quant1->text().length()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci prima i dati correttamente"));
        art_nom->setStyleSheet("background-color: rgb(249, 22, 5)");
        cod_barre->setStyleSheet("background-color: rgb(249, 22, 5)");
        descrizione->setStyleSheet("background-color: rgb(249, 22, 5)");
        scaffale->setStyleSheet("background-color: rgb(249, 22, 5)");
        autore->setStyleSheet("background-color: rgb(249, 22, 5)");
        lingua->setStyleSheet("background-color: rgb(249, 22, 5)");
        quant1->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{

                    QSqlQuery Query;
                    Query.prepare("INSERT INTO prodotti_dvd (cod_prodotto,codbarre,nome_prodotto,descrizione,autore,lingua,infoeditore,categ,scaffale,quantita,image)"
                                  " VALUES (:cod_prodotto,:cod_barre,:nome_prodotto,:descrizione,:autore,:lingua,:infoeditore,:categ,:scaffale,:quantita,:image)");

                    Query.bindValue(":cod_prodotto",QString::fromUtf8(cod_art->text().toStdString().c_str()));
                    Query.bindValue(":codbarre",QString::fromUtf8(cod_barre->text().toStdString().c_str()));
                    Query.bindValue(":nome_prodotto",QString::fromUtf8(art_nom->text().toStdString().c_str()));
                    Query.bindValue(":descrizione",QString::fromUtf8(descrizione->toPlainText().toStdString().c_str()));
                    Query.bindValue(":categ",QString::fromUtf8(comboBox->currentText().toStdString().c_str()));
                    Query.bindValue(":scaffale",QString::fromUtf8(scaffale->text().toStdString().c_str()));
                    Query.bindValue(":autore",QString::fromUtf8(autore->text().toStdString().c_str()));
                    Query.bindValue(":lingua",QString::fromUtf8(lingua->text().toStdString().c_str()));
                    Query.bindValue(":infoeditore",QString::fromUtf8(textEdit->toPlainText().toStdString().c_str()));
                    double quant = local_settings->toDouble(quant1->text().toStdString().c_str());
                    Query.bindValue(":quantita",quant);
                    Query.bindValue(":image",QString::fromUtf8(image_dir->text().toStdString().c_str()));

                    if (Query.exec())
                    {
                        // Aggiornamento effettuato
                        cod_art->setText("");
                        art_nom->setText("");
                        descrizione->setText("");
                        quant1->setText("");
                        textEdit->setText("");
                        autore->setText("");
                        lingua->setText("");
                        image_dir->setText("");
                    }
                    else
                    {
                        // Errore Aggiornamento
                        // scrivere codice per per gestione dell'errore

                            QMessageBox MsgBox;
                            MsgBox.setText(tr("Non puoi inserire"));
                            MsgBox.setInformativeText(tr("Impossibile inserire"));
                            MsgBox.setIcon(QMessageBox::Warning);
                            MsgBox.exec();


                    }
        }
                lista();
                emit realodprodotti();
}

void prodotti_dvd::lista(){

    mod_grid->setTable("prodotti_dvd");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Codice a barre."));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Prodotto."));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Descrizione."));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Autore"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("Lingua"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Categoria"));
    mod_grid->setHeaderData(7,Qt::Horizontal,tr("Scaffale"));
    mod_grid->setHeaderData(8,Qt::Horizontal,QString::fromUtf8(tr("Quantità")));
    mod_grid->setHeaderData(9,Qt::Horizontal,tr("Info editore"));
    mod_grid->setHeaderData(10, Qt::Horizontal, tr("Image"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tab_view->verticalHeader()->hide();
    tab_view->setModel(mod_grid);

    tab_view->setAlternatingRowColors(true);

    tab_view->setItemDelegateForColumn(10, new coldxdelegato(this));


}

void prodotti_dvd::clickgrid(){

        QString id_new;
        int riga = tab_view->selectionModel()->currentIndex().row();
        QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

        id_new = mod_grid->data(elemento).toString();
        cod_art->setText(id_new);
        cerca();
}

void prodotti_dvd::cerca(){


    QSqlQuery query;
    query.prepare("SELECT * FROM prodotti_dvd where cod_prodotto=:cod_prodotto");
        query.bindValue(":cod_prodotto",cod_art->text());
        query.exec();


        if (query.next())
        {
            dettagli->setText(tr("Cod. Art.: ")+query.value(0).toString()+"\n"
                               +tr("Codice a barre: ")+query.value(1).toString()+"\n"
                               +tr("Nome prodotto: ")+query.value(2).toString()+"\n"
                               +tr("Descrizione: ")+query.value(3).toString()+"\n"
                               +tr("Autore: ")+query.value(4).toString()+"\n"
                               +tr("Lingua: ")+query.value(5).toString()+"\n"
                               +tr("Categoria prodotto: ")+query.value(6).toString()+"\n"
                               +tr("Info editore: ")+query.value(9).toString()+"\n"
                               +tr("Scaffale: ")+query.value(7).toString()+"\n"
                               +QString::fromUtf8(tr("Quantità: "))+query.value(8).toString());
            QImage img(query.value(10).toString());
            image_file->setPixmap(QPixmap::fromImage(img));
            cod_barre->setText(query.value(1).toString());
            art_nom->setText(query.value(2).toString());
            descrizione->setText(query.value(3).toString());
            autore->setText(query.value(4).toString());
            lingua->setText(query.value(5).toString());
            comboBox->setCurrentText(query.value(6).toString());
            textEdit->setText(query.value(9).toString());
            scaffale->setText(query.value(7).toString());
            quant1->setText(query.value(8).toString());
            image_dir->setText(query.value(10).toString());
        }
        else
        {
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

            art_nom->setText("");
            cod_barre->setText("");
            descrizione->setText("");
            scaffale->setText("");
            quant1->setText("");
        }
}

void prodotti_dvd::image_but(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("LyLibrary"),
                                     tr("Impossibile aprire %1.").arg(fileName));
            return;
        }
        image_file->setPixmap(QPixmap::fromImage(image));
        image_dir->setText(fileName);
    }
}

void prodotti_dvd::stampa(){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle(tr("Anteprima di stampa."));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
    preview.setWindowIcon(icon);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_new(QPrinter*)));
    preview.exec();

}

void prodotti_dvd::preview_new(QPrinter *printer){

    Q_UNUSED(printer);
    Stampe *pr = new Stampe();
    pr->stampaprodotti(printer,mod_grid);
}

void prodotti_dvd::vis_codbarre(){

    codice_barre->show();

    gridLayout_4->addWidget(codice_barre,3, 1, 1, 1);
}

void prodotti_dvd::ist_codbarre(){

    QModelIndex modelIndex1 = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex1.row();
    QModelIndex newindex1 = modelIndex1.sibling(riga,0);
    tab_view->setCurrentIndex(newindex1);
    tab_view->selectRow(riga);
    view_barre(newindex1);
}

void prodotti_dvd::view_barre(QModelIndex av){

    if(av.row() < mod_grid->rowCount()){
        QSqlQuery qr;
        qr.prepare("select codbarre from prodotti_dvd where cod_prodotto=:cod_prodotto");
        qr.bindValue(":cod_prodotto",cod_art->text());
        qr.exec();

        if(qr.next()){

            QFont nv("IDAutomationHC39M", 8, QFont::Bold);
            cod_barre->setFont(nv);
         codice_barre->updateSwtClicked(qr.value(0).toString());
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile ottenere il barcode...\n")+qr.lastError().text());
        }
    }
}

void prodotti_dvd::stampacodbarre(){
    if(!tab_view->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Seleziona una riga per stampare l'etichetta...."));
    }
    else{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_printbarre(QPrinter*)));
    preview.exec();
    }
}

void prodotti_dvd::preview_printbarre(QPrinter *printer){

    Q_UNUSED(printer);
    QModelIndex in = tab_view->selectionModel()->currentIndex();
    int riga = in.row();
    QModelIndex newindex = in.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    tab_view->selectRow(riga);
    Stampe *report = new Stampe();
    EAN13 *en = new EAN13();
    report->stampabarcode(printer,mod_grid,&newindex,en);
}

void prodotti_dvd::bar_code_en(){
    cod_barre->setVisible(true);
    codice_barre->setVisible(true);
    groupBox_2->setVisible(true);
}

void prodotti_dvd::esporta_pdf()
{
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
       Stampe *ex_pdf = new Stampe();
       ex_pdf->stampaprodotti(printer,mod_grid);
}

void prodotti_dvd::esporta_cvs()
{
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
