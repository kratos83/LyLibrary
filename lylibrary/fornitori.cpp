#include "fornitori.h"
#include "ui_fornitori.h"
#include "itdelegato.h"
#include <ctype.h>
#include "print.h"
#include "fatt_new.h"

#include <QtSql>
#include <QMessageBox>
#include <QModelIndex>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QCompleter>

fornitori::fornitori(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Anagrafica fornitori"));
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(nuovo_b,SIGNAL(clicked()),this,SLOT(nuovo()));
    connect(elimina_b,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(salva_b,SIGNAL(clicked()),this,SLOT(salva()));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(stampa_b,SIGNAL(clicked()),this,SLOT(stampa()));

    //Inizializza la lista:
    mod_grid = new QSqlRelationalTableModel();
    //Disabilito id edit
    id->setEnabled(false);

    //Visualizza lista query:
    lista();
}

void fornitori::nuovo(){

    QSqlQuery query;
    query.prepare("SELECT max(id+1) FROM fornitori");
    query.exec();
    if(query.next()){
        if(query.value(0).toInt() == 0){
	   id->setText("1");
	}
	else{
        QString text = query.value(0).toString();
        id->setText(text);
	}
    }
    rag_soc->setText("");
    part_iva->setText("");
    cod_fisc->setText("");
    indirizzo->setText("");
    cap->setText("");
    localita->setText("");
    prov->setText("");
    telefono->setText("");
    cap->setText("");
    email->setText("");
    sito->setText("");
}

void fornitori::salva(){

    if(id->text() == ""){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati"));
    }
    else{
    QSqlQuery query;
    if(!id->text().isEmpty()){
        query.prepare("SELECT * FROM fornitori where id=:id");
        query.bindValue(":id",id->text());
        //Esegui query
        query.exec();
        if(query.next()){
            aggiorna();
        }
        else{
            inserisci();
        }
    }
    else{
        inserisci();
    }
    }

}

void fornitori::inserisci(){

    QSqlQuery query;
    query.prepare("INSERT INTO fornitori(id,ragione_sociale,part_iva,cod_fiscale,indirizzo,cap,localita,prov,telefono,fax,email,sito) "
                  "VALUES (:id,:ragione_sociale,:part_iva,:cod_fiscale,:indirizzo,:cap,:localita,:prov,:telefono,:fax,:email,:sito)");

    query.bindValue(":id",id->text());
    query.bindValue(":ragione_sociale",rag_soc->text());
    query.bindValue(":part_iva",part_iva->text());
    query.bindValue(":cod_fiscale",cod_fisc->text());
    query.bindValue(":indirizzo",indirizzo->text());
    query.bindValue(":cap",cap->text());
    query.bindValue(":localita",localita->text());
    query.bindValue(":prov",prov->text());
    query.bindValue(":telefono",telefono->text());
    query.bindValue(":fax",fax->text());
    query.bindValue(":email",email->text());
    query.bindValue(":sito",sito->text());

    if(query.exec()){
        id->setText("");
        rag_soc->setText("");
        part_iva->setText("");
        cod_fisc->setText("");
        indirizzo->setText("");
        cap->setText("");
        localita->setText("");
        prov->setText("");
        telefono->setText("");
        cap->setText("");
        email->setText("");
        sito->setText("");
    }
    else{
        //Errore in caso di inserimento
        qWarning() << query.lastError();
    }
    emit vis_forn();
    lista();
}

void fornitori::aggiorna(){

    QSqlQuery query;
    query.prepare("UPDATE fornitori SET ragione_sociale=:ragione_sociale, part_iva=:part_iva, cod_fiscale=:cod_fiscale, "
                  " indirizzo=:indirizzo, cap=:cap, localita=:localita, prov=:prov, telefono=:telefono, fax=:fax, email=:email, "
                  " sito=:sito "
                  "WHERE id=:id");

    query.bindValue(":id",id->text());
    query.bindValue(":ragione_sociale",rag_soc->text());
    query.bindValue(":part_iva",part_iva->text());
    query.bindValue(":cod_fiscale",cod_fisc->text());
    query.bindValue(":indirizzo",indirizzo->text());
    query.bindValue(":cap",cap->text());
    query.bindValue(":localita",localita->text());
    query.bindValue(":prov",prov->text());
    query.bindValue(":telefono",telefono->text());
    query.bindValue(":fax",fax->text());
    query.bindValue(":email",email->text());
    query.bindValue(":sito",sito->text());

    if(query.exec()){
        id->setText("");
        rag_soc->setText("");
        part_iva->setText("");
        cod_fisc->setText("");
        indirizzo->setText("");
        cap->setText("");
        localita->setText("");
        prov->setText("");
        telefono->setText("");
        cap->setText("");
        email->setText("");
        sito->setText("");
    }
    else{
        //Errore in caso di inserimento
        qWarning() << query.lastError();
    }
    emit vis_forn();
    lista();
}

void fornitori::elimina(){

    QString flag_controllo = "NO";

    if(!tableView->selectionModel()->currentIndex().isValid()){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da eliminare..."));
    }
     else if (!id->text().isEmpty())
        {

            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM fornitori WHERE id=id");
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

            query.prepare("DELETE FROM fornitori WHERE id=:id");
            query.bindValue(":id",id->text());

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                id->setText("");
                rag_soc->setText("");
                part_iva->setText("");
                cod_fisc->setText("");
                indirizzo->setText("");
                cap->setText("");
                localita->setText("");
                prov->setText("");
                telefono->setText("");
                cap->setText("");
                email->setText("");
                sito->setText("");
            }
            else
            {
                QMessageBox MsgBox;
                MsgBox.setText(QString::fromUtf8(tr("Errore").replace("'","''").toStdString().c_str()));
                MsgBox.setInformativeText(QString::fromUtf8(tr("Impossibile eliminare record poichè non hai selezionato nulla").replace("'","''").toStdString().c_str()));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }


        }
       emit vis_forn();
       lista();

}

