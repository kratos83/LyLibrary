#include <QtSql/QtSql>
#include <QCompleter>
#include <QStringList>
#include <QVariant>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintPreviewDialog>
#include <QTextEdit>
#include <QMenu>
#include <math.h>
#include <time.h>
#include "settingsmanager.h"
#include "articoli.h"
#include "ui_articoli.h"
#include "itdelegato.h"
#include "stampa/StampaBiblioteca.h"
#include "print.h"
#include "../xlsx/xlsxdocument.h"
#include "../xlsx/xlsxformat.h"
using namespace QXlsx;

articoli *prodotto = 0;

articoli::articoli(QWidget *parent) : QDialog(parent)
{

    setupUi(this);
    setWindowModality(Qt::WindowModal);
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
    //Chiamata della tastiera per cercare gli articoli
    pushButton->setShortcut(tr("Ctrl+C"));
    connect(cerca1,SIGNAL(textEdited(QString)),this,SLOT(cerca_libri(QString)));
    connect(ins_image,SIGNAL(clicked()),this,SLOT(image_but()));
    codice_barre = new barcode;
    cod_barre->setMaxLength(13);
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(ist_codbarre()));
    connect(pr_bar,SIGNAL(clicked()),this,SLOT(stampacodbarreart()));
    connect(st_pdf,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
    connect(st_cvs,SIGNAL(clicked()),this,SLOT(esporta_cvs()));
    connect(importa_excel,&QPushButton::clicked,this,&articoli::importExcel);
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
    QStringList list1;
    list1 << tr("Cerca per codice a barre") << tr("Cerca per libro") << tr("Cerca per autore");
    comboBox_2->addItems(list1);
    modifica->setEnabled(false);
    lista();
    combo_iva();
    vis_codbarre();
    nuovo();
}

void articoli::close_art(){
    close();
}

