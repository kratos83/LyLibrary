#include "presto.h"
#include "ui_presto.h"
#include "itdelegato.h"
#include "stampa/StampaPrestiti.h"
#include "mainwindow.h"
#include "pref.h"
#include <QCalendarWidget>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include "../xlsx/xlsxdocument.h"
#include "../xlsx/xlsxformat.h"
using namespace QXlsx;

presto::presto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::presto)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->nuovo,SIGNAL(clicked()),this,SLOT(nuovo()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(ui->salva,SIGNAL(clicked()),this,SLOT(aggiungi()));
    connect(ui->modifica,SIGNAL(clicked()),this,SLOT(agg_ass()));
    connect(ui->elimina,SIGNAL(clicked()),this,SLOT(delete_art()));
    connect(ui->stampa,SIGNAL(clicked()),this,SLOT(stampa()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(filtro()));
    connect(ui->cerca_txt,SIGNAL(textEdited(QString)),this,SLOT(cerca_libri(QString)));
    connect(ui->client_op,SIGNAL(clicked()),this,SLOT(open_clienti()));
    connect(ui->art_lib,SIGNAL(clicked()),this,SLOT(open_articoli()));
    connect(ui->import_excel,&QPushButton::clicked,this,&presto::importExcel);
    local_settings = new QLocale;
    mod_grid = new QSqlRelationalTableModel;
    ui->dockWidget->setFloating(false);
    ui->dockWidget->setTitleBarWidget(new QWidget);
    ui->dockWidget->setObjectName("Top");
    ui->id->setEnabled(false);
    ui->modifica->setEnabled(false);

    /*********************************************************
      Creazione del data e tel tempo
     *********************************************************/
    QDate data;
    data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    ui->data_prestito->setCalendarPopup(true);
    ui->data_prestito->setDate(data);


    /*************************************************************
      Inizializzazione dati
    **************************************************************/
    lista();
    combo_iva();
    combo_um();
    connetti();
    nuovo();
}

void presto::connetti(){

    QDate data_ren;
    data_ren.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    data_ren = data_ren.addDays(7);
    ui->data_rientro->setCalendarPopup(true);
    ui->data_rientro->setDate(data_ren);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_win()));
    timer->start();
}

void presto::time_win(){

    QTime temp = QTime::currentTime();
    QString dateTime = temp.toString("hh:mm:ss");
    if(temp.second()%2==0){
        (void)dateTime[2]; //FIXME
    }
    ui->time_prest->setText(""+dateTime);
    ui->time_ren->setText(""+dateTime);
}

void presto::nuovo(){

    QSqlQuery query;
    query.exec("select max(id+1) from biblioteca");

    if(query.next()){
        if(query.value(0).toInt() == 0){
                    ui->id->setText("1");
        }
        else{
           ui->id->setText(query.value(0).toString());
        }
    }
    ui->cliente->clear();
    ui->libro->clear();
    combo_iva();
    combo_um();
}

void presto::open_clienti(){

    utente = new user(this);
    utente->setModal(true);

    connect(utente,SIGNAL(clienti_rel()),this,SLOT(combo_iva()));
    utente->exec();
}

void presto::open_articoli(){
    product = new articoli(this);
    product->setModal(true);

    connect(product,SIGNAL(article_prod()),this,SLOT(combo_um()));
    product->exec();
}

void presto::combo_iva(){

        ui->cliente->clear();
        QStringList list_st;
        QSqlQuery querys("select nome,cognome,telefono from clienti");
        if(querys.exec()){
        while(querys.next()){
            list_st << querys.value(0).toString() +" "+querys.value(1).toString()+" "+querys.value(2).toString();
        }
        QCompleter *complete = new QCompleter(list_st,this);
        complete->setCaseSensitivity(Qt::CaseInsensitive);
        complete->setCompletionMode(QCompleter::PopupCompletion);
        complete->setModel(ui->cliente->model());
        ui->cliente->setCompleter(complete);
        ui->cliente->addItems(list_st);
        }
}

