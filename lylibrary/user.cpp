/*
 * Libreria Utente
 */
#include "user.h"
#include "ui_user.h"
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QtSql>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QCompleter>
#include <QMenu>
#include <QFileDialog>
#include "itdelegato.h"
#include "stampa/StampaUtenti.h"

#include "../xlsx/xlsxdocument.h"
#include "../xlsx/xlsxformat.h"
using namespace QXlsx;

user::user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->elimina,SIGNAL(clicked()),this,SLOT(del_rec()));
    connect(ui->salva,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->g_lista, SIGNAL(clicked(QModelIndex)), this, SLOT(clickgrid()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(nuovo()));
    connect(ui->preview,SIGNAL(clicked()),this,SLOT(anteprima()));
    connect(ui->cerca_user,SIGNAL(clicked()),this,SLOT(filtro()));
    connect(ui->cerca,SIGNAL(textEdited(QString)),this,SLOT(cerca_utente(QString)));
    connect(ui->pdf_p,SIGNAL(clicked()),this,SLOT(stampa_pdf()));
    connect(ui->cvs,SIGNAL(clicked()),this,SLOT(esporta_cvs()));
    connect(ui->import_excel,&QPushButton::clicked,this,&user::importExcel);
    connect(ui->combo_pref,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::textActivated),[=](){
        ui->cerca->clear();
    });
    ui->id->setEnabled(false);
    ui->totale_list->setReadOnly(true);
    mod_grid = new QSqlRelationalTableModel;
    lista();
    somma();
    nuovo();
}

user::~user()
{
    delete ui;
}

void user::somma(){

    QString str;
    QSqlQuery query;
    query.exec("select max(id) from clienti");
    if(query.next()){
       str = query.value(0).toString();
       ui->totale_list->setText(QString::fromUtf8("N°  ")+str);
    }
}
void user::nuovo(){

    QSqlQuery query;
    query.exec("SELECT max(id+1) from clienti");

    if(query.next()){
        if(query.value(0).toInt() == 0){
              ui->id->setText("1");
        }
        else{
           ui->id->setText(query.value(0).toString());
        }
    }
    ui->nome->setText("");
    ui->cognome->setText("");
    ui->indirizzo->setText("");
    ui->telefono->setText("");
    ui->email->setText("");
    somma();
}

void user::on_esci_clicked()
{
        close();
}

void user::save(){

        /* Salva il record nel DB. Il salvataggio avviene solo se il valore del campo cd_voce
        presente nella maschera non è nullo (è un campo chiave).
        Se il record esiste già in archivio è effettuato solo l'aggiornamento altrimenti viene
        inserito un record nuovo.
        Il DB presenta come chiave primaria univoca la colonna cd_voce. L'applicativo esegue
        i controlli prima del salvataggio per non incorrere in errori SQL di inserimento chiave
        doppia.
        */

    if(ui->id->text() == "" || ui->nome->text().isEmpty() || ui->cognome->text().isEmpty() || ui->cod_f->text().isEmpty() || ui->part_iva->text().isEmpty()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati: Nome, cognome, classe e sezione"));
        ui->nome->setStyleSheet("QLineEdit{background-color: rgba(255, 0, 0, 0.3);}");
        ui->cognome->setStyleSheet("QLineEdit{background-color: rgba(255, 0, 0, 0.3);}");
        ui->cod_f->setStyleSheet("QLineEdit{background-color: rgba(255, 0, 0, 0.3);}");
        ui->part_iva->setStyleSheet("QLineEdit{background-color: rgba(255, 0, 0, 0.3);}");
    }
    else{
         QSqlQuery Qctrl;
         if(!ui->id->text().isEmpty()){
         Qctrl.prepare("SELECT * FROM clienti WHERE id = :id");
         Qctrl.bindValue(":id",ui->id->text());

         Qctrl.exec();

         if (Qctrl.next()) //Se esiste già
         {
             //Tentativo di aggiornamento del record perché trovato
             QMessageBox::StandardButton reply;
             reply = QMessageBox::information(this,"LyLibrary","Vuoi modificare "+ui->nome->text() + " " + ui->cognome->text()+"?",QMessageBox::Ok | QMessageBox::Cancel);
             if(reply == QMessageBox::Ok)
             {
                QSqlQuery Query;

                Query.prepare("UPDATE clienti SET nome = :nome, cognome = :cognome, indirizzo=:indirizzo, "
                            "telefono=:telefono, email=:email, cod_fisc=:cod_fisc, part_iva=:part_iva, fax=:fax "
                            "WHERE id= :id");

                Query.bindValue(":id",ui->id->text());
                Query.bindValue(":nome", QString::fromUtf8(ui->nome->text().toStdString().c_str()));
                Query.bindValue(":cognome",QString::fromUtf8(ui->cognome->text().toStdString().c_str()));
                Query.bindValue(":indirizzo",QString::fromUtf8(ui->indirizzo->text().toStdString().c_str()));
                Query.bindValue(":telefono",QString::fromUtf8(ui->telefono->text().toStdString().c_str()));
                Query.bindValue(":email",QString::fromUtf8(ui->email->text().toStdString().c_str()));
                Query.bindValue(":cod_fisc",QString::fromUtf8(ui->cod_f->text().toStdString().c_str()));
                Query.bindValue(":part_iva",QString::fromUtf8(ui->part_iva->text().toStdString().c_str()));
                Query.bindValue(":fax",QString::fromUtf8(ui->fax->text().toStdString().c_str()));

                if (Query.exec())
                {
                    // Aggiornamento effettuato
                    clearText();
                }
                else
                {
                    // Errore Aggiornamento
                    // scrivere codice per per gestione dell'errore
                    qWarning() << Query.lastError();

                }
             }
         }
         else //Se invece non esiste il record..
         {
             inserisci();
         }

    lista();
    somma();
    emit clienti_rel();
    }
    }
}