void articoli::cerca_libri(const QString &){

    if(comboBox_2->currentIndex() == 0){
    QString text1;
    text1 = cerca1->text();
    QStringList list_st;
    QSqlQuery querys("select codbarre from articoli");
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
        QSqlQuery querys1("select nome_articolo from articoli");
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
        QSqlQuery querys2("select autore from articoli");
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

void articoli::filtro(){
    if(cerca1->text().length() == 0){
        lista();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle(tr("Lylibrary"));
        MsgBox.setText(tr("Avviso"));
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca").replace("'","''").toStdString().c_str()));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(comboBox_2->currentIndex() == 1){
        mod_grid->setFilter(QString("nome_articolo = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(comboBox_2->currentIndex() == 0){
        QFont nv("IDAutomationHC39M", 8, QFont::Normal);
        cerca1->setFont(nv);
        mod_grid->setFilter(QString("codbarre = '" +display_codbarre(cerca1->text()) +"'" ).arg(display_codbarre(cerca1->text())));
    }
    else if(comboBox_2->currentIndex() == 2){
        mod_grid->setFilter(QString("autore = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
}

void articoli::nuovo(){

    QSqlQuery query;
    query.exec("SELECT max(cod_articolo+1) from articoli");
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

void articoli::combo_iva(){

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
        articoli::combo_iva();
    }
}

void articoli::clear_lin_text()
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

void articoli::delete_art(){

    QString flag_controllo = "NO";
    if(!tab_view->selectionModel()->currentIndex().isValid()){

        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!cod_art->text().isEmpty())
        {
            QMessageBox::StandardButton messaggio;
            messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler eliminare il libro "+art_nom->text()+"?",QMessageBox::Yes|QMessageBox::No);
            if(messaggio == QMessageBox::Yes)
            {
                QSqlQuery qctrl;
                qctrl.prepare("SELECT * FROM articoli WHERE cod_articolo='"+cod_art->text()+"'");
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
                        MsgBox.setInformativeText(tr("E' una voce utilizzata in anagrafica alunni"));
                        MsgBox.setIcon(QMessageBox::Warning);
                        MsgBox.exec();
                        return;
                }
                // Se si passano i controlli di cui sopra si procede all'eliminazione
                QSqlQuery query;
                query.prepare("DELETE FROM articoli WHERE cod_articolo='"+cod_art->text()+"' LIMIT 1");
                if (query.exec()) //Se esito OK Eliminato da DB
                {
                    clear_lin_text();
                    modifica->setEnabled(false);
                    agg_art->setEnabled(true);
                }
                else
                {
                    QMessageBox MsgBox;
                    MsgBox.setText(tr("Voce non eliminabile"));
                    MsgBox.setInformativeText(tr("Impossibile eliminare..."));
                    MsgBox.setIcon(QMessageBox::Warning);
                    MsgBox.exec();
                }
            }
        }
        lista();
        combo_iva();
        emit article_prod();
}

void articoli::aggiungi(){

         //Si controlla se il record esiste già sul DB
         QSqlQuery Qctrl;
         Qctrl.prepare("SELECT * FROM articoli WHERE cod_articolo='"+cod_art->text()+"'");
         Qctrl.exec();
         if (Qctrl.next()) //Se esiste già
         {
            QMessageBox::StandardButton messaggio;
            messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler inserire il libro "+art_nom->text()+"?",QMessageBox::Yes|QMessageBox::No);
            if(messaggio == QMessageBox::Yes)
            {
                inserisci();
            }
         }
         else
             {
                QMessageBox::StandardButton messaggio;
                messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler inserire il libro "+art_nom->text()+"?",QMessageBox::Yes|QMessageBox::No);
                if(messaggio == QMessageBox::Yes)
                {
                    inserisci();
                }
             }
             this->lista();
         emit article_prod();
}

void articoli::agg_ass(){

    if(!tab_view->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da modificare..."));
    }
    else{
        QMessageBox::StandardButton messaggio;
        messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler aggiornare il libro "+art_nom->text()+"?",QMessageBox::Yes|QMessageBox::No);
        if(messaggio == QMessageBox::Yes)
        {
            modifica->setEnabled(true);
            agg_art->setEnabled(false);
            QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
            int riga = modelIndex.row();
            QModelIndex newindex = modelIndex.sibling(riga,0);
            tab_view->setCurrentIndex(newindex);
            tab_view->selectRow(riga);
            aggiorna(newindex);
        }
    }
}

void articoli::aggiorna(QModelIndex index){

    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){
    QSqlQuery Query;
    Query.prepare("UPDATE articoli SET  codbarre='"+cod_barre->text()+"', nome_articolo='"+art_nom->text()+"', "
                  " descrizione='"+descrizione->toPlainText()+"', autore='"+autore->text()+"', "
		  " lingua='"+lingua->text()+"', infoeditore='"+textEdit->toPlainText()+"', "
                  " categ='"+comboBox->currentText()+"', scaffale='"+scaffale->text()+"', "
		  " quantita='"+quant1->text()+"', image='"+image_dir->text()+"' "
                  "WHERE cod_articolo ='"+cod_art->text()+"'");
    if (Query.exec())
    {
        // Aggiornamento effettuato
        clear_lin_text();
        modifica->setEnabled(false);
        agg_art->setEnabled(true);
    }
    else
    {
            QMessageBox MsgBox;
            MsgBox.setText(tr("Voce non aggiornabile"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare"));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
    }
    }
    lista();
    emit article_prod();
}

void articoli::inserisci(){

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
          Query.prepare("INSERT INTO articoli (cod_articolo,codbarre,nome_articolo,"
			"descrizione,autore,lingua,"
			"categ,scaffale,quantita,"
			"infoeditore,image)"
                         " VALUES ('"+cod_art->text()+"','"+cod_barre->text()+"','"+art_nom->text()+"',"
			 "'"+descrizione->toPlainText()+"','"+autore->text()+"','"+lingua->text()+"',"
			 "'"+comboBox->currentText()+"','"+scaffale->text()+"','"+quant1->text()+"',"
			 "'"+textEdit->toPlainText()+"','"+image_dir->text()+"')");
           if (Query.exec())
           {
               // inserimento effettuato
	            clear_lin_text();
                modifica->setEnabled(false);
                agg_art->setEnabled(true);
           }
           else
           {
              // Errore inserimento
               QMessageBox MsgBox;
               MsgBox.setText(tr("Non puoi inserire"));
               MsgBox.setInformativeText(tr("Impossibile inserire"));
               MsgBox.setIcon(QMessageBox::Warning);
               MsgBox.exec();
           }
           lista();
           emit article_prod();
    }
}

void articoli::lista(){

    mod_grid->setTable("articoli");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Codice a barre."));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Titolo."));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Descrizione."));
    mod_grid->setHeaderData(4, Qt::Horizontal, tr("Autore"));
    mod_grid->setHeaderData(5, Qt::Horizontal, tr("Lingua"));
    mod_grid->setHeaderData(6, Qt::Horizontal, tr("Categoria"));
    mod_grid->setHeaderData(7,Qt::Horizontal,tr("Collocazione"));
    mod_grid->setHeaderData(8,Qt::Horizontal,QString::fromUtf8(tr("Quantità").replace("'","''").toStdString().c_str()));
    mod_grid->setHeaderData(9,Qt::Horizontal,tr("Info editore"));
    mod_grid->setHeaderData(10, Qt::Horizontal, tr("Image"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tab_view->verticalHeader()->hide();
    tab_view->setModel(mod_grid);

    tab_view->setAlternatingRowColors(true);

    tab_view->setItemDelegateForColumn(10, new coldxdelegato(this));
}

void articoli::clickgrid(){

        QString id_new;
        int riga = tab_view->selectionModel()->currentIndex().row();
        QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

        id_new = mod_grid->data(elemento).toString();
        cod_art->setText(id_new);
        cerca();
        modifica->setEnabled(true);
        agg_art->setEnabled(false);
}

void articoli::cerca(){
    QSqlQuery query;
    query.prepare("SELECT * FROM articoli where cod_articolo='"+cod_art->text()+"'");
    query.exec();
    
        if (query.next())
        {
            dettagli->setText(QObject::tr("Cod. Art.: ")+query.value(0).toString()+"\n"+
                               QObject::tr("Codice a barre: ")+query.value(1).toString()+"\n"+
                                  QObject::tr("Titolo: ")+query.value(2).toString()+"\n"+
                                  QObject::tr("Descrizione: ")+query.value(3).toString()+"\n"+
                                  QObject::tr("Autore: ")+query.value(4).toString()+"\n"+
                                  QObject::tr("Lingua: ")+query.value(5).toString()+"\n"+
                                  QObject::tr("Categoria libro: ")+query.value(6).toString()+"\n"+
                                  QObject::tr("Info editore: ")+query.value(9).toString()+"\n"+
                                  QObject::tr("Collocazione: ")+query.value(7).toString()+"\n"+
                                  QString::fromUtf8(QObject::tr("Quantità: ").replace("'","''").toStdString().c_str())+query.value(8).toString());
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
            // Elemento non trovato
	    clear_lin_text();
        }
}

void articoli::image_but(){
    QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("Apri file"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, QObject::tr("Lylibrary"),QObject::tr("Impossibile aprire %1.").arg(fileName));
            return;
        }
        image_file->setPixmap(QPixmap::fromImage(image));
        image_dir->setText(fileName);
    }
}

void articoli::stampa(){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle(QObject::tr("Anteprima di stampa."));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
    preview.setWindowIcon(icon);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_new(QPrinter*)));
    preview.exec();

}

void articoli::preview_new(QPrinter *printer){

    Q_UNUSED(printer);
    QTextDocument *doc = new QTextDocument;
    StampaBiblioteca *ex_pdf = new StampaBiblioteca();
    ex_pdf->stampaBiblioteca(printer,mod_grid,doc);
}

void articoli::esporta_pdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Esporta PDF"),
                                                    "*.pdf", QObject::tr("Pdf Files(*.pdf);;Tutti i file(*.*)"));

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
       StampaBiblioteca *ex_pdf = new StampaBiblioteca();
       ex_pdf->stampaBiblioteca(printer,mod_grid,doc);
      }
}