void presto::combo_um(){

        ui->libro->clear();
        QStringList list_st;
        QSqlQuery querys("select nome_articolo from articoli");
        if(querys.exec()){
        while(querys.next()){
            list_st << querys.value(0).toString();
        }
        QCompleter *complete = new QCompleter(list_st,this);
        complete->setCaseSensitivity(Qt::CaseInsensitive);
        complete->setCompletionMode(QCompleter::PopupCompletion);
        complete->setModel(ui->libro->model());
        ui->libro->setCompleter(complete);
        ui->libro->addItems(list_st);
        }
}

void presto::aggiungi(){

        /* Salva il record nel DB. Il salvataggio avviene solo se il valore del campo cd_voce
        presente nella maschera non è nullo (è un campo chiave).
        Se il record esiste già in archivio è effettuato solo l'aggiornamento altrimenti viene
        inserito un record nuovo.
        Il DB presenta come chiave primaria univoca la colonna cd_voce. L'applicativo esegue
        i controlli prima del salvataggio per non incorrere in errori SQL di inserimento chiave
        doppia.
        */

    if(ui->id->text() == ""){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati"));
    }
    else{
         //Si controlla se il record esiste già sul DB
         QSqlQuery Qctrl;
         Qctrl.prepare("SELECT * FROM biblioteca WHERE id = :id");
         Qctrl.bindValue(":id",ui->id->text());

         Qctrl.exec();

         if (Qctrl.next()) //Se esiste già
         {
            QMessageBox::StandardButton messaggio;
            messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler inserire "+ui->cliente->currentText()+"?",QMessageBox::Yes|QMessageBox::No);
            if(messaggio == QMessageBox::Yes)
            {
                inserisci();
            }
         }
         else
             {
                QMessageBox::StandardButton messaggio;
                messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler inserire "+ui->cliente->currentText()+"?",QMessageBox::Yes|QMessageBox::No);
                if(messaggio == QMessageBox::Yes)
                {
                    inserisci();
                }
             }
         reloadframe();
             lista();
    }
}

void presto::inserisci(){

    //Tentativo di inserimento record perché nuovo codice voce


                 QString sql = "INSERT INTO biblioteca (id,cliente,libro,data_prestito,ora_prestito,data_rientro,ora_rientro) VALUES(:id,:cliente,:libro,:data_prestito,:ora_prestito,:data_rientro,:ora_rientro)";
                 QString sql1 = "INSERT INTO prestiti(id,cliente,libro,data_prestito,data_rientro) VALUES(:id,:cliente,:libro,:data_prestito,:data_rientro)";

                 QSqlQuery Query,qr;
                 Query.prepare(sql);
                 qr.prepare(sql1);

                 Query.bindValue(":id",ui->id->text());
                 Query.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().replace("'","''").toStdString().c_str()));
                 Query.bindValue(":libro",QString::fromUtf8(ui->libro->currentText().replace("'","''").toStdString().c_str()));
                 Query.bindValue(":data_prestito",ui->data_prestito->date());
                 Query.bindValue(":ora_prestito",QString::fromUtf8(ui->time_prest->text().replace("'","''").toStdString().c_str()));
                 Query.bindValue(":data_rientro",ui->data_rientro->date());
                 Query.bindValue(":ora_rientro",QString::fromUtf8(ui->time_ren->text().replace("'","''").toStdString().c_str()));

                 qr.bindValue(":id",ui->id->text());
                 qr.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().replace("'","''").toStdString().c_str()));
                 qr.bindValue(":libro",QString::fromUtf8(ui->libro->currentText().replace("'","''").toStdString().c_str()));
                 qr.bindValue(":data_prestito",ui->data_prestito->date());
                 qr.bindValue(":data_rientro",ui->data_rientro->date());



                 if (Query.exec() && qr.exec())
                 {
                     // Aggiornamento effettuato
                     ui->id->setText("");
                     ui->salva->setEnabled(true);
                     ui->modifica->setEnabled(false);
                 }
                 else
                 {
                     // Errore Aggiornamento
                     // scrivere codice per per gestione dell'errore

                         QMessageBox MsgBox;
                         MsgBox.setText(tr("Impossibile inserire"));
                         MsgBox.setInformativeText(tr("Impossibile inserire ")+Query.lastError().text());
                         MsgBox.setIcon(QMessageBox::Warning);
                         MsgBox.exec();
                 }
                 reloadframe();
                 lista();

}