void user::clearText()
{
    ui->nome->setText("");
    ui->cognome->setText("");
    ui->indirizzo->setText("");
    ui->telefono->setText("");
    ui->email->setText("");
    ui->cod_f->setText("");
    ui->part_iva->setText("");
    ui->fax->setText("");
    ui->nome->setStyleSheet("QLineEdit{background-color: transparent;}");
    ui->cognome->setStyleSheet("QLineEdit{background-color: transparent;}");
    ui->cod_f->setStyleSheet("QLineEdit{background-color: transparent;}");
    ui->part_iva->setStyleSheet("QLineEdit{background-color: transparent;}");
    nuovo();
}

void user::inserisci(){

    //Tentativo di inserimento record perché nuovo codice voce
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this,"LyLibrary","Vuoi inseire "+ui->nome->text() + " " + ui->cognome->text()+"?",QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok)
    {
        QSqlQuery Query;

        Query.prepare("INSERT INTO clienti (id, nome, cognome,telefono,indirizzo,email,cod_fisc,part_iva,fax )"
                    " VALUES (:id, :nome, :cognome, :telefono, :indirizzo, :email, :cod_fisc, :part_iva, :fax )");

        Query.bindValue(":id",ui->id->text());
        Query.bindValue(":nome", QString::fromUtf8(ui->nome->text().toStdString().c_str()));
        Query.bindValue(":cognome",QString::fromUtf8(ui->cognome->text().toStdString().c_str()));
        Query.bindValue(":indirizzo",QString::fromUtf8(ui->indirizzo->text().toStdString().c_str()));
        Query.bindValue(":telefono",QString::fromUtf8(ui->telefono->text().toStdString().c_str()));
        Query.bindValue(":email",QString::fromUtf8(ui->email->text().toStdString().c_str()));
        Query.bindValue(":cod_fisc",QString::fromUtf8(ui->cod_f->text().toStdString().c_str()));
        Query.bindValue(":part_iva",QString::fromUtf8(ui->part_iva->text().toStdString().c_str()));
        Query.bindValue(":fax",QString::fromUtf8(ui->fax->text().toStdString().c_str()));

        if (Query.exec()) //Se esito OK inserimento DB
        {   
            clearText();
        }
        else
        {
            // Scrivere codice per errore inserimento
            qWarning()<<Query.lastError();
        }

        //Fine Tentativo di inserimento
        lista();
        somma();
        emit clienti_rel();
    }
}