void articoli::esporta_cvs()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Esporta CSV"),
                                                    "*.csv", QObject::tr("CSV(*.csv);;Tutti i file(*.*)"));

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
    ttext.append(exportdata.toUtf8());
    file.write(ttext);
}

void articoli::vis_codbarre(){

    gridLayout_11->addWidget(codice_barre,2, 1, 1, 1);

    codice_barre->show();

}

void articoli::ist_codbarre(){

    QModelIndex modelIndex1 = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex1.row();
    QModelIndex newindex1 = modelIndex1.sibling(riga,0);
    tab_view->setCurrentIndex(newindex1);
    tab_view->selectRow(riga);
    view_barre(newindex1);
}

void articoli::view_barre(QModelIndex av){

    if(av.row() < mod_grid->rowCount()){
        QSqlQuery qr;
        qr.prepare("select codbarre from articoli where cod_articolo='"+cod_art->text()+"'");
        qr.exec();

        if(qr.next()){
            QFont nv("IDAutomationHC39M", 8, QFont::Normal);
            cod_barre->setFont(nv);
	    codice_barre->updateSwtClicked(qr.value(0).toString());
        }
        else{
            QMessageBox::warning(this,QObject::tr("LyLibrary"),QObject::tr("Impossibile ottenere il barcode....\n")+qr.lastError().text());
        }
    }
}