void presto::agg_ass(){

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Seleziona una riga da modificare...."));
    }
    else{
        QMessageBox::StandardButton messaggio;
        messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler aggiornare "+ui->cliente->currentText()+"?",QMessageBox::Yes|QMessageBox::No);
        if(messaggio == QMessageBox::Yes)
        {
            QModelIndex modelIndex = ui->tableView->selectionModel()->currentIndex();
            int riga = modelIndex.row();
            QModelIndex newindex = modelIndex.sibling(riga,0);
            ui->tableView->setCurrentIndex(newindex);
            ui->tableView->selectRow(riga);
            aggiorna(newindex);
            reloadframe();
        }
    }
}

void presto::aggiorna(QModelIndex index){

    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){

    QString sql2 = "UPDATE biblioteca SET  cliente=:cliente, "
            " libro=:libro, data_prestito=:data_prestito, "
            "ora_prestito=:ora_prestito, data_rientro=:data_rientro,"
            "ora_rientro=:ora_rientro "
            "WHERE id = :id";

    QString sql3 = "UPDATE prestiti set cliente=:cliente, "
                   "libro=:libro, data_prestito=:data_prestito, "
                   "data_rientro=:data_rientro "
                   "WHERE id=:id";
    QSqlQuery Query, quer;
    Query.prepare(sql2);

    Query.bindValue(":id",ui->id->text());
    Query.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().replace("'","''").toStdString().c_str()));
    Query.bindValue(":libro",QString::fromUtf8(ui->libro->currentText().replace("'","''").toStdString().c_str()));
    Query.bindValue(":data_prestito",ui->data_prestito->date());
    Query.bindValue(":ora_prestito",QString::fromUtf8(ui->time_prest->text().replace("'","''").toStdString().c_str()));
    Query.bindValue(":data_rientro",ui->data_rientro->date());
    Query.bindValue(":ora_rientro",QString::fromUtf8(ui->time_ren->text().replace("'","''").toStdString().c_str()));

    //Execute update prestiti
    quer.prepare(sql3);
    quer.bindValue(":id",ui->id->text());
    quer.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().replace("'","''").toStdString().c_str()));
    quer.bindValue(":libro",QString::fromUtf8(ui->libro->currentText().replace("'","''").toStdString().c_str()));
    quer.bindValue(":data_prestito",ui->data_prestito->date());
    quer.bindValue(":data_rientro",ui->data_rientro->date());

    if (Query.exec())
    {
        // Aggiornamento effettuato
        ui->id->setText("");
        ui->salva->setEnabled(true);
        ui->modifica->setEnabled(false);
    }
    else
    {
        // Errore Aggiornamento
        // scrivere codice per per gestione dell'errore

            QMessageBox MsgBox;
            MsgBox.setText(tr("Voce non aggiornabile"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare")+Query.lastError().text());
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
            qWarning() << Query.lastError();

    }
    }
    reloadframe();
    lista();

}

void presto::delete_art(){

    QString flag_controllo = "NO";

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else  if (!ui->id->text().isEmpty())
        {
            QMessageBox::StandardButton messaggio;
            messaggio = QMessageBox::warning(this,"LyLibrary","Sei sicuro di voler eliminare "+ui->cliente->currentText()+"?",QMessageBox::Yes|QMessageBox::No);
            if(messaggio == QMessageBox::Yes)
            {

                QSqlQuery qctrl,qt;
                qctrl.prepare("SELECT * FROM biblioteca WHERE id = :id");
                qt.prepare("SELECT * from prestiti WHERE id = :id");
                qctrl.bindValue(":id",ui->id->text());
                qt.bindValue(":id",ui->id->text());
                qctrl.exec();
                qt.exec();

                if (qctrl.next() && qt.next()) //Se esito OK inserimento DB
                {
                    flag_controllo = "SI";
                }

                // Se si passano i controlli di cui sopra si procede all'eliminazione

                QSqlQuery query,qr;

                query.prepare("DELETE FROM biblioteca WHERE id = :id LIMIT 1");
                qr.prepare("DELETE FROM prestiti WHERE id = :id LIMIT 1");
                query.bindValue(":id",ui->id->text());
                qr.bindValue(":id",ui->id->text());

                if (query.exec() && qr.exec()) //Se esito OK Eliminato da DB
                {
                    ui->id->setText("");
                    ui->salva->setEnabled(true);
                    ui->modifica->setEnabled(false);
                }
                else
                {
                    //scrivere codice per gestione Errore cancellazione
                    qWarning()<<query.lastError();
                }
            }
        }
        reloadframe();
        lista();
        combo_iva();
        nuovo();
}