void user::del_rec(){

    QString flag_controllo = "NO";
    if(!ui->g_lista->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!ui->id->text().isEmpty())
        {

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM clienti WHERE id = :id");
            qctrl.bindValue(":id",ui->id->text());
            qctrl.exec();

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this,"LyLibrary","Vuoi eliminare "+ui->nome->text() + " " + ui->cognome->text()+"?",QMessageBox::Ok | QMessageBox::Cancel);
            if(reply == QMessageBox::Ok)
            {
                QSqlQuery query;

                query.prepare("DELETE FROM clienti WHERE id = :id");
                query.bindValue(":id",ui->id->text());

                if (query.exec()) //Se esito OK Eliminato da DB
                {
                    clearText();
                }
                else
                {
                    QMessageBox MsgBox;
                    MsgBox.setText(QString::fromUtf8(tr("Errore").toStdString().c_str()));
                    MsgBox.setInformativeText(QString::fromUtf8(tr("Impossibile eliminare record poichè non hai selezionato nulla").toStdString().c_str()));
                    MsgBox.setIcon(QMessageBox::Warning);
                    MsgBox.exec();
                }
            }
        }

       lista();
       somma();
       emit clienti_rel();
}
void user::lista(){


        mod_grid->setTable("clienti");
        mod_grid->select();
        mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
        mod_grid->setHeaderData(1, Qt::Horizontal, tr("Nome"));
        mod_grid->setHeaderData(2, Qt::Horizontal, tr("Cognome."));
        mod_grid->setHeaderData(3,Qt::Horizontal,tr("Indirizzo."));
        mod_grid->setHeaderData(4,Qt::Horizontal,tr("Telefono."));
        mod_grid->setHeaderData(5,Qt::Horizontal,tr("E-mail."));
        mod_grid->setHeaderData(6,Qt::Horizontal,tr("Classe"));
        mod_grid->setHeaderData(7,Qt::Horizontal,tr("Sezione"));
        mod_grid->setHeaderData(8,Qt::Horizontal,tr("Fax"));

        ui->g_lista->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->g_lista->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->g_lista->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->g_lista->verticalHeader()->hide();
        ui->g_lista->setModel(mod_grid);
        ui->g_lista->setAlternatingRowColors(true);

        ui->g_lista->setItemDelegateForColumn(6, new coldxdelegato(this));
}

void user::cerca(){

    QSqlQuery query;
        query.prepare("SELECT id, nome, cognome,indirizzo,telefono,email,cod_fisc,part_iva,fax FROM clienti WHERE id = :id");
        query.bindValue(":id",ui->id->text());
        query.exec();

        if (query.next())
        {
            ui->dettagli->setText(tr("ID: ")+query.value(0).toString()+"\n"
                                  +tr("Nome: ")+query.value(1).toString()+"\n"
                                  +tr("Cognome: ")+query.value(2).toString()+"\n"
                                  +tr("Indirizzo: ")+query.value(3).toString()+"\n"
                                  +tr("Telefono: ")+query.value(4).toString()+"\n"
                                  +tr("E-mail: ")+query.value(5).toString()+"\n"
                                  +tr("Classe: ")+query.value(6).toString()+"\n"
                                  +tr("Sezione: ")+query.value(7).toString()+"\n"
                                  +tr("Fax: ")+query.value(8).toString()+"\n");
            ui->nome->setText(query.value(1).toString());
            ui->cognome->setText(query.value(2).toString());
            ui->indirizzo->setText(query.value(3).toString());
            ui->telefono->setText(query.value(4).toString());
            ui->email->setText(query.value(5).toString());
            ui->cod_f->setText(query.value(6).toString());
            ui->part_iva->setText(query.value(7).toString());
            ui->fax->setText(query.value(8).toString());
        }
        else
        {
            clearText();
        }

}

void user::clickgrid(){

        QString id_new;
        int riga = ui->g_lista->currentIndex().row();
        QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

        id_new = mod_grid->data(elemento).toString();
        ui->id->setText(id_new);

        cerca();
}

void user::cerca_utente(const QString &){

    if(ui->combo_pref->currentIndex() == 0){
    QString text;
    text = ui->cerca->text();
    QStringList list_st;
    QSqlQuery querys("select nome,cognome from clienti");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString() +" "+querys.value(1).toString();
    }
    }
    list_st.removeDuplicates();
    QCompleter *complete = new QCompleter(list_st,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    ui->cerca->setCompleter(complete);
    if(text.length() == 0){
        lista();
    }
    }
    else if(ui->combo_pref->currentIndex() == 1){
        QString text1;
        text1 = ui->cerca->text();
        QStringList list_st1;
        QSqlQuery querys1("select cod_fisc,part_iva from clienti");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString()+" "+querys1.value(1).toString();
        }
        }
        list_st1.removeDuplicates();
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        ui->cerca->setCompleter(complete1);
        if(text1.length() == 0){
            lista();
        }
    }
}

