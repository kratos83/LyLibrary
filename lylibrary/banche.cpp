#include "banche.h"
#include "ui_banche.h"
#include "itdelegato.h"
#include "pdf_export.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtGui>

banche::banche(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(nuovo_b,SIGNAL(clicked()),this,SLOT(nuovo()));
    connect(salva,SIGNAL(clicked()),this,SLOT(clicca()));
    connect(elimina_b,SIGNAL(clicked()),this,SLOT(elimina()));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid()));
    connect(pdf_ex,SIGNAL(clicked()),this,SLOT(esporta_pdf()));
    model = new QSqlRelationalTableModel();
    lista();
}

void banche::nuovo(){

    QSqlQuery query;
    query.exec("SELECT max(id+1) from banca");

    if(query.next()){
        if(query.value(0).toInt() == 0){
              id->setText("1");
        }
        else{
           id->setText(query.value(0).toString());
        }
    }
    iban->setText("");
    banca->setText("");
    cap->setText("");
    citta->setText("");
    indirizzo->setText("");
}

void banche::clicca(){

    if(id->text() == ""){
        QMessageBox::warning(this,tr("LyLibrary"),tr("Inserisci correttamente i dati"));
    }
    else{
    QSqlQuery querys;
    if(!iban->text().isEmpty()){

    querys.prepare("select iban from banca where id='"+id->text()+"'");
    querys.exec();

    if(querys.next()){
        QSqlQuery query;
        query.prepare("UPDATE banca SET iban='"+iban->text()+"', nome_banca = '"+banca->text()+"', "
		      "cap='"+cap->text()+"', comune='"+citta->text()+"', indirizzo='"+indirizzo->text()+"' "
                      " where id='"+id->text()+"'");

        if(query.exec()){
            clear_lin_text();
        }
        else{
            QMessageBox MsgBox;
            MsgBox.setText(tr("Errore"));
            MsgBox.setInformativeText(tr("Impossibile aggiornare.Controlla correttamente i dati."));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
        }
        emit banca_add();
    }
    else{
        inserisci();
    }
    }
    }
    lista();
    emit banca_add();

}

void banche::inserisci(){

    QSqlQuery query;
    query.prepare("INSERT INTO banca "
                  "VALUES('"+id->text()+"', '"+iban->text()+"', '"+banca->text()+"', "
		  "'"+cap->text()+"', '"+indirizzo->text()+"', '"+citta->text()+"')");

    if(query.exec()){
        clear_lin_text();
    }
    else{
        QMessageBox MsgBox;
        MsgBox.setText(tr("Errore"));
        MsgBox.setInformativeText(tr("Impossibile inserire.Controlla correttamente i dati."));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    lista();
    emit banca_add();

}

void banche::elimina(){

	QString flag_controllo = "NO";

        if (!iban->text().isEmpty())
        {
            QSqlQuery qctrl;
            qctrl.prepare("SELECT * FROM banca WHERE id='"+id->text()+"'");
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

            query.prepare("DELETE FROM banca WHERE id='"+id->text()+"'");

            if (query.exec()) //Se esito OK Eliminato da DB
            {
                clear_lin_text();
            }
            else
            {
                QMessageBox MsgBox;
                MsgBox.setText(QString::fromUtf8(tr("Errore").toStdString().c_str()));
                MsgBox.setInformativeText(QString::fromUtf8(tr("Impossibile eliminare").toStdString().c_str()));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
             }
        }

       lista();
       emit banca_add();
}

void banche::clickgrid(){

    QString id_new;
    int riga = tableView->selectionModel()->currentIndex().row();
    QModelIndex index = model->index(riga,0,QModelIndex());

    id_new = model->data(index).toString();
    id->setText(id_new);

    cerca();
}

void banche::lista(){

    model->setTable("banca");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("IBAN"));
    model->setHeaderData(2, Qt::Horizontal, tr("Banca"));
    model->setHeaderData(3, Qt::Horizontal, tr("CAP"));
    model->setHeaderData(4, Qt::Horizontal, tr("Indirizzo"));
    model->setHeaderData(5, Qt::Horizontal, tr("Citta"));

    tableView->setModel(model);
    tableView->setColumnWidth(0, 200);
    tableView->setColumnWidth(1, 200);
    tableView->setColumnWidth(2, 50);
    tableView->setColumnWidth(3, 100);
    tableView->setColumnWidth(4, 100);
    tableView->setColumnWidth(5,200);
    tableView->setAlternatingRowColors(true);
    tableView->setItemDelegateForColumn(6, new coldxdelegato(this));
}

void banche::cerca(){

    QSqlQuery query;
    query.prepare("select * from banca where id='"+id->text()+"'");
    query.exec();

    if(query.next()){
        iban->setText(query.value(1).toString());
        banca->setText(query.value(2).toString());
        cap->setText(query.value(3).toString());
        citta->setText(query.value(4).toString());
        indirizzo->setText(query.value(5).toString());
    }
    else{
        clear_lin_text();
    }
}

void banche::esporta_pdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Esporta PDF"),
                                                    "*.pdf", tr("Pdf Files(*.pdf);;Tutti i file(*.*)"));

    if (fileName.length() != 0) {
            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".pdf") < 0) {
                fileName += ".pdf";
            }
        }
       QPrinter *printer = new QPrinter(QPrinter::HighResolution);
       printer->setPageSize(QPrinter::A4);
       printer->setOrientation(QPrinter::Landscape);
       printer->setOutputFileName(fileName);
       pdf_export *ex_pdf = new pdf_export();
       ex_pdf->stampa_banca(printer,model);
}

void banche::clear_lin_text()
{
    id->setText("");
    iban->setText("");
    banca->setText("");
    cap->setText("");
    citta->setText("");
    indirizzo->setText("");
}

banche::~banche()
{
}
