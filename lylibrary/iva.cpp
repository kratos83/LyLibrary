#include "iva.h"
#include "ui_iva.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QtSql>
#include "itdelegato.h"

iva::iva(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(esci_iva,SIGNAL(clicked()),this,SLOT(close()));
    connect(list_view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickgrid()));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(ctrlkey()));
    connect(agg_iva,SIGNAL(clicked()),this,SLOT(aggiungi_iva()));
    connect(del_iva,SIGNAL(clicked()),this,SLOT(delete_iva()));
    connect(nuovo_b,SIGNAL(clicked()),this,SLOT(nuovo()));
    lineEdit->setEnabled(false);
    mod_grid = new QSqlQueryModel;
    lista();
}

iva::~iva()
{
}

void iva::nuovo(){

    QString id;

    QSqlQuery query;
    query.prepare("select count(id)+1 from aliquota");
    query.exec();
    if(query.next()){
         id = query.value(0).toString();
    }
    lineEdit->setText(id);
    lineEdit_2->setText("");
    textEdit->setText("");
}

void iva::lista(){

    mod_grid->setQuery("select * from aliquota order by id");
    mod_grid->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1, Qt::Horizontal, tr("Iva"));
    mod_grid->setHeaderData(2, Qt::Horizontal, tr("Descizione."));

    list_view->setModel(mod_grid);

    list_view->setColumnWidth(0, 60);
    list_view->setColumnWidth(1, 100);
    list_view->setColumnWidth(2, 150);

    list_view->setAlternatingRowColors(true);

     list_view->setItemDelegateForColumn(2, new coldxdelegato(this));

}

void iva::clickgrid(){
    QString id_new;
    int riga = list_view->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    lineEdit->setText(id_new);

    this->cerca();
}

void iva::aggiungi_iva(){


        /* Salva il record nel DB. Il salvataggio avviene solo se il valore del campo cd_voce
        presente nella maschera non è nullo (è un campo chiave).
        Se il record esiste già in archivio è effettuato solo l'aggiornamento altrimenti viene
        inserito un record nuovo.
        Il DB presenta come chiave primaria univoca la colonna cd_voce. L'applicativo esegue
        i controlli prima del salvataggio per non incorrere in errori SQL di inserimento chiave
        doppia.
        */
         //Si controlla se il record esiste già sul DB

    if(lineEdit->text() == ""){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati"));
    }
    else{
         QSqlQuery Qctrl;
         if(!lineEdit->text().isEmpty()){
         Qctrl.prepare("SELECT * FROM aliquota WHERE id = :id");
         Qctrl.bindValue(":id",lineEdit->text());


         Qctrl.exec();

         if (Qctrl.next()) //Se esiste già
         {
            aggiorna();
         }
         else //Se invece non esiste il record..
         {
            inserisci();
         }
         }
         else{
             inserisci();
         }
    }
    lista();

}

void iva::aggiorna(){

    //Tentativo di aggiornamento del record perché trovato

    QSqlQuery Query;

    Query.prepare("UPDATE aliquota SET descrizione = :descrizione, iva = :iva "
                  "WHERE id = :id");

    Query.bindValue(":id", lineEdit->text());
    Query.bindValue(":iva",lineEdit_2->text());
    Query.bindValue(":descrizione",textEdit->toPlainText());


    if (Query.exec())
    {
        // Aggiornamento effettuato

        lineEdit->setText("");
        lineEdit_2->setText("");
        textEdit->setText("");
     }
    else
    {
        // Errore Aggiornamento
        // scrivere codice per per gestione dell'errore
              QMessageBox box;
            box.setText(tr("Impossibile aggiornare \n")+Query.lastError().text());
            box.setIcon(QMessageBox::Information);
            box.exec();
     }
 this->lista();
}

void iva::inserisci(){
    //Tentativo di inserimento record perché nuovo codice voce

    QSqlQuery Query;

    Query.prepare("INSERT INTO aliquota (id, iva, descrizione)"
                  " VALUES (:id, :iva, :descrizione)");
    Query.bindValue(":id", lineEdit->text());
    Query.bindValue(":iva",lineEdit_2->text());
    Query.bindValue(":descrizione",textEdit->toPlainText());


    if (Query.exec()) //Se esito OK inserimento DB
    {
        lineEdit->setText("");
        lineEdit_2->setText("");
        textEdit->setText("");;
     }
    else
    {
        // Scrivere codice per errore inserimento

        QMessageBox box;
        box.setText(QString::fromUtf8(tr("Impossibile inserire").replace("'","''").toStdString().c_str()));
        box.setIcon(QMessageBox::Information);
        box.exec();
     }
  lista();
    //Fine Tentativo di inserimento
}

void iva::delete_iva(){

    QString flag_controllo = "NO";
    if(!list_view->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!lineEdit->text().isEmpty())
        {
            // Si controlla che il cd_voce non sia usato in Spese o Budget

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM aliquota WHERE id = :id");
            qctrl.bindValue(":id",lineEdit->text());
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

            query.prepare("DELETE FROM aliquota WHERE id = :id");
            query.bindValue(":id",lineEdit->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                lineEdit->setText("");
                textEdit->setText("");
                lineEdit_2->setText("");
            }
            else
            {
                //scrivere codice per gestione Errore cancellazione
                qWarning()<<query.lastError();
             }


        }

       this->lista();
}

void iva::cerca(){

    QSqlQuery query;
        query.prepare("SELECT id, iva, descrizione FROM aliquota WHERE id = :id");
        query.bindValue(":id",lineEdit->text());
        query.exec();

        if (query.next())
        {
            lineEdit_2->setText(query.value(1).toString());
            textEdit->setText(query.value(2).toString());

        }
        else
        {
            // Elemento non trovato, pulizia campi di immissione
            //  voce contabile di nuova creazione

            lineEdit_2->setText("");
            textEdit->setText("");

        }
}

void iva::ctrlkey(){

    if (lineEdit->text().length()==3)
        {
           iva::cerca();

        } else
        {
            lineEdit_2->setText("");
            textEdit->setText("");

        }
}