void user::filtro(){

    if(ui->cerca->text().length() == 0){
        lista();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle(tr("LyLibrary"));
        MsgBox.setText(tr("Avviso"));
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca").toStdString().c_str()));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(ui->combo_pref->currentIndex() == 0){
        QStringList lista  = ui->cerca->text().split(" ");
        QString nome = lista.at(0);
        QString cognome = lista.at(1);
        mod_grid->setFilter(QString("nome = '" +nome.replace("'","''")+"' and cognome ='"+cognome.replace("'","''")+"'").arg(ui->cerca->text().replace("'","''")));
    }
    else if(ui->combo_pref->currentIndex() == 1){
        QStringList lista  = ui->cerca->text().split(" ");
        QString classe = lista.at(0);
        QString sezione = lista.at(1);
        mod_grid->setFilter(QString("cod_fisc = '" +classe+"' and part_iva = '"+sezione.replace("'","''")+"'").arg(ui->cerca->text().replace("'","''")));
    }
}

void user::anteprima(){
     //Aggiornamento query
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

void user::preview_new(QPrinter *printer){

    Q_UNUSED(printer);
    QTextDocument *doc = new QTextDocument;
    StampaUtenti *m_stampa = new StampaUtenti(this);
    m_stampa->stampaUtente(printer,mod_grid,doc);
}

void user::stampa_pdf(){

    QString fileName = QFileDialog::getSaveFileName(this, tr("Esporta PDF"),
                                                    "*.pdf", tr("Pdf Files(*.pdf);;Tutti i file(*.*)"));

    if (fileName.length() != 0) {
            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".pdf") < 0) {
                fileName += ".pdf";
            }
        }
     if(!fileName.isEmpty())
     {
       QPrinter *printer = new QPrinter(QPrinter::HighResolution);
       printer->setPageSize(QPageSize(QPageSize::A4));
       printer->setPageOrientation(QPageLayout::Landscape);
       printer->setOutputFileName(fileName);
       QTextDocument *doc = new QTextDocument;
       StampaUtenti *m_stampa = new StampaUtenti(this);
       m_stampa->stampaUtente(printer,mod_grid,doc);
     }
}

