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
    connect(licenza,SIGNAL(clicked()),this,SLOT(clicca()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(esci()));
    connect(pushButton_2,SIGNAL(clicked()),this,SLOT(elimina()));

    mod_grid = new QSqlQueryModel(this);
    cerca();
}

void azienda::clear_lin_text()
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


void azienda::clicca(){

    QSqlQuery Qctrl;
    Qctrl.prepare("SELECT * FROM azienda WHERE id=1");
    Qctrl.exec();

    if (Qctrl.next()) //Se esiste già
    {
        //Tentativo di aggiornamento del record perché trovato
        QSqlQuery Query;
        Query.prepare("UPDATE azienda SET nome_azienda ='"+rag_soc->text()+"', partita_iva='"+p_iva->text()+"', cod_fisc='"+cod_f->text()+"', "
		      " indirizzo='"+indirizzo->text()+"', cap='"+cap->text()+"', localita='"+localita->text()+"', "
		      " prov='"+prov->text()+"', telefono='"+telef->text()+"', fax='"+fax->text()+"', "
		      " sito='"+web->text()+"', email='"+email->text()+"' "
                      "WHERE  id=1");

        if(Query.exec()){
            clear_lin_text();
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
    query.prepare("INSERT INTO azienda "
                  "VALUES ('1','"+rag_soc->text()+"', '"+p_iva->text()+"', '"+cod_f->text()+"',"
			  "'"+indirizzo->text()+"', '"+cap->text()+"', '"+localita->text()+"', "
			  "'"+prov->text()+"', '"+telef->text()+"', '"+fax->text()+"', "
			  "'"+web->text()+"', '"+email->text()+"')");

        if(query.exec()){
            clear_lin_text();
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
            qctrl.prepare("SELECT * FROM azienda WHERE id=1");
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
            QSqlQuery query;

            query.prepare("DELETE FROM azienda WHERE id=1");
            if (query.exec()) //Se esito OK Eliminato da DB
            {
                clear_lin_text();
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
        query.prepare("select * from azienda where id=1");
        query.exec();

        if(query.next()){
            rag_soc->setText(query.value(1).toString());
            indirizzo->setText(query.value(4).toString());
            cap->setText(query.value(5).toString());
            localita->setText(query.value(6).toString());
            prov->setText(query.value(7).toString());
            telef->setText(query.value(8).toString());
            fax->setText(query.value(9).toString());
            web->setText(query.value(10).toString());
            email->setText(query.value(11).toString());
            p_iva->setText(query.value(2).toString());
            cod_f->setText(query.value(3).toString());
        }
        else{
            clear_lin_text();
        }
}

void azienda::esci(){

    close();
}

azienda::~azienda()
{
}