QString articoli::display_codbarre(QString text)
{
    QSqlQuery qr;
    qr.prepare("select * from articoli where codbarre='"+text+"'");
    qr.exec();

    if(qr.next()){
        QFont nv("IDAutomationHC39M", 8, QFont::Normal);
        cerca1->setFont(nv);
    }
    else{
        QMessageBox::warning(this,QObject::tr("LyLibrary"),QObject::tr("Impossibile ottenere il barcode....\n")+qr.lastError().text());
    }

    return text;
}

void articoli::bar_code_en(){
    cod_barre->setVisible(true);
    codice_barre->setVisible(true);
    groupBox_2->setVisible(true);
}

void articoli::stampacodbarreart(){

    if(!tab_view->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,QObject::tr("LyLibrary"),QObject::tr("Seleziona una riga per stampare l'etichetta...."));
    }
    else{
        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer);
        preview.setWindowFlags(Qt::Window);
        connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_printbarreart(QPrinter*)));
        preview.exec();
    }
}

void articoli::preview_printbarreart(QPrinter *printer){

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

bool articoli::eventFilter(QObject *o, QEvent *e){

    QKeyEvent* ke = (QKeyEvent*)e;
    //Add event to menu key
    if (o==tab_view && e->type() == QEvent::KeyPress) {

                    if (ke->key() == Qt::Key_Delete) {
                        // ESEGUO IL CODICE DI ELIMINAZIONE
                        delete_art();
                    }
                    if(ke->key() == Qt::Key_S){
                        //ESEGUO IL CODICE PER INSERIRE
                        aggiungi();
                    }
                    if(ke->key() == Qt::Key_U){
                        //ESEGUO IL CODICE PER AGGIORNARE
                        agg_ass();
                    }
                    if(ke->key() == Qt::Key_N){
                        //ESEGUO IL CODICE PER UN NUOVO ARTICOLO
                        nuovo();
                    }
                    if(ke->key() == Qt::Key_P){
                        //ESEGUO IL CODICE DI STAMPA
                        stampa();
                    }
       }

    //Installazione eventi Qt::Key_Tab
        if( o==tab_view && e->type() == QEvent::ContextMenu)
                {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (e);
                    this ->Popup(mouseEvent->pos());
                    return false;
        }

        return QDialog::eventFilter(o,e);

}

void articoli::mousePressEvent(QMouseEvent *event){

    /******************************************
      * Imposta il tasto destro per il mouse
      * riproducendo un menu contestuale
      *****************************************/
    if (event->button() == Qt::RightButton) {

                    this->Popup(event ->pos());
                    event->accept();
                }
}

void  articoli::Popup(const QPoint &pt){

                /**************************************************
                  * Imposta la variabile pt per abilitare il
                  * menu.
                  * @param global = mapToGlobal(pt)
                  * @author Angelo Scarnà
                  *************************************************/
                QPoint global = this ->mapToGlobal(pt);
                QMenu* pPopup = new QMenu(this);

                /***********************************************************
                *  @author Angelo Scarnà
                *  Immagini per il menu a tendina
                ************************************************************/
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/images/dialog-close.png"));
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/images/edit-delete.png"));
                QIcon icon2;
                icon2.addFile(QString::fromUtf8(":/images/filenew.png"));
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/images/document-save.png"));
                QIcon icon4;
                icon4.addFile(QString::fromUtf8(":/images/view-refresh.png"));
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/images/document-print.png"));

                /***********************************************************
                *  @author Angelo Scarnà
                *  Menu a tendina
                ************************************************************/
                QAction* pAction1 = new QAction(QObject::tr("Chiudi"), this);
                pAction1->setIcon(icon);
                pAction1->setIconVisibleInMenu(true);
                QAction* pAction2 = new QAction(QObject::tr("Elimina"), this);
                pAction2->setIcon(icon1);
                pAction2->setIconVisibleInMenu(true);
                QAction* nw = new QAction(QObject::tr("Nuovo"),this);
                nw->setIcon(icon2);
                nw->setIconVisibleInMenu(true);
                QAction* sa = new QAction(QObject::tr("Salva"),this);
                sa->setIcon(icon3);
                sa->setIconVisibleInMenu(true);
                QAction* md = new QAction(QObject::tr("Aggiorna"),this);
                md->setIcon(icon4);
                md->setIconVisibleInMenu(true);
                QAction* pr = new QAction(QObject::tr("Stampa"), this);
                pr->setIcon(icon5);
                pr->setIconVisibleInMenu(true);

                /***********************************************************
                *  @author Angelo Scarnà
                *  Instanzio il menu
                ************************************************************/
                pPopup->addAction(nw);
                pPopup->addSeparator();
                pPopup->addAction(sa);
                pPopup->addSeparator();
                pPopup->addAction(md);
                pPopup->addSeparator();
                pPopup->addAction(pAction2);
                pPopup->addSeparator();
                pPopup->addAction(pr);
                pPopup->addSeparator();
                pPopup->addAction(pAction1);

                /***********************************************************
                *  @author Angelo Scarnà
                *  Imposto la connessione ai moduli
                ************************************************************/
                QAction* pItem = pPopup ->exec(global);

                if(pItem == pAction1)
                {
                    close();
                }
                else if(pItem == pAction2)
                {
                    delete_art();
                }
                else if(pItem == nw){
                    nuovo();
                }
                else if(pItem == sa)
                {
                    aggiungi();
                }
                else if(pItem == md)
                {
                    agg_ass();
                }
                else if(pItem == pr)
                {
                    stampa();
                }
}

void articoli::reload_list()
{
    time_t t;
    t = time(NULL);

    while( difftime(time(NULL),t) < 2.0  ){
        lista();
    }
}

void articoli::importExcel()
{
    //Procedura per importare gli alunni 
    //mediante file excel
    QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle("LyLibrary");
        box->setText("Il file excel\ndeve essere composto\n");
        box->setInformativeText("dalle sole colonne Codice a barre, titolo del libro, descrizione, autore, lingua, categoria, scaffale, quantità");
        box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box->setDefaultButton(QMessageBox::Ok);
        int ret = box->exec();
        switch(ret) {
        case QMessageBox::Ok:{
            QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Apri file xlsx"),
                                                                        "*.xlsx", QObject::tr("File XLSX(*.xlsx);;Tutti i file(*.*)"));

            if (fileName.isEmpty())
            return;
            else {
            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Impossibile aprire il file."),
                        file.errorString());
                        return;
            }
            Document xlsx(fileName);
            if(xlsx.dimension().columnCount() > 9)
            {
                QMessageBox::warning(this,"LyLibrary","Il file contiene più di 10 colonne.");
            }
            else{
                for(int i=1; i <= xlsx.dimension().rowCount(); i++)
                {
                    QProgressDialog progressDialog(this);
                    progressDialog.setRange(0, xlsx.dimension().rowCount());
                    progressDialog.setWindowModality(Qt::WindowModal);
                    progressDialog.setWindowTitle(tr("Inserimento libri"));
                    progressDialog.setValue(i);
                    progressDialog.setLabelText(tr("Inserimento libri %1 di %2...")
                                                    .arg(i).arg(xlsx.dimension().rowCount()));
                    qApp->processEvents();
                    
                    QSqlQuery query;
                    QString name_surname;
                    QString req = "INSERT INTO articoli(codbarre,nome_articolo,descrizione,autore,lingua,categ,scaffale,quantita) VALUES('";
                    if(Cell *m_codbarre = xlsx.cellAt(i,1)){//codice a barre
                    req.append(QString(m_codbarre->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname = QString(m_codbarre->value().toString().toStdString().c_str()).replace("'","''");
                    req.append("','");
                    }
                    if(Cell *nome_articolo = xlsx.cellAt(i,2)){//nome_articolo
                    req.append(QString(nome_articolo->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(nome_articolo->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_descrizione = xlsx.cellAt(i,3)){//descrizione
                    req.append(QString(m_descrizione->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_descrizione->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_autore = xlsx.cellAt(i,4)){//autore
                    req.append(QString(m_autore->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_autore->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_lingua = xlsx.cellAt(i,5)){//lingua
                    req.append(QString(m_lingua->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_lingua->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_categoria = xlsx.cellAt(i,6)){//categoria
                    req.append(QString(m_categoria->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_categoria->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_scaffale = xlsx.cellAt(i,7)){//scaffale
                    req.append(QString(m_scaffale->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_scaffale->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_quantita = xlsx.cellAt(i,8)){//quantita
                    req.append(QString(m_quantita->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(m_quantita->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("'");
                    }
                    req.append(");");
                    qDebug() << req;
                    query.prepare(req);
                    progressDialog.show();
                    progressDialog.update();
                    
                    query.prepare(req);
                    if(query.exec())
                        qDebug() << "Inserimento effettuato con successo";
                    else
                        qDebug() << "Errore: " << query.lastError().text();
                }
            }
            }
            lista();
            break;
            }
        case QMessageBox::Cancel:
            //Close
            QMessageBox::warning(this,"LyLibrary","Nessun file selezionato.");
            box->close();
        break;
    }

}

articoli::~articoli()
{
}