void fornitori::clickgrid(){

    QString id_new;
    int riga = tableView->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    id->setText(id_new);

    this->cerca();

}

void fornitori::cerca(){

    QSqlQuery query;
    query.prepare("SELECT * FROM fornitori WHERE id=:id");
    query.bindValue(":id",id->text());
    query.exec();
    if(query.next()){
        rag_soc->setText(query.value(1).toString());
        part_iva->setText(query.value(2).toString());
        cod_fisc->setText(query.value(3).toString());
        indirizzo->setText(query.value(4).toString());
        cap->setText(query.value(5).toString());
        localita->setText(query.value(6).toString());
        prov->setText(query.value(7).toString());
        telefono->setText(query.value(8).toString());
        cap->setText(query.value(9).toString());
        email->setText(query.value(10).toString());
        sito->setText(query.value(11).toString());
    }
    else{
        id->setText("");
        rag_soc->setText("");
        part_iva->setText("");
        cod_fisc->setText("");
        indirizzo->setText("");
        cap->setText("");
        localita->setText("");
        prov->setText("");
        telefono->setText("");
        cap->setText("");
        email->setText("");
        sito->setText("");
    }

}

void fornitori::stampa(){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(anteprima(QPrinter*)));
    preview.exec();
}

void fornitori::anteprima(QPrinter *printer){

    Q_UNUSED(printer);
    Stampe *report = new Stampe();
    report->stampaforn(printer,mod_grid);
}

void fornitori::lista(){

    mod_grid->setTable("fornitori");
    mod_grid->select();
    mod_grid->setHeaderData(0,Qt::Horizontal, tr("ID"));
    mod_grid->setHeaderData(1,Qt::Horizontal, tr("Ragione sociale"));
    mod_grid->setHeaderData(2,Qt::Horizontal, tr("Partita IVA"));
    mod_grid->setHeaderData(3,Qt::Horizontal, tr("Codice Fiscale"));
    mod_grid->setHeaderData(4,Qt::Horizontal, tr("Indirizzo"));
    mod_grid->setHeaderData(5,Qt::Horizontal, tr("CAP"));
    mod_grid->setHeaderData(6,Qt::Horizontal,QString::fromUtf8((tr("Località").replace("'","''").toStdString().c_str())));
    mod_grid->setHeaderData(7,Qt::Horizontal, tr("PROV"));
    mod_grid->setHeaderData(8,Qt::Horizontal, tr("Telefono"));
    mod_grid->setHeaderData(9,Qt::Horizontal, tr("FAX"));
    mod_grid->setHeaderData(10,Qt::Horizontal, tr("E-mail"));
    mod_grid->setHeaderData(11,Qt::Horizontal, tr("Sito web"));


    tableView->setModel(mod_grid);
    tableView->setColumnWidth(0,50);
    tableView->setColumnWidth(1,150);
    tableView->setColumnWidth(2,50);
    tableView->setColumnWidth(3,50);
    tableView->setColumnWidth(4,100);
    tableView->setColumnWidth(5,50);
    tableView->setColumnWidth(6,80);
    tableView->setColumnWidth(7,50);
    tableView->setColumnWidth(8,100);
    tableView->setColumnWidth(9,100);
    tableView->setColumnWidth(10,100);
    tableView->setColumnWidth(11,150);

    tableView->setAlternatingRowColors(true);

    tableView->setItemDelegateForColumn(12,new coldxdelegato(this));

}

fornitori::~fornitori()
{
}
