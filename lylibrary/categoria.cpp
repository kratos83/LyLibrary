#include "categoria.h"
#include "ui_categoria.h"
#include <QSqlQuery>
#include <QtSql>
#include <QModelIndex>
#include <QCompleter>
#include <QStringList>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include "itdelegato.h"

using namespace std;

porto::porto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::porto)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    //Chiamata della tastiera per uscire da categorie
    ui->esci->setShortcut(tr("Ctrl+E"));
    //Chiamata della tastiera per salvare e aggiornare i dati nel db
    ui->salva->setShortcut(tr("Ctrl+S"));
    connect(ui->salva,SIGNAL(clicked()),this,SLOT(aggiorna()));
    //Chiamata della tastiera per eliminare i dati dal db
    ui->elimina->setShortcut(tr("Ctrl+D"));
    connect(ui->elimina,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(ui->nuovo,SIGNAL(clicked()),this,SLOT(nuovo()));
    //Chiamata della tastiera per inserire una nuova categoria
    ui->nuovo->setShortcut(tr("Ctrl+N"));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(ui->search,SIGNAL(textEdited(QString)),this,SLOT(cerca_libri(QString)));
    connect(ui->cerca_categ,SIGNAL(clicked()),this,SLOT(filtro()));
    mod_grid = new QSqlRelationalTableModel;
    ui->id->setEnabled(false);
    lista();
    nuovo();
}

void porto::nuovo(){
    QSqlQuery query;
    query.exec("select max((categ)+1) from categoria");
    QString id_cl;
    if(query.next()){
        if(query.value(0).toInt() == 0){
                    ui->id->setText("1");
        }
        else{
           id_cl = (query.value(0).toString());
           ui->id->setText(id_cl);
        }

    }
    ui->lineEdit->setText("");
}

void porto::cerca_libri(const QString &){

    if(ui->comboBox->currentIndex() == 0){
    QString text;
    text = ui->search->text();
    QStringList list_st;
    QSqlQuery querys("select tipo_categ from categoria");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list_st,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    ui->search->setCompleter(complete);
    if(text.length() == 0){
        lista();
    }
    }
    else if(ui->comboBox->currentIndex() == 1){
        QString text1;
        text1 = ui->search->text();
        QStringList list_st1;
        QSqlQuery querys1("select categ from categoria");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        ui->search->setCompleter(complete1);
        if(text1.length() == 0){
            lista();
        }
    }
}

void porto::filtro(){
    if(ui->search->text().length() == 0){
        lista();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle("Lylibrary");
        MsgBox.setText("Avviso");
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca").replace("'","''").toStdString().c_str()));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(ui->comboBox->currentIndex() == 0){
        mod_grid->setFilter(QString("tipo_categ = '" +ui->search->text()+"'").arg(ui->search->text()));
    }
    else if(ui->comboBox->currentIndex() == 1){
        mod_grid->setFilter(QString("categ = '" +ui->search->text()+"'").arg(ui->search->text()));
    }
}

void porto::clear_lin_text()
{
    ui->id->setText("");
    ui->lineEdit->setText("");
}

void porto::aggiorna(){

    if(ui->id->text() == ""){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati"));
    }
    else{
    QSqlQuery querys;
    if(!ui->id->text().isEmpty()){

    querys.prepare("select * from categoria where categ='"+ui->id->text()+"'");
    querys.exec();

    if(querys.next()){
        QSqlQuery query;
        query.prepare("UPDATE categoria SET tipo_categ ='"+ui->lineEdit->text()+"' "
                      " where categ='"+ui->id->text()+"'");

        if(query.exec()){
            clear_lin_text();
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile aggiornare...\n")+querys.lastError().text());
        }
    }
    else{
        salva();
    }
    }
    }
    lista();

}

void porto::salva(){

    QSqlQuery query;
    query.prepare("INSERT INTO categoria "
                  "VALUES('"+ui->id->text()+"','"+ui->lineEdit->text()+"')");

    if(query.exec()){
        clear_lin_text();
    }
    else{
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8(tr("Errore").replace("'","''").toStdString().c_str()));
        MsgBox.setInformativeText(tr("Impossibile inserire...\n")+query.lastError().text());
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }

    lista();
}

void porto::elimina(){


        if (ui->tableView->selectionModel()->currentIndex().isValid())
        {
            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM categoria WHERE categ='"+ui->id->text()+"'");
            qctrl.exec();

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QSqlQuery query;

            query.prepare("DELETE FROM categoria WHERE categ='"+ui->id->text()+"'");

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                clear_lin_text();
            }
            else
            {
                QMessageBox MsgBox;
                MsgBox.setText(QString::fromUtf8(tr("Errore").replace("'","''").toStdString().c_str()));
                MsgBox.setInformativeText(tr("Impossibile eliminare...\n")+query.lastError().text());
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }
        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText(QString::fromUtf8(tr("Errore").replace("'","''").toStdString().c_str()));
            MsgBox.setInformativeText(QString::fromUtf8(tr("Seleziona una riga per eliminarla...").replace("'","''").toStdString().c_str()));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }

       lista();

}



void porto::clickgrid(){
    QString id_new;
    int riga = ui->tableView->selectionModel()->currentIndex().row();
    QModelIndex index = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(index).toString();
    ui->id->setText(id_new);

    cerca();
}

void porto::cerca(){

    QSqlQuery query;
    query.prepare("select * from categoria where categ='"+ui->id->text()+"'");
    query.exec();

    if(query.next()){
        ui->id->setText(query.value(0).toString());
        ui->lineEdit->setText(query.value(1).toString());
    }
    else{
        clear_lin_text();
    }
}

void porto::lista(){

    mod_grid->setTable("categoria");
    mod_grid->select();
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Tipo di categoria"));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(mod_grid);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setItemDelegateForColumn(2,new coldxdelegato(this));
}
 
bool porto::eventFilter(QObject *o, QEvent *e){

    if (o==ui->tableView && e->type() == QEvent::KeyPress) {
                        QKeyEvent* ke = (QKeyEvent*)e;
                        if (ke->key() == Qt::Key_Delete) {
                            // ESEGUO IL CODICE DI ELIMINAZIONE
                            elimina();
                        }
                        if(ke->key() == Qt::Key_S){
                            //ESEGUO IL CODICE PER INSERIRE
                            salva();
                        }
                        if(ke->key() == Qt::Key_U){
                            //ESEGUO IL CODICE PER AGGIORNARE
                            aggiorna();
                        }
                        if(ke->key() == Qt::Key_N){
                            //ESEGUO IL CODICE PER UN NUOVO ARTICOLO
                            nuovo();
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

void porto::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::RightButton) {

                 this->Popup(event ->pos());
                 event->accept();
             }
}

void porto::Popup(const QPoint& pt){

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
        elimina();
    }
    else if(pItem == nw){
        nuovo();
    }
    else if(pItem == sa)
    {
        salva();
    }
    else if(pItem == md)
    {
        aggiorna();
    }
}
porto::~porto()
{
    delete ui;
}
