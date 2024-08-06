#include "prest_lib.h"
#include "ui_prest_lib.h"
#include "mainwindow.h"
#include "itdelegato.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QtGui>
#include "stampa/StampaScadenze.h"

prest_lib::prest_lib(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prest_lib)
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
    connect(ui->stampa,&QPushButton::clicked,this,&prest_lib::stampaPrestito);
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

void prest_lib::clickgrid(){

    QString id_new;
    int riga = ui->tableView->selectionModel()->currentIndex().row();
    QModelIndex elemento = model->index(riga,0,QModelIndex());

    id_new = model->data(elemento).toString();
    ui->id->setText(id_new);
    cerca();
}

void prest_lib::check_list(){

    if(ui->checkBox->isChecked() == false){
        lista();
    }
    else if(ui->checkBox->isChecked() == true){
        list_comp();
    }
}

void prest_lib::lista(){

    model->setTable("prestiti");
    model->setFilter("rientro='Non rientrato' order by id");
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

void prest_lib::clicca_rientrati(){

    if(ui->list_r->isChecked() == true){
        lista_rietrati();
    }
    else{
        lista();
    }
}

void prest_lib::lista_rietrati(){

    model->setTable("prestiti");
    model->setFilter("rientro='Rientrato' order by id");
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

void prest_lib::list_comp(){

    model->setTable("prestiti");
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

void prest_lib::aggiorna(){

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Seleziona una riga da modificare..."));
    }
    else{
    QSqlQuery querys;
    QString sql = tr("Non rientrato"); QString sql1 = tr("Rientrato");
    querys.prepare("select id from prestiti where id=:id");
    querys.bindValue(":id",ui->id->text());
    querys.exec();

    if(querys.next()){
        QSqlQuery query;
        query.prepare("UPDATE prestiti SET cliente = :cliente, libro=:libro, data_prestito=:data_prestito, "
                      "data_rientro=:data_rientro, rientro=:rientro "
                      " where id=:id");
        query.bindValue(":id",ui->id->text());
        query.bindValue(":cliente",QString::fromUtf8(ui->cliente->text().replace("'","''").toStdString().c_str()));
        query.bindValue(":libro",QString::fromUtf8(ui->libro->text().replace("'","''").toStdString().c_str()));
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
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile aggiornare i prestiti...\n")+query.lastError().text());
        }//fine else
    }//fine if(query.next())
    list_comp();
    agg_riavvia_count();
    }
}

void prest_lib::cerca(){

    QSqlQuery query;
    query.prepare("select * from prestiti where id=:id");
    query.bindValue(":id",ui->id->text());
    query.exec();

    if(query.next()){
        ui->id->setText(query.value(0).toString());
        ui->cliente->setText(query.value(1).toString());
        ui->libro->setText(query.value(2).toString());
        ui->data_prestito->setDate(query.value(3).toDate());
        ui->data_rientro->setDate(query.value(4).toDate());
        if(query.value(5).toString().indexOf(tr("Non rientrato"))){
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

void prest_lib::cerca_libri(const QString &){

    if(ui->comboBox->currentIndex() == 0){
        QString text;
        text = ui->cerca_txt->text();
        QStringList list_st;
        QSqlQuery querys("select cliente from prestiti");
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
        QSqlQuery querys1("select libro from prestiti");
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

void prest_lib::filtro(){

    if(ui->cerca_txt->text().length() == 0){
        list_comp();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle(tr("Lylibrary"));
        MsgBox.setText(tr("Avviso"));
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca").replace("'","''").toStdString().c_str()));
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

void prest_lib::elimina(){
    QString flag_controllo = "NO";

    if(!ui->tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!ui->id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM prestiti WHERE id = :id");
            qctrl.bindValue(":id",ui->id->text());
            qctrl.exec();

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QSqlQuery query;

            query.prepare("DELETE FROM prestiti WHERE id = :id LIMIT 1");
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

void prest_lib::agg_riavvia_count(){
    emit riavvia();
}

void prest_lib::el_riavvia_count(){
    emit riavvia();
}

bool prest_lib::eventFilter(QObject *o, QEvent *e){

    if (o==ui->tableView && e->type() == QEvent::KeyPress) {
                QKeyEvent* ke = (QKeyEvent*)e;
                if (ke->key() == Qt::Key_Delete) {
                    // ESEGUO IL CODICE DI ELIMINAZIONE
                    elimina();
                }
                if(ke->key() == Qt::Key_L){
                    //ESEGUO IL CODICE PER VISUALIZZARE LA LISTA COMPLETA DEI PRESTITI
                    lista();
                }
                if(ke->key() == Qt::Key_U){
                    //ESEGUO IL CODICE PER AGGIORNARE
                    aggiorna();
                }
                if(ke->key() == Qt::Key_N){
                    //ESEGUO IL CODICE PER LISTA RIENTRIATI
                    lista_rietrati();
                }
                if(ke->key() == Qt::Key_M){
                    //ESEGUO IL CODICE PER VISUALIZARE LA LISTA COMPLETA
                    list_comp();
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

void prest_lib::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::RightButton) {

                 this->Popup(event ->pos());
                 event->accept();
             }
}

void prest_lib::Popup(const QPoint &pt){

    QPoint global = this ->mapToGlobal(pt);
    QMenu* pPopup = new QMenu(this);

    /**
    *  @author Angelo Scarnà
    *  Immagini per il menu a tendina
    */
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/dialog-close.png"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/edit-delete.png"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/view-refresh.png"));

    /**
    *  @author Angelo Scarnà
    *  Menu a tendina
    **/
    QAction* pAction1 = new QAction(tr("Chiudi"), this);
    pAction1->setIcon(icon);
    pAction1->setIconVisibleInMenu(true);
    QAction* pAction2 = new QAction(tr("Elimina"), this);
    pAction2->setIcon(icon1);
    pAction2->setIconVisibleInMenu(true);
    QAction* nw = new QAction(tr("Visualizza lista completa"),this);
    QAction* ls = new QAction(tr("Visualizza la lista dei prestiti"),this);
    QAction* sa = new QAction(tr("Visualizza lista libri rientrati"),this);
    QAction* md = new QAction(tr("Aggiorna"),this);
    md->setIcon(icon4);
    md->setIconVisibleInMenu(true);

    /**
    *  @author Angelo Scarnà
    *  Instanzio il menu
    **/
    pPopup->addAction(md);
    pPopup->addSeparator();
    pPopup->addAction(pAction2);
    pPopup->addSeparator();
    pPopup->addAction(nw);
    pPopup->addSeparator();
    pPopup->addAction(ls);
    pPopup->addSeparator();
    pPopup->addAction(sa);
    pPopup->addSeparator();
    pPopup->addAction(pAction1);

    /**
    *  @author Angelo Scarnà
    *  Imposto la connessione ai moduli
    ***/
    QAction* pItem = pPopup ->exec(global);

    if(pItem == pAction1)
    {
        close();
    }
    else if(pItem == pAction2)
    {
        elimina();
    }
    else if(pItem == nw){
        list_comp();
    }
    else if(pItem == sa)
    {
        lista_rietrati();
    }
    else if(pItem == md)
    {
        aggiorna();
    }
    else if(pItem == ls)
    {
        lista();
    }
}

void prest_lib::stampaPrestito()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle(tr("Anteprima di stampa."));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
    preview.setWindowIcon(icon);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(anteprima(QPrinter*)));
    preview.exec();
}

void prest_lib::anteprima(QPrinter *printer)
{
    Q_UNUSED(printer);
    QTextDocument *doc = new QTextDocument;
    StampaScadenze *m_scadenze = new StampaScadenze(this);
    m_scadenze->stampaScadenze(printer,model,doc);
}

prest_lib::~prest_lib()
{
    delete ui;
}
