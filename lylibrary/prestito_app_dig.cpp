#include <QCalendarWidget>
#include <QPrintPreviewDialog>
#include <QMessageBox>

#include "itdelegato.h"
#include "print.h"
#include "mainwindow.h"
#include "pref.h"
#include "prestito_app_dig.h"
#include "ui_prestito_app_dig.h"

prestito_app_dig::prestito_app_dig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prestito_app_dig)
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
    connect(ui->prod_dig,SIGNAL(clicked()),this,SLOT(open_articoli()));
    local_settings = new QLocale;
    mod_grid = new QSqlRelationalTableModel;
    ui->dockWidget->setFloating(false);
    ui->dockWidget->setTitleBarWidget(new QWidget);
    ui->dockWidget->setObjectName("Top");
    ui->id->setEnabled(false);


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
    clickgrid();
    combo_iva();
    combo_um();
    connetti();
}

void prestito_app_dig::connetti(){

    QDate data_ren;
    data_ren.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    data_ren = data_ren.addDays(7);
    ui->data_rientro->setCalendarPopup(true);
    ui->data_rientro->setDate(data_ren);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_win()));
    timer->start();
}

void prestito_app_dig::time_win(){

    QTime temp = QTime::currentTime();
        QString dateTime = temp.toString("hh:mm:ss");
        if(temp.second()%2==0){
          dateTime[2];
      }
        ui->time_prest->setText(""+dateTime);
        ui->time_ren->setText(""+dateTime);
}

void prestito_app_dig::nuovo(){

    QSqlQuery query;
    query.exec("select max(id+1) from biblioteca_dvd");
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

void prestito_app_dig::open_clienti(){

    utente = new user(this);
    utente->setModal(true);

    connect(utente,SIGNAL(clienti_rel()),this,SLOT(combo_iva()));
    utente->exec();
}

void prestito_app_dig::open_articoli(){
    dvd_pro = new prodotti_dvd(this);
    dvd_pro->setModal(true);

    connect(dvd_pro,SIGNAL(realodprodotti()),this,SLOT(combo_um()));
    dvd_pro->exec();
}

void prestito_app_dig::combo_iva(){

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

void prestito_app_dig::combo_um(){

        ui->libro->clear();
        QStringList list_st;
        QSqlQuery querys("select nome_prodotto from prodotti_dvd");
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

void prestito_app_dig::aggiungi(){


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
         Qctrl.prepare("SELECT * FROM biblioteca_dvd WHERE id = :id");
         Qctrl.bindValue(":id",ui->id->text());

         Qctrl.exec();

         if (Qctrl.next()) //Se esiste già
         {
             inserisci();
         }
         else
             {
              inserisci();
             }
         reloadframe();
             lista();
    }
}

void prestito_app_dig::inserisci(){

    //Tentativo di inserimento record perché nuovo codice voce


                 QString sql = "INSERT INTO biblioteca_dvd (id,cliente,dvd,data_prestito,ora_prestito,data_rientro,ora_rientro) VALUES(:id,:cliente,:dvd,:data_prestito,:ora_prestito,:data_rientro,:ora_rientro)";
                 QString sql1 = "INSERT INTO prestiti_dvd(id,cliente,dvd,data_prestito,data_rientro) VALUES(:id,:cliente,:dvd,:data_prestito,:data_rientro)";

                 QSqlQuery Query,qr;
                 Query.prepare(sql);
                 qr.prepare(sql1);

                 Query.bindValue(":id",ui->id->text());
                 Query.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().toStdString().c_str()));
                 Query.bindValue(":dvd",QString::fromUtf8(ui->libro->currentText().toStdString().c_str()));
                 Query.bindValue(":data_prestito",ui->data_prestito->date());
                 Query.bindValue(":ora_prestito",QString::fromUtf8(ui->time_prest->text().toStdString().c_str()));
                 Query.bindValue(":data_rientro",ui->data_rientro->date());
                 Query.bindValue(":ora_rientro",QString::fromUtf8(ui->time_ren->text().toStdString().c_str()));

                 qr.bindValue(":id",ui->id->text());
                 qr.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().toStdString().c_str()));
                 qr.bindValue(":dvd",QString::fromUtf8(ui->libro->currentText().toStdString().c_str()));
                 qr.bindValue(":data_prestito",ui->data_prestito->date());
                 qr.bindValue(":data_rientro",ui->data_rientro->date());

                 if (Query.exec() && qr.exec())
                 {
                     // Aggiornamento effettuato
                     ui->id->setText("");
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

void prestito_app_dig::agg_ass(){

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Seleziona una riga da modificare...."));
    }
    else{
    QModelIndex modelIndex = ui->tableView->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    ui->tableView->setCurrentIndex(newindex);
    ui->tableView->selectRow(riga);
    aggiorna(newindex);
    reloadframe();
    }
}

