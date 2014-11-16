#include "causali.h"
#include "ui_causali.h"
#include "itdelegato.h"
#include <QtSql>
#include <QMessageBox>
#include <QModelIndex>

causali::causali(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //Pulsante chiusura finestra di dialogo:
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    //Pulsante nuovo tipo di causale di trasporto:
    connect(nuovo_b,SIGNAL(clicked()),this,SLOT(nuovo()));
    //Cliccando sulla tabella si cerca automaticamente:
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    //Salvataggio dati:
    connect(salva_b,SIGNAL(clicked()),this,SLOT(salva()));
    //Eliminazione dati:
    connect(elimina_b,SIGNAL(clicked()),this,SLOT(elimina()));
    //Disabilita qlineedit id:
    id->setEnabled(false);

    //Query mod_grid per riempire la tabella:
    mod_grid = new  QSqlQueryModel;

    //Visualizza i dati nella tabella:
    lista();
}

/*
 Selezione nuovo id per nuovo tipo di causale id trasporto:
 */
void causali::nuovo(){

    QSqlQuery query;
    query.prepare("select count(id)+1 from causali");

    query.exec();
    if(query.next()){
        QString id_new = query.value(0).toString();
        id->setText(id_new);
    }
    tipo->setText("");
}

void causali::salva(){

    if(!id->text().isEmpty())
    {
        QSqlQuery qctrl;
        qctrl.prepare("select * from causali where id=:id");

        qctrl.bindValue(":id",id->text());
        qctrl.exec();

        if(qctrl.next())
        {
           aggiorna();
        }
        else{
            inserisci();
        }

    }
    else
    {
       inserisci();
    }
    lista();
}

void causali::inserisci(){

    QSqlQuery query;
    query.prepare("INSERT INTO causali (id,nome_causale)"
                  "VALUES(:id,:nome_causale)");

    query.bindValue(":id",id->text());
    query.bindValue(":nome_causale",tipo->text());

    if(query.exec()){
        id->setText("");;
        tipo->setText("");
    }
    else{
        qWarning() << query.lastError();
    }
    lista();

}

void causali::aggiorna(){

    QSqlQuery query;
    query.prepare("UPDATE causali SET nome_causale=:nome_causale "
                  "where id=:id");

    query.bindValue(":id",id->text());
    query.bindValue(":nome_causale",tipo->text());

    if(query.exec()){
        id->setText("");;
        tipo->setText("");
    }
    else{
        qWarning() << query.lastError();
    }
    lista();
}

void causali::elimina(){

    QString flag_controllo = "NO";

        if (!id->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM causali WHERE id = :id");
            qctrl.bindValue(":id",id->text());
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

            query.prepare("DELETE FROM causali WHERE id=:id LIMIT 1");
            query.bindValue(":id",id->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                id->setText("");
                tipo->setText("");
            }
            else
            {
                QMessageBox MsgBox;
                MsgBox.setText(QString::fromUtf8(tr("Errore")));
                MsgBox.setInformativeText(QString::fromUtf8(tr("Inpossibile eliminare record poichè non hai selezionato nulla")));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }


        }

       this->lista();


}

void causali::clickgrid(){

    QString id_new;
    int riga = tableView->selectionModel()->currentIndex().row();
    QModelIndex index = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(index).toString();
    id->setText(id_new);

    this->cerca();
}

void causali::cerca(){

    QSqlQuery query;
    query.prepare("select * from causali where id=:id");
    query.bindValue(":id",id->text());
    query.exec();

    if(query.next()){
        id->setText(query.value(0).toString());
        tipo->setText(query.value(1).toString());
    }
    else{
        id->setText("");
        tipo->setText("");
    }
}

void causali::lista(){

    mod_grid->setQuery("SELECT * FROM causali ORDER BY id");
    mod_grid->setHeaderData(0,Qt::Horizontal,tr("ID"));
    mod_grid->setHeaderData(1,Qt::Horizontal,tr("Tipo di causale"));
    tableView->setModel(mod_grid);

    tableView->setColumnWidth(0,90);
    tableView->setColumnWidth(1,150);

    tableView->setAlternatingRowColors(true);

    tableView->setItemDelegateForColumn(2, new coldxdelegato(this));
}

causali::~causali()
{
}
