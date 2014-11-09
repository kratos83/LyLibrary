#include "pag.h"
#include "ui_pag.h"
#include "itdelegato.h"
#include <QtSql>
#include <QMessageBox>

pag::pag(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(del,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(nuovo,SIGNAL(clicked()),this,SLOT(nuovo_id()));
    connect(save,SIGNAL(clicked()),this,SLOT(clicca()));

    mod_grid = new QSqlQueryModel(this);
    lista();
}

void pag::nuovo_id(){

    codice->setText("");
    textEdit->setPlainText("");
    textEdit_2->setPlainText("");

}

void pag::clicca(){

    if(codice->text() == ""){
        QMessageBox::warning(this,"LyLibrary","Inserisci correttamente i dati");
    }
    else{
    QSqlQuery Qctrl;
    Qctrl.prepare("SELECT * FROM pagam WHERE codice = :codice");
    Qctrl.bindValue(":codice",codice->text());

    Qctrl.exec();

    if (Qctrl.next()) //Se esiste già
    {
        //Tentativo di aggiornamento del record perché trovato

        QSqlQuery Query;

        Query.prepare("UPDATE pagam SET descrizione=:descrizione, nota_fattura=:nota_fattura "
                      "WHERE codice = :codice");


        Query.bindValue(":codice",codice->text());
        Query.bindValue(":descrizione",textEdit->toPlainText());
        Query.bindValue(":nota_fattura",textEdit_2->toPlainText());

        if(Query.exec()){
            codice->setText("");
            textEdit->setPlainText("");
            textEdit_2->setPlainText("");
        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText("Errore");
            MsgBox.setInformativeText("Impossibile aggiornare");
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        }
        else
        {
         inserisci();
        }
    }
        lista();
}

void pag::clickgrid(){

    QString id_new;
    int riga = tableView->selectionModel()->currentIndex().row();
    QModelIndex index = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(index).toString();
    codice->setText(id_new);

    cerca();
}

void pag::lista(){

    mod_grid->setQuery("select * from pagam order by codice");
    mod_grid->setHeaderData(0,Qt::Horizontal, "Codice");
    mod_grid->setHeaderData(1,Qt::Horizontal, "Descrizione");
    mod_grid->setHeaderData(2,Qt::Horizontal, "Nota su Fattura");

    tableView->setModel(mod_grid);

    tableView->setColumnWidth(0,150);
    tableView->setColumnWidth(1,150);
    tableView->setColumnWidth(2,400);

    tableView->setAlternatingRowColors(true);

    tableView->setItemDelegateForColumn(3,new coldxdelegato(this));

}

void pag::cerca(){

    QSqlQuery query;
    query.prepare("select * from pagam where codice=:codice");
    query.bindValue(":codice",codice->text());
    query.exec();

    if(query.next()){
        codice->setText(query.value(0).toString());
        textEdit->setText(query.value(1).toString());
        textEdit_2->setText(query.value(2).toString());
    }
    else{
        codice->setText("");
        textEdit->setPlainText("");
        textEdit_2->setPlainText("");
    }
}

void pag::elimina(){

    QString flag_controllo = "NO";

    if(!tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,"LyLibrary","Selezionare una riga da eliminare...");
    }
     else if (!codice->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM pagam WHERE codice = :codice");
            qctrl.bindValue(":codice",codice->text());
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

            query.prepare("DELETE FROM pagam WHERE codice = :codice");
            query.bindValue(":codice",codice->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                codice->setText("");
                textEdit->setText("");
                textEdit_2->setText("");
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

}

void pag::inserisci(){

    QSqlQuery query;
    query.prepare("INSERT INTO pagam (codice,descrizione,nota_fattura)"
                  "VALUES(:codice,:descrizione,:nota_fattura)");

    query.bindValue(":codice",codice->text());
    query.bindValue(":descrizione",textEdit->toPlainText());
    query.bindValue(":nota_fattura",textEdit_2->toPlainText());


    if(query.exec()){
        codice->setText("");
        textEdit->setPlainText("");
        textEdit_2->setPlainText("");
    }
    else{
        QMessageBox MsgBox;
        MsgBox.setText("Errore");
        MsgBox.setInformativeText("Impossibile aggiornare");
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    lista();

}

pag::~pag()
{
}