void presto::lista(){

    mod_grid->setTable("biblioteca");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Cliente."));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Libro."));
    mod_grid->setHeaderData(3, Qt::Horizontal, tr("Data prestito"));
    mod_grid->setHeaderData(4,Qt::Horizontal,  tr("Ora prestito"));
    mod_grid->setHeaderData(5,Qt::Horizontal,  tr("Data rientro"));
    mod_grid->setHeaderData(6,Qt::Horizontal,  tr("Ora rientro"));


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(mod_grid);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setItemDelegateForColumn(7, new coldxdelegato(this));
}

void presto::cerca(){

    QSqlQuery query;
    query.prepare("SELECT * FROM biblioteca where id=:id");
        query.bindValue(":id",ui->id->text());
        query.exec();


        if (query.next())
        {
            ui->dettagli->setText(tr("ID: ")+query.value(0).toString()+"\n"+
                                  tr("Cliente: ")+query.value(1).toString()+"\n"
                                  +tr("Libro: ")+query.value(2).toString()+"\n"
                                  +tr("Data prestito: ")+query.value(3).toString()+"\n"
                                  +tr("Ora prestito: ")+query.value(4).toString()+"\n"
                                  +tr("Data rientro: ")+query.value(5).toString()+"\n"
                                  +tr("Ora rientro: ")+query.value(6).toString());
            ui->cliente->setCurrentText(query.value(1).toString());
            ui->libro->setCurrentText(query.value(2).toString());
            ui->data_prestito->setDate(query.value(3).toDate());
            ui->time_prest->setText(query.value(4).toString());
            ui->data_rientro->setDate(query.value(5).toDate());
            ui->time_ren->setText(query.value(6).toString());
        }
        else
        {
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

            ui->cliente->clear();
            ui->libro->clear();
        }
}

void presto::clickgrid(){

    QString id_new;
    int riga = ui->tableView->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    ui->id->setText(id_new);
    cerca();
    ui->salva->setEnabled(false);
    ui->modifica->setEnabled(true);
}

void presto::cerca_libri(const QString &){

    if(ui->comboBox->currentIndex() == 0){
    QString text;
    text = ui->cerca_txt->text();
    QStringList list_st;
    QSqlQuery querys("select cliente from biblioteca");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list_st,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    ui->cerca_txt->setCompleter(complete);
    if(text.length() == 0){
        lista();
    }
    }
    else if(ui->comboBox->currentIndex() == 1){
        QString text1;
        text1 = ui->cerca_txt->text();
        QStringList list_st1;
        QSqlQuery querys1("select cliente from biblioteca");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        ui->cerca_txt->setCompleter(complete1);
        if(text1.length() == 0){
            lista();
        }
    }

}

