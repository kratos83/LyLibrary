/*
 * Libreria Utente
 */
#include "user.h"
#include "ui_user.h"
#include <QtGui>
#include <QtGui/QMessageBox>
#include <QDebug>
#include <QtSql/QtSql>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QCompleter>
#include "itdelegato.h"
#include "print.h"
#include "pdf_export.h"

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
    ui->id->setEnabled(false);
    ui->totale_list->setReadOnly(true);
    ui->dockWidget->setFloating(false);
    ui->dockWidget->setTitleBarWidget(new QWidget);
    ui->dockWidget->setObjectName("Top");
    mod_grid = new QSqlRelationalTableModel;
    lista();
    somma();
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

    if(ui->id->text() == ""){
        QMessageBox::warning(this,"LyLibrary","Inserisci correttamente i dati");
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

             QSqlQuery Query;

             Query.prepare("UPDATE clienti SET nome = :nome, cognome = :cognome, indirizzo=:indirizzo, "
                           "telefono=:telefono, email=:email, cod_fisc=:cod_fisc, part_iva=:part_iva, fax=:fax "
                           "WHERE id= :id");

             Query.bindValue(":id",ui->id->text());
             Query.bindValue(":nome", QString::fromUtf8(ui->nome->text()));
             Query.bindValue(":cognome",QString::fromUtf8(ui->cognome->text()));
             Query.bindValue(":indirizzo",QString::fromUtf8(ui->indirizzo->text()));
             Query.bindValue(":telefono",QString::fromUtf8(ui->telefono->text()));
             Query.bindValue(":email",QString::fromUtf8(ui->email->text()));
             Query.bindValue(":cod_fisc",QString::fromUtf8(ui->cod_f->text()));
             Query.bindValue(":part_iva",QString::fromUtf8(ui->part_iva->text()));
             Query.bindValue(":fax",QString::fromUtf8(ui->fax->text()));

             if (Query.exec())
             {
                 // Aggiornamento effettuato

                 ui->id->setText("");
                 ui->nome->setText("");
                 ui->cognome->setText("");
                 ui->indirizzo->setText("");
                 ui->telefono->setText("");
                 ui->email->setText("");
                 ui->cod_f->setText("");
                 ui->part_iva->setText("");
                 ui->fax->setText("");
              }
             else
             {
                 // Errore Aggiornamento
                 // scrivere codice per per gestione dell'errore
                 qWarning() << Query.lastError();

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

void user::inserisci(){

    //Tentativo di inserimento record perché nuovo codice voce

    QSqlQuery Query;

    Query.prepare("INSERT INTO clienti (id, nome, cognome,telefono,indirizzo,email,cod_fisc,part_iva,fax )"
                  " VALUES (:id, :nome, :cognome, :telefono, :indirizzo, :email, :cod_fisc, :part_iva, :fax )");

    Query.bindValue(":id",ui->id->text());
    Query.bindValue(":nome", QString::fromUtf8(ui->nome->text()));
    Query.bindValue(":cognome",QString::fromUtf8(ui->cognome->text()));
    Query.bindValue(":indirizzo",QString::fromUtf8(ui->indirizzo->text()));
    Query.bindValue(":telefono",QString::fromUtf8(ui->telefono->text()));
    Query.bindValue(":email",QString::fromUtf8(ui->email->text()));
    Query.bindValue(":cod_fisc",QString::fromUtf8(ui->cod_f->text()));
    Query.bindValue(":part_iva",QString::fromUtf8(ui->part_iva->text()));
    Query.bindValue(":fax",QString::fromUtf8(ui->fax->text()));

    if (Query.exec()) //Se esito OK inserimento DB
    {
        ui->id->setText("");
        ui->nome->setText("");
        ui->cognome->setText("");
        ui->indirizzo->setText("");
        ui->telefono->setText("");
        ui->email->setText("");
        ui->cod_f->setText("");
        ui->part_iva->setText("");
        ui->fax->setText("");
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

void user::del_rec(){

    QString flag_controllo = "NO";
    if(!ui->g_lista->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare...");
    }
     else if (!ui->id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

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

            QSqlQuery query;

            query.prepare("DELETE FROM clienti WHERE id = :id");
            query.bindValue(":id",ui->id->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                ui->id->setText("");
                ui->nome->setText("");
                ui->cognome->setText("");
                ui->indirizzo->setText("");
                ui->telefono->setText("");
                ui->email->setText("");
            }
            else
            {
                QMessageBox MsgBox;
                MsgBox.setText(QString::fromUtf8("Errore"));
                MsgBox.setInformativeText(QString::fromUtf8("Inpossibile eliminare record poichè non hai selezionato nulla"));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
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
        mod_grid->setHeaderData(6,Qt::Horizontal,tr("Codice fiscale"));
        mod_grid->setHeaderData(7,Qt::Horizontal,tr("Partita Iva"));
        mod_grid->setHeaderData(8,Qt::Horizontal,tr("Fax"));

        ui->g_lista->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->g_lista->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        ui->g_lista->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->g_lista->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->g_lista->setSortingEnabled(true);
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
            ui->dettagli->setText("ID: "+query.value(0).toString()+"\n"
                                  "Nome: "+query.value(1).toString()+"\n"
                                  "Cognome: "+query.value(2).toString()+"\n"
                                  "Indirizzo: "+query.value(3).toString()+"\n"
                                  "Telefono: "+query.value(4).toString()+"\n"
                                  "E-mail: "+query.value(5).toString()+"\n"
                                  "Codice fiscale: "+query.value(6).toString()+"\n"
                                  "Partita iva: "+query.value(7).toString()+"\n"
                                  "Fax: "+query.value(8).toString()+"\n");
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
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

            ui->nome->setText("");
            ui->cognome->setText("");
            ui->indirizzo->setText("");
            ui->telefono->setText("");
            ui->email->setText("");
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
    QSqlQuery querys("select nome from clienti");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString();
    }
    }
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
        QSqlQuery querys1("select cognome from clienti");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
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
        MsgBox.setWindowTitle("Luxury library");
        MsgBox.setText("Avviso");
        MsgBox.setInformativeText(QString::fromUtf8("Inserisci il testo nella casella cerca"));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(ui->combo_pref->currentIndex() == 0){
    mod_grid->setFilter(QString("nome = '" +ui->cerca->text()+"'").arg(ui->cerca->text()));
    }
    else if(ui->combo_pref->currentIndex() == 1){
    mod_grid->setFilter(QString("cognome = '" +ui->cerca->text()+"'").arg(ui->cerca->text()));
    }
}

void user::anteprima(){
     //Aggiornamento query
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle("Anteprima di stampa.");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
    preview.setWindowIcon(icon);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_new(QPrinter*)));
    preview.exec();
}

void user::preview_new(QPrinter *printer){

        Q_UNUSED(printer);
        Stampe *report = new Stampe();
        report->StampaVoci1(printer,mod_grid);
}

void user::stampa_pdf(){

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
       pdf_export *ex_pdf = new pdf_export();
       ex_pdf->StampaVoci1(printer,mod_grid);
}

void user::esporta_cvs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Esporta CSv",
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
    ttext.append(exportdata);
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
        else{
                    return ui->g_lista->eventFilter(o,e);
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
                QAction* pAction1 = new QAction("Chiudi", this);
                pAction1->setIcon(icon);
                pAction1->setIconVisibleInMenu(true);
                QAction* pAction2 = new QAction("Elimina", this);
                pAction2->setIcon(icon1);
                pAction2->setIconVisibleInMenu(true);
                QAction* nw = new QAction("Nuovo",this);
                nw->setIcon(icon2);
                nw->setIconVisibleInMenu(true);
                QAction* sa = new QAction("Salva",this);
                sa->setIcon(icon3);
                sa->setIconVisibleInMenu(true);
                QAction* md = new QAction("Aggiorna",this);
                md->setIcon(icon4);
                md->setIconVisibleInMenu(true);
                QAction* pr = new QAction("Stampa", this);
                pr->setIcon(icon5);
                pr->setIconVisibleInMenu(true);
                QAction *expdf = new QAction("Esporta pdf",this);
                expdf->setIcon(icon7);
                expdf->setIconVisibleInMenu(true);
                QAction *excsv = new QAction("Esporta csv",this);
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