void user::esporta_cvs()
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

    counthidden+=ui->g_lista->isColumnHidden(jint);
    jint++;
    }


    int w = 1;
    while (x < mod_grid->columnCount()){

    if (!ui->g_lista->isColumnHidden(x)) {


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
    if (!ui->g_lista->isColumnHidden(x)) {


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

bool user::eventFilter(QObject *o, QEvent *e){

    //Add event to menu key
    if (o==ui->g_lista && e->type() == QEvent::KeyPress) {
                    QKeyEvent* ke = (QKeyEvent*)e;
                    if (ke->key() == Qt::Key_Delete) {
                        // ESEGUO IL CODICE DI ELIMINAZIONE
                        del_rec();
                    }
                    if(ke->key() == Qt::Key_S){
                        //ESEGUO IL CODICE PER INSERIRE
                        inserisci();
                    }
                    if(ke->key() == Qt::Key_U){
                        //ESEGUO IL CODICE PER AGGIORNARE
                        save();
                    }
                    if(ke->key() == Qt::Key_N){
                        //ESEGUO IL CODICE PER UN NUOVO ARTICOLO
                        nuovo();
                    }
                    if(ke->key() == Qt::Key_P){
                        //ESEGUO IL CODICE DI STAMPA
                        anteprima();
                    }
                    if(ke->key() == Qt::Key_C){
                        //ESEGUO IL CODICE DI ESPORTAZIONE PDF
                        stampa_pdf();
                    }
                    if(ke->key() == Qt::Key_S){
                        esporta_cvs();
                    }
                }

        if( o==ui->g_lista && e->type() == QEvent::ContextMenu)
                {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (e);
                    this ->Popup(mouseEvent->pos());
                    return false;
        }

        return QDialog::eventFilter(o,e);

}

void user::mousePressEvent(QMouseEvent *event){

    /******************************************
      * Imposta il tasto destro per il mouse
      * riproducendo un menu contestuale
      *****************************************/
    if (event->button() == Qt::RightButton) {

                    this->Popup(event ->pos());
                    event->accept();
                }
}

void  user::Popup(const QPoint &pt){

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
                QIcon icon6;
                icon6.addFile(QString::fromUtf8(":/images/excel.png"));
                QIcon icon7;
                icon7.addFile(QString::fromUtf8(":/images/pdf.png"));


                /***********************************************************
                *  @author Angelo Scarnà
                *  Menu a tendina
                ************************************************************/
                QAction* pAction1 = new QAction(tr("Chiudi"), this);
                pAction1->setIcon(icon);
                pAction1->setIconVisibleInMenu(true);
                QAction* pAction2 = new QAction(tr("Elimina"), this);
                pAction2->setIcon(icon1);
                pAction2->setIconVisibleInMenu(true);
                QAction* nw = new QAction(tr("Nuovo"),this);
                nw->setIcon(icon2);
                nw->setIconVisibleInMenu(true);
                QAction* sa = new QAction(tr("Salva"),this);
                sa->setIcon(icon3);
                sa->setIconVisibleInMenu(true);
                QAction* md = new QAction(tr("Aggiorna"),this);
                md->setIcon(icon4);
                md->setIconVisibleInMenu(true);
                QAction* pr = new QAction(tr("Stampa"), this);
                pr->setIcon(icon5);
                pr->setIconVisibleInMenu(true);
                QAction *expdf = new QAction(tr("Esporta pdf"),this);
                expdf->setIcon(icon7);
                expdf->setIconVisibleInMenu(true);
                QAction *excsv = new QAction(tr("Esporta csv"),this);
                excsv->setIcon(icon6);
                excsv->setIconVisibleInMenu(true);

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
                pPopup->addAction(expdf);
                pPopup->addSeparator();
                pPopup->addAction(excsv);
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
                    del_rec();
                }
                else if(pItem == nw){
                    nuovo();
                }
                else if(pItem == sa)
                {
                    inserisci();
                }
                else if(pItem == md)
                {
                    save();
                }
                else if(pItem == pr)
                {
                    anteprima();
                }
                else if(pItem == expdf)
                {
                    stampa_pdf();
                }
                else if(pItem == excsv)
                {
                    esporta_cvs();
                }
}

void user::importExcel()
{
    //Procedura per importare gli alunni 
    //mediante file excel
    QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle("LyLibrary");
        box->setText("Il file excel\ndeve essere composto\n");
        box->setInformativeText("dalle sole colonne Nome, Cognome, Classe, Sezione");
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
            if(xlsx.dimension().columnCount() > 4)
            {
                QMessageBox::warning(this,"LyLibrary","Il file contiene più di 4 colonne.");
            }
            else{
                for(int i=1; i <= xlsx.dimension().rowCount(); i++)
                {
                    QProgressDialog progressDialog(this);
                    progressDialog.setRange(0, xlsx.dimension().rowCount());
                    progressDialog.setWindowModality(Qt::WindowModal);
                    progressDialog.setWindowTitle(tr("Inserimento alunni"));
                    progressDialog.setValue(i);
                    progressDialog.setLabelText(tr("Inserimento alunni %1 di %2...")
                                                    .arg(i).arg(xlsx.dimension().rowCount()));
                    qApp->processEvents();
                    
                    QSqlQuery query;
                    QString name_surname;
                    QString req = "INSERT INTO clienti(nome,cognome,cod_fisc,part_iva) VALUES('";
                    if(Cell *name = xlsx.cellAt(i,1)){
                    req.append(QString(name->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname = QString(name->value().toString().toStdString().c_str()).replace("'","''");
                    req.append("','");
                    }
                    if(Cell *lastname = xlsx.cellAt(i,2)){
                    req.append(QString(lastname->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(lastname->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *classe = xlsx.cellAt(i,3)){
                    req.append(QString(classe->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(classe->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *sezione = xlsx.cellAt(i,4)){
                    req.append(QString(sezione->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(sezione->value().toString().toStdString().c_str()).replace("'","''"));
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