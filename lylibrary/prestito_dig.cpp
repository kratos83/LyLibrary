#include "prestito_dig.h"
#include "ui_prestito_dig.h"
#include "mainwindow.h"
#include "itdelegato.h"
#include <QtGui>
#include <QMessageBox>

prestito_dig::prestito_dig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prestito_dig)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(check_list()));
    connect(ui->aggiorna_db,SIGNAL(clicked()),this,SLOT(aggiorna()));
    connect(ui->aggiorna_db,SIGNAL(clicked(bool)),this,SLOT(agg_riavvia_count()));
    connect(ui->lib_rientro,SIGNAL(clicked()),this,SLOT(aggiorna()));
    connect(ui->list_r,SIGNAL(clicked()),this,SLOT(clicca_rientrati()));
    connect(ui->cerca_txt,SIGNAL(textEdited(QString)),this,SLOT(cerca_libri(QString)));
    connect(ui->_search,SIGNAL(clicked()),this,SLOT(filtro()));
    connect(ui->cancella,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(ui->cancella,SIGNAL(clicked(bool)),this,SLOT(el_riavvia_count()));
    ui->dockWidget->setFloating(false);
    ui->dockWidget->setTitleBarWidget(new QWidget);
    model = new QSqlRelationalTableModel(this);
    local_settings = new QLocale(QLocale::system());

    /*********************************************************
      Creazione della data
     *********************************************************/
    QDate data;
    data.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    ui->data_prestito->setCalendarPopup(true);
    ui->data_prestito->setDate(data);

    QDate data_ren;
    data_ren.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
    data_ren = data_ren.addDays(7);
    ui->data_rientro->setCalendarPopup(true);
    ui->data_rientro->setDate(data_ren);

    /*************************************************************
      Inizializzazione dati
    **************************************************************/
    lista();
}

void prestito_dig::clickgrid(){

    QString id_new;
    int riga = ui->tableView->selectionModel()->currentIndex().row();
    QModelIndex elemento = model->index(riga,0,QModelIndex());

    id_new = model->data(elemento).toString();
    ui->id->setText(id_new);
    cerca();
}

void prestito_dig::check_list(){

    if(ui->checkBox->isChecked() == false){
        lista();
    }
    else if(ui->checkBox->isChecked() == true){
        list_comp();
    }
}

void prestito_dig::lista(){

    model->setTable("prestiti_dvd");
    model->setFilter("data_rientro=Date() and rientro='Non rientrato' order by id");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Cliente"));
    model->setHeaderData(2, Qt::Horizontal, tr("Libro"));
    model->setHeaderData(3, Qt::Horizontal, tr("Data prestito"));
    model->setHeaderData(4, Qt::Horizontal, tr("Data rientro"));
    model->setHeaderData(5, Qt::Horizontal, tr("Rientro libro"));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(model);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setItemDelegateForColumn(6, new coldxdelegato(this));
}

void prestito_dig::clicca_rientrati(){

    if(ui->list_r->isChecked() == true){
        lista_rietrati();
    }
    else{
        lista();
    }
}

void prestito_dig::lista_rietrati(){

    model->setTable("prestiti_dvd");
    model->setFilter("rientro='Rientrato' order by id");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Cliente");
    model->setHeaderData(2, Qt::Horizontal, "Libro");
    model->setHeaderData(3, Qt::Horizontal, "Data prestito");
    model->setHeaderData(4, Qt::Horizontal, "Data rientro");
    model->setHeaderData(5, Qt::Horizontal, "Rientro libro");

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(model);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setItemDelegateForColumn(6, new coldxdelegato(this));

}

void prestito_dig::list_comp(){

    model->setTable("prestiti_dvd");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Cliente");
    model->setHeaderData(2, Qt::Horizontal, "Libro");
    model->setHeaderData(3, Qt::Horizontal, "Data prestito");
    model->setHeaderData(4, Qt::Horizontal, "Data rientro");
    model->setHeaderData(5, Qt::Horizontal, "Rientro libro");

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(model);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setItemDelegateForColumn(6, new coldxdelegato(this));
}