void presto::filtro(){
    if(ui->cerca_txt->text().length() == 0){
        lista();
    }
    else if(ui->comboBox->currentIndex() == 0){
    mod_grid->setFilter(QString("cliente = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
    else if(ui->comboBox->currentIndex() == 1){
        mod_grid->setFilter(QString("libro = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
}

void presto::reloadframe(){
    emit avvia_data();
}

void presto::stampa(){

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

void presto::preview_new(QPrinter *printer){

    Q_UNUSED(printer);
    QTextDocument *doc = new QTextDocument;
    StampaPrestiti *pr = new StampaPrestiti(this);
    pr->stampaPrestiti(printer,mod_grid,doc);
}

bool presto::eventFilter(QObject *o, QEvent *e){

    if (o==ui->tableView && e->type() == QEvent::KeyPress) {
                QKeyEvent* ke = (QKeyEvent*)e;
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
                ke->accept();
                return true;
            }

    if( o==ui->tableView && e->type() == QEvent::ContextMenu)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (e);
                this ->Popup(mouseEvent->pos());
                return false;
    }

    return QDialog::eventFilter(o,e);
}

void presto::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::RightButton) {

                 this->Popup(event ->pos());
                 event->accept();
             }
}

void presto::Popup(const QPoint &pt){

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

void presto::importExcel()
{
    //Procedura per importare le scadenze
    QMessageBox *box= new QMessageBox(this);
        box->setWindowTitle("LyLibrary");
        box->setText("Il file excel\ndeve essere composto\n");
        box->setInformativeText("dalle sole colonne alunno, titolo del libro, data prestito, ora prestito, data rientro, ora rientro.\n"
                                "La data deve essere composta nel seguente formato \n"
                                "anno,mese,giorno --- attenzione: nel file excel per le date si deve usare la virgola;"
                                "\nl'orario deve essere composto nel seguente modo\n "
                                "hh:mm:ss.000 --- ATTENZIONE: nel file excel l'ora dopo i secondi si deve aggiungere .000");
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
            if(xlsx.dimension().columnCount() > 7)
            {
                QMessageBox::warning(this,"LyLibrary","Il file contiene più di 6 colonne.");
            }
            else{
                for(int i=1; i <= xlsx.dimension().rowCount(); i++)
                {
                    QProgressDialog progressDialog(this);
                    progressDialog.setRange(0, xlsx.dimension().rowCount());
                    progressDialog.setWindowModality(Qt::WindowModal);
                    progressDialog.setWindowTitle(tr("Inserimento scadenze"));
                    progressDialog.setValue(i);
                    progressDialog.setLabelText(tr("Inserimento scadenza %1 di %2...")
                                                    .arg(i).arg(xlsx.dimension().rowCount()));
                    qApp->processEvents();
                    
                    QSqlQuery query;
                    QString name_surname;
                    QString req = "INSERT INTO biblioteca(cliente,libro,data_prestito,ora_prestito,data_rientro,ora_rientro) VALUES('";
                    if(Cell *m_alunno = xlsx.cellAt(i,1)){//cliente
                    req.append(QString(m_alunno->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname = QString(m_alunno->value().toString().toStdString().c_str()).replace("'","''");
                    req.append("','");
                    }
                    if(Cell *nome_articolo = xlsx.cellAt(i,2)){//libro
                    req.append(QString(nome_articolo->value().toString().toStdString().c_str()).replace("'","''"));
                    name_surname += QString(QString(nome_articolo->value().toString().toStdString().c_str()).replace("'","''"));
                    req.append("','");
                    }
                    if(Cell *m_dataprestito = xlsx.cellAt(i,3)){//data_prestito
                    req.append(QString(m_dataprestito->value().toDate().toString("yyyy-MM-dd")));
                    name_surname += QString(QString(m_dataprestito->value().toDate().toString("yyyy-MM-dd")));
                    req.append("','");
                    }
                    if(Cell *m_oraprestito = xlsx.cellAt(i,4)){//ora_prestito
                    req.append(QString(m_oraprestito->value().toTime().toString("hh:mm:ss")));
                    name_surname += QString(m_oraprestito->value().toTime().toString("hh:mm:ss"));
                    req.append("','");
                    }
                    if(Cell *m_datarientro = xlsx.cellAt(i,5)){//data_rientro
                    req.append(QString(m_datarientro->value().toDate().toString("yyyy-MM-dd")));
                    name_surname += QString(QString(m_datarientro->value().toDate().toString("yyyy-MM-dd")));
                    req.append("','");
                    }
                    if(Cell *m_orarientro = xlsx.cellAt(i,6)){//ora_rientro
                    req.append(QString(m_orarientro->value().toTime().toString("hh:mm:ss")));
                    name_surname += QString(m_orarientro->value().toTime().toString("hh:mm:ss"));
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

presto::~presto()
{
    delete ui;
}