void prestito_app_dig::aggiorna(QModelIndex index){

    //Tentativo di aggiornamento del record perché trovato
    if(index.row() < mod_grid->rowCount()){

    QString sql2 = "UPDATE biblioteca_dvd SET  cliente=:cliente, "
            " dvd=:dvd, data_prestito=:data_prestito, "
            "ora_prestito=:ora_prestito, data_rientro=:data_rientro,"
            "ora_rientro=:ora_rientro "
            "WHERE id = :id";

    QString sql3 = "UPDATE prestiti set cliente=:cliente, "
                   "dvd=:dvd, data_prestito=:data_prestito, "
                   "data_rientro=:data_rientro "
                   "WHERE id=:id";
    QSqlQuery Query, quer;
    Query.prepare(sql2);

    Query.bindValue(":id",ui->id->text());
    Query.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().toStdString().c_str()));
    Query.bindValue(":dvd",QString::fromUtf8(ui->libro->currentText().toStdString().c_str()));
    Query.bindValue(":data_prestito",ui->data_prestito->date());
    Query.bindValue(":ora_prestito",QString::fromUtf8(ui->time_prest->text().toStdString().c_str()));
    Query.bindValue(":data_rientro",ui->data_rientro->date());
    Query.bindValue(":ora_rientro",QString::fromUtf8(ui->time_ren->text().toStdString().c_str()));

    //Execute update prestiti
    quer.prepare(sql3);
    quer.bindValue(":id",ui->id->text());
    quer.bindValue(":cliente",QString::fromUtf8(ui->cliente->currentText().toStdString().c_str()));
    quer.bindValue(":dvd",QString::fromUtf8(ui->libro->currentText().toStdString().c_str()));
    quer.bindValue(":data_prestito",ui->data_prestito->date());
    quer.bindValue(":data_rientro",ui->data_rientro->date());

    if (Query.exec())
    {
        // Aggiornamento effettuato
        ui->id->setText("");
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
    }
    }
    reloadframe();
    lista();

}

void prestito_app_dig::delete_art(){

    QString flag_controllo = "NO";
    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!ui->id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl,qt;
            qctrl.prepare("SELECT * FROM biblioteca_dvd WHERE id = :id");
            qt.prepare("SELECT * from prestiti_dvd WHERE id = :id");
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

            query.prepare("DELETE FROM biblioteca_dvd WHERE id = :id LIMIT 1");
            qr.prepare("DELETE FROM prestiti_dvd WHERE id = :id LIMIT 1");
            query.bindValue(":id",ui->id->text());
            qr.bindValue(":id",ui->id->text());

            if (query.exec() && qr.exec()) //Se esito OK Eliminato da DB
            {
                ui->id->setText("");
            }
            else
            {
                //scrivere codice per gestione Errore cancellazione
                qWarning()<<query.lastError();
             }


        }
        reloadframe();
        lista();
        combo_iva();
}

void prestito_app_dig::lista(){

    mod_grid->setTable("biblioteca_dvd");
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
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(mod_grid);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setItemDelegateForColumn(7, new coldxdelegato(this));
}

void prestito_app_dig::cerca(){

    QSqlQuery query;
    query.prepare("SELECT * FROM biblioteca_dvd where id=:id");
        query.bindValue(":id",ui->id->text());
        query.exec();


        if (query.next())
        {
            ui->dettagli->setText(tr("ID: ")+query.value(0).toString()+"\n"+
                                  tr("Cliente: ")+query.value(1).toString()+"\n"+
                                  tr("Libro: ")+query.value(2).toString()+"\n"+
                                  tr("Data prestito: ")+query.value(3).toString()+"\n"+
                                  tr("Ora prestito: ")+query.value(4).toString()+"\n"+
                                  tr("Data rientro: ")+query.value(5).toString()+"\n"+
                                  tr("Ora rientro: ")+query.value(6).toString());
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

void prestito_app_dig::clickgrid(){

    QString id_new;
    int riga = ui->tableView->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    ui->id->setText(id_new);
    cerca();
}

void prestito_app_dig::cerca_libri(const QString &){

    if(ui->comboBox->currentIndex() == 0){
    QString text;
    text = ui->cerca_txt->text();
    QStringList list_st;
    QSqlQuery querys("select cliente from biblioteca_dvd");
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
        QSqlQuery querys1("select dvd from biblioteca_dvd");
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

void prestito_app_dig::filtro(){
    if(ui->cerca_txt->text().length() == 0){
        lista();
    }
    else if(ui->comboBox->currentIndex() == 0){
    mod_grid->setFilter(QString("cliente = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
    else if(ui->comboBox->currentIndex() == 1){
        mod_grid->setFilter(QString("dvd = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
}

void prestito_app_dig::reloadframe(){
    form->cl1->clear();
    form->trayIcon->setVisible(false);
    form->reload_data();
    emit prestitodvd();
}

void prestito_app_dig::stampa(){

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

void prestito_app_dig::preview_new(QPrinter *printer){

    Q_UNUSED(printer);
    Stampe *pr = new Stampe();
    pr->stampa_prodotti_digitali(printer,mod_grid);
}
prestito_app_dig::~prestito_app_dig()
{
    delete ui;
}
