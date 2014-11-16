#include <QtSql/QtSql>
#include <QMessageBox>
#include "azienda.h"
#include "ui_azienda.h"
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <QCloseEvent>
#include "itdelegato.h"
#include "mainwindow.h"

azienda::azienda(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(licenza,SIGNAL(clicked()),this,SLOT(clicca()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(esci()));
    connect(pushButton_2,SIGNAL(clicked()),this,SLOT(elimina()));

    mod_grid = new QSqlQueryModel(this);
    cerca();
}

void azienda::clicca(){


    QSqlQuery Qctrl;
    Qctrl.prepare("SELECT * FROM azienda WHERE nome_azienda = :nome_azienda LIMIT 1");
    Qctrl.bindValue(":nome_azienda",rag_soc->text());

    Qctrl.exec();

    if (Qctrl.next()) //Se esiste già
    {
        //Tentativo di aggiornamento del record perché trovato

        QSqlQuery Query;

        Query.prepare("UPDATE azienda SET partita_iva=:partita_iva, cod_fisc=:cod_fisc, indirizzo=:indirizzo, "
                      "cap=:cap, localita=:localita, prov=:prov, telefono=:telefono, fax=:fax, sito=:sito, email=:email "
                      "WHERE nome_azienda = :nome_azienda LIMIT 1");


        Query.bindValue(":nome_azienda",rag_soc->text());
        Query.bindValue(":partita_iva",p_iva->text());
        Query.bindValue(":cod_fisc",cod_f->text());
        Query.bindValue(":indirizzo",indirizzo->text());
        Query.bindValue(":cap",cap->text());
        Query.bindValue(":localita",localita->text());
        Query.bindValue(":prov",prov->text());
        Query.bindValue(":telefono",telef->text());
        Query.bindValue(":fax",fax->text());
        Query.bindValue(":sito",web->text());
        Query.bindValue(":email",email->text());

        if(Query.exec()){
            rag_soc->setText("");
            indirizzo->setText("");
            cap->setText("");
            localita->setText("");
            prov->setText("");
            telef->setText("");
            fax->setText("");
            web->setText("");
            email->setText("");
            p_iva->setText("");
            cod_f->setText("");
        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText(tr("Errore"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare"));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        }
        else
        {
         inserisci();
        }
    cerca();
}

void azienda::inserisci(){


    QSqlQuery query;
    query.prepare("INSERT INTO azienda (nome_azienda, partita_iva, cod_fisc, indirizzo, cap, localita, prov, telefono, fax, sito, email )"
                  "VALUES (:nome_azienda, :partita_iva, :cod_fisc, :indirizzo, :cap, :localita, :prov, :telefono, :fax, :sito, :email)");


        query.bindValue(":nome_azienda",rag_soc->text());
        query.bindValue(":indirizzo",indirizzo->text());
        query.bindValue(":cap",cap->text());
        query.bindValue(":localita",localita->text());
        query.bindValue(":prov",prov->text());
        query.bindValue(":telefono",telef->text());
        query.bindValue(":fax",fax->text());
        query.bindValue(":sito",web->text());
        query.bindValue(":email",email->text());
        query.bindValue(":partita_iva",p_iva->text());
        query.bindValue(":cod_fisc",cod_f->text());


        if(query.exec()){
            rag_soc->setText("");
            indirizzo->setText("");
            cap->setText("");
            localita->setText("");
            prov->setText("");
            telef->setText("");
            fax->setText("");
            web->setText("");
            email->setText("");
            p_iva->setText("");
            cod_f->setText("");
        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText(tr("Errore"));
            MsgBox.setInformativeText(tr("Impossibile inserire\n ")+query.lastError().text());
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        cerca();
}

void azienda::elimina(){

    QString flag_controllo = "NO";

        if (!rag_soc->text().isEmpty())
        {

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM azienda WHERE nome_azienda = :nome_azienda");
            qctrl.bindValue(":nome_azienda",rag_soc->text());
            qctrl.exec();

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";
             }

            if (qctrl.next()) //Se esito OK inserimento DB
            {
                flag_controllo = "SI";


                    QMessageBox MsgBox;
                    MsgBox.setText(tr("Voce non eliminabile"));
                    MsgBox.setInformativeText("E' una voce utilizzata in fornitori");
                    MsgBox.setIcon(QMessageBox::Warning);
                    MsgBox.exec();

                    return;


             }

            // Se si passano i controlli di cui sopra si procede all'eliminazione

            QSqlQuery query;

            query.prepare("DELETE FROM azienda WHERE nome_azienda = :nome_azienda");
            query.bindValue(":nome_azienda",rag_soc->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                rag_soc->setText("");
                indirizzo->setText("");
                cap->setText("");
                localita->setText("");
                prov->setText("");
                telef->setText("");
                fax->setText("");
                web->setText("");
                email->setText("");
                p_iva->setText("");
                cod_f->setText("");
            }
            else
            {
                //scrivere codice per gestione Errore cancellazione
                QMessageBox MsgBox;
                MsgBox.setText(tr("Voce non eliminabile"));
                MsgBox.setInformativeText(tr("Impossibile eliminare \n")+query.lastError().text());
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }


        }
        cerca();
}

void azienda::cerca(){

        QSqlQuery query;
        query.prepare("select * from azienda ");
        query.exec();

        if(query.next()){
            rag_soc->setText(query.value(0).toString());
            indirizzo->setText(query.value(3).toString());
            cap->setText(query.value(4).toString());
            localita->setText(query.value(5).toString());
            prov->setText(query.value(6).toString());
            telef->setText(query.value(7).toString());
            fax->setText(query.value(8).toString());
            web->setText(query.value(9).toString());
            email->setText(query.value(10).toString());
            p_iva->setText(query.value(1).toString());
            cod_f->setText(query.value(2).toString());
        }
        else{
            rag_soc->setText("");
            indirizzo->setText("");
            cap->setText("");
            localita->setText("");
            prov->setText("");
            telef->setText("");
            fax->setText("");
            web->setText("");
            email->setText("");
            p_iva->setText("");
            cod_f->setText("");
        }
}

void azienda::esci(){

    close();
}

azienda::~azienda()
{
}