void prestito_dig::aggiorna(){

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,"LyLibrary","Seleziona una riga da modificare....");
    }
    else{
    QSqlQuery querys;
    QString sql = "Non rientrato"; QString sql1 = "Rientrato";
    querys.prepare("select id from prestiti_dvd where id=:id");
    querys.bindValue(":id",ui->id->text());
    querys.exec();

    if(querys.next()){
        QSqlQuery query;
        query.prepare("UPDATE prestiti_dvd SET cliente = :cliente, libro=:libro, data_prestito=:data_prestito, "
                      "data_rientro=:data_rientro, rientro=:rientro "
                      " where id=:id");
        query.bindValue(":id",ui->id->text());
        query.bindValue(":cliente",QString::fromUtf8(ui->cliente->text().toStdString().c_str()));
        query.bindValue(":libro",QString::fromUtf8(ui->libro->text().toStdString().c_str()));
        query.bindValue(":data_prestito",ui->data_prestito->date());
        query.bindValue(":data_rientro",ui->data_rientro->date());
        if(ui->lib_rientro->isChecked() == true){
            query.bindValue(":rientro",sql1);
        }
        else{
            query.bindValue(":rientro",sql);
        }

        if(query.exec()){
            ui->id->setText("");
            ui->cliente->setText("");
            ui->libro->setText("");
        }//fine if(query.exec())
        else{
            QMessageBox::warning(this,"LyLibrary","Impossibile aggiornare i prestiti...\n"+query.lastError().text());
        }//fine else
    }//fine if(query.next())
    list_comp();
    agg_riavvia_count();
    }
}

void prestito_dig::cerca(){

    QSqlQuery query;
    query.prepare("select * from prestiti_dvd where id=:id");
    query.bindValue(":id",ui->id->text());
    query.exec();

    if(query.next()){
        ui->id->setText(query.value(0).toString());
        ui->cliente->setText(query.value(1).toString());
        ui->libro->setText(query.value(2).toString());
        ui->data_prestito->setDate(query.value(3).toDate());
        ui->data_rientro->setDate(query.value(4).toDate());
        if(query.value(5).toString().indexOf("Non rientrato")){
            ui->lib_rientro->setChecked(true);
        }
        else{
            ui->lib_rientro->setChecked(false);
        }
    }
    else{
        ui->id->setText("");
        ui->cliente->setText("");
        ui->libro->setText("");
    }
}

void prestito_dig::cerca_libri(const QString &){

    if(ui->comboBox->currentIndex() == 0){
        QString text;
        text = ui->cerca_txt->text();
        QStringList list_st;
        QSqlQuery querys("select cliente from prestiti_dvd");
        if(querys.exec()){
        while(querys.next()){
            list_st << querys.value(0).toString();
        }
        }
        QCompleter *complete = new QCompleter(list_st,this);
        complete->setCaseSensitivity(Qt::CaseInsensitive);
        ui->cerca_txt->setCompleter(complete);
        if(text.length() == 0){
            list_comp();
        }
    }
    else if(ui->comboBox->currentIndex() == 1){
        QString text1;
        text1 = ui->cerca_txt->text();
        QStringList list_st1;
        QSqlQuery querys1("select libro from prestiti_dvd");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        ui->cerca_txt->setCompleter(complete1);
        if(text1.length() == 0){
            list_comp();
        }
    }
}

void prestito_dig::filtro(){

    if(ui->cerca_txt->text().length() == 0){
        list_comp();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle("Lylibrary");
        MsgBox.setText("Avviso");
        MsgBox.setInformativeText(QString::fromUtf8("Inserisci il testo nella casella cerca"));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(ui->comboBox->currentIndex() == 0){
        model->setFilter(QString("cliente = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
    else if(ui->comboBox->currentIndex() == 1){
        model->setFilter(QString("libro = '" +ui->cerca_txt->text()+"'").arg(ui->cerca_txt->text()));
    }
}

void prestito_dig::elimina(){
    QString flag_controllo = "NO";

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare...");
    }
     else if (!ui->id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM prestiti_dvd WHERE id = :id");
            qctrl.bindValue(":id",ui->id->text());
            qctrl.exec();

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QSqlQuery query;

            query.prepare("DELETE FROM prestiti_dvd WHERE id = :id LIMIT 1");
            query.bindValue(":id",ui->id->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                ui->id->setText("");
            }
            else
            {
                //scrivere codice per gestione Errore cancellazione
                qWarning()<<query.lastError();
             }

        }
        el_riavvia_count();
        list_comp();
}

void prestito_dig::agg_riavvia_count(){
    form->cl1->clear();
    form->trayIcon->setVisible(false);
    form->riavvia_data();
    emit riavvia();
}

void prestito_dig::el_riavvia_count(){
    form->cl1->clear();
    form->trayIcon->setVisible(false);
    form->riavvia_data();
    emit riavvia();
}

prestito_dig::~prestito_dig()
{
    delete ui;
}
