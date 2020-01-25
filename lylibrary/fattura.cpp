#include "fattura.h"
#include "ui_fattura.h"
#include "fattura_rg.h"
#include "print.h"
#include <QtSql>
#include <QtGui>
#include <QMenu>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QCompleter>
#include <QFileDialog>

fattura::fattura(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    id->setVisible(false);
    setAttribute(Qt::WA_DeleteOnClose);

    /*********************************************
     * Connessioni ai diversi metodi
     *********************************************/
    connect(esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(stampa,SIGNAL(clicked()),this,SLOT(stampa_fat()));
    connect(tab_view,SIGNAL(clicked(QModelIndex)),this,SLOT(clickgrid_libri()));
    connect(find_cer,SIGNAL(clicked()),this,SLOT(filtro()));
    connect(Aggiungi,SIGNAL(clicked()),this,SLOT(nuova_fattura_vendita_libri()));
    connect(modifica,SIGNAL(clicked()),this,SLOT(aggiorna_fattura_libri()));
    connect(elimina,SIGNAL(clicked()),this,SLOT(elimina_fattura()));
    connect(cerca_txt,SIGNAL(editTextChanged(QString)),this,SLOT(cerca_dxt_libri(const QString &)));
    connect(p_fatt_s,SIGNAL(clicked()),this,SLOT(stampa_libri()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(esporta_pdf_libri()));

    /**************************************************
     * Impostazione tabellare di dati sql
     ***************************************************/
    mod_grid = new QSqlRelationalTableModel(this);
    model = new QSortFilterProxyModel;
    textEdit->setReadOnly(true);

    /************************************************
     * Impostazione combBox cerca_txt
     ************************************************/
    QStringList list1;
    list1 << tr("Cerca per ID") << tr("Cerca per fornitore o cliente") << tr("Cerca per tipo fattura");
    cerca_txt->addItems(list1);
    lista_libri();
    vis_dati_fattura_libri();
}

void fattura::cerca_dxt_libri(const QString &){

    if(cerca_txt->currentIndex() == 0){
    QString text1;
    text1 = cerca1->text();
    QStringList list_st;
    QSqlQuery querys("select id from fattura_vendita");
    if(querys.exec()){
    while(querys.next()){
        list_st << querys.value(0).toString();
    }
    }
    QCompleter *complete = new QCompleter(list_st,this);
    complete->setCaseSensitivity(Qt::CaseInsensitive);
    cerca1->setCompleter(complete);
    if(text1.length() == 0){
        lista_libri();
    }
    }
    else if(cerca_txt->currentIndex() == 1){
        QString text2;
        text2 = cerca1->text();
        QStringList list_st1;
        QSqlQuery querys1("select fornitore from fattura_vendita");
        if(querys1.exec()){
        while(querys1.next()){
            list_st1 << querys1.value(0).toString();
        }
        }
        QCompleter *complete1 = new QCompleter(list_st1,this);
        complete1->setCaseSensitivity(Qt::CaseInsensitive);
        cerca1->setCompleter(complete1);
        if(text2.length() == 0){
            lista_libri();
        }
    }
    else if(cerca_txt->currentIndex() == 2){
        QString text3;
        text3 = cerca1->text();
        QStringList list_st2;
        QSqlQuery querys2("select tipo_fattura from fattura_vendita");
        if(querys2.exec()){
        while(querys2.next()){
            list_st2 << querys2.value(0).toString();
        }
        }
        QCompleter *complete2 = new QCompleter(list_st2,this);
        complete2->setCaseSensitivity(Qt::CaseInsensitive);
        cerca1->setCompleter(complete2);
        if(text3.length() == 0){
            lista_libri();
        }
    }
}

void fattura::filtro(){

    if(cerca1->text().length() == 0){
        lista_libri();
        QMessageBox MsgBox;
        MsgBox.setWindowTitle(tr("Lylibrary"));
        MsgBox.setText(tr("Avviso"));
        MsgBox.setInformativeText(QString::fromUtf8(tr("Inserisci il testo nella casella cerca").toStdString().c_str()));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
    }
    else if(cerca_txt->currentIndex() == 0){
        mod_grid->setFilter(QString("id = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(cerca_txt->currentIndex() == 1){
        mod_grid->setFilter(QString("fornitore = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
    else if(cerca_txt->currentIndex() == 2){
        mod_grid->setFilter(QString("tipo_fattura = '" +cerca1->text()+"'").arg(cerca1->text()));
    }
}

void fattura::stampa_fat(){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog ft(&printer,this);
    ft.setWindowFlags(Qt::Window);
    ft.setWindowTitle(tr("Anteprima di stampa."));
    connect(&ft,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_view(QPrinter*)));
    ft.exec();
}

void fattura::preview_view(QPrinter *printer){

    Q_UNUSED(printer);
    Stampe *ft1 = new Stampe();
    ft1->stampa_fattura(printer,mod_grid);
}

void fattura::clickgrid_libri(){
    QString id_new;
    int riga = tab_view->selectionModel()->currentIndex().row();
    QModelIndex elemento = mod_grid->index(riga,0,QModelIndex());

    id_new = mod_grid->data(elemento).toString();
    id->setText(id_new);
    cerca_libri();
}

void fattura::nuova_fattura_vendita_libri(){

    fattura_rg *fs = new fattura_rg(this);
    QSqlQuery query("select max(id)+1 from fattura_vendita where year(data)");
    query.exec();
    if(query.next()){
        if(query.value(0).toInt() == 0){
            fs->id1->setText("1");
            fs->formatta_num();

    connect(fs,SIGNAL(salvafattura()),SLOT(lista_libri()));
    fs->show();
    }
    else{
            fs->id1->setText(query.value(0).toString());
            fs->formatta_num();
            connect(fs,SIGNAL(salvafattura()),SLOT(lista_libri()));
            fs->show();
        }
    }
}

void fattura::aggiorna_fattura_libri(){
    if(tab_view->selectionModel()->currentIndex().isValid()){
    QModelIndex modelIndex = tab_view->selectionModel()->currentIndex();
    int riga = modelIndex.row();
    QModelIndex newindex = modelIndex.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    agg_fatt_libri(newindex);
    }
    else
    {
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da modificare..."));
    }
}

void fattura::agg_fatt_libri(QModelIndex index){
    if(index.row() < mod_grid->rowCount()){
            fattura_rg *ft = new fattura_rg(this);
            QSqlQuery query;
            query.prepare("select * from fatture_vendita_righe where id='"+id->text()+"'");
            query.exec();
            ft->setModal(true);
            QSqlRecord rec = mod_grid->record(index.row());
            ft->id1->setText(rec.value("id").toString());
            if(query.next()){
            ft->data_fatt->setDate(rec.value("data").toDate());
            if(query.value(3).toInt() == 0){
            ft->tipo_cliente->setCurrentIndex(query.value(3).toInt());

            ft->pers->setCurrentText(query.value(2).toString());
            }
            else if(query.value(3).toInt() == 1){
                ft->tipo_cliente->setCurrentIndex(query.value(3).toInt());
                ft->pers->setCurrentText(query.value(2).toString());
            }
            ft->sconto1->setText(query.value(4).toString());
            ft->sconto2->setText(query.value(5).toString());
            ft->sconto3->setText(query.value(6).toString());
            ft->spes_tr->setText(query.value(7).toString());
            ft->spes_in->setText(query.value(8).toString());
            ft->ann->setText(query.value(9).toString());
            ft->ordine->setText(query.value(10).toString());
            ft->pagam->setCurrentText(query.value(11).toString());
            ft->lineEdit->setText(query.value(12).toString());
            ft->rag_soc->setText(query.value(13).toString());
            ft->indirizzo->setText(query.value(14).toString());
            ft->cap->setText(query.value(15).toString());
            ft->local->setText(query.value(16).toString());
            ft->prov->setText(query.value(17).toString());
            ft->tel->setText(query.value(18).toString());
            ft->cell->setText(query.value(19).toString());
            ft->paese->setText(query.value(20).toString());
            ft->vis_banca->setCurrentText(query.value(21).toString());
            ft->iban->setText(query.value(22).toString());
            ft->lineEdit_2->setText(query.value(23).toString());
            ft->lineEdit_3->setText(query.value(24).toString());
            }
            ft->formatta_num();
            ft->tot_imp_iva_pr();
            ft->lista_libri();
            ft->exec();
        }
        vis_dati_fattura_libri();
        lista_libri();
}

void fattura::vis_dati_fattura_libri(){
    QSqlQuery query,query1;
    query.prepare("select count(id) from fattura_vendita");
    query.exec();
    if(query.next()){
        QString text = QString::fromUtf8(tr("Le fatture di vendita sono: ").toStdString().c_str()) + query.value(0).toString();
        tot_fatt->setText(text);
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile ricavare le fatture di vendita..."));
    }

    query1.prepare("select sum(totale) from fattura_vendita");
    query1.exec();
    if(query1.next()){
        QString txt = QString::fromUtf8(tr("Il totale delle fatture di vendita è: ").toStdString().c_str())+QString::fromUtf8("\u20AC")+" "+query1.value(0).toString();
        tot_eur->setText(txt);
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile ricavare le fatture di vendita..."));
    }
}

void fattura::elimina_fattura(){

            if (!id->text().isEmpty())
            {
                QSqlQuery qctrl;
                qctrl.prepare("SELECT * FROM fattura_vendita WHERE id='"+id->text()+"'");
                qctrl.exec();
		
                        QMessageBox *box= new QMessageBox(this);
                        box->setWindowTitle(tr("Lylibrary"));
                        box->setInformativeText(tr("Vuoi eliminare veramente \n il record selezionato?...."));
                        box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                        box->setDefaultButton(QMessageBox::Ok);
                        int ret = box->exec();
                        switch(ret){
                        case QMessageBox::Ok:
                             //Close
                             elimina_riga();
                             box->close();
                             break;
                        case QMessageBox::Cancel:
                            //return
                            box->close();
                            break;
                        }
            }
}

void fattura::elimina_riga(){

    QSqlQuery query,query1,query2;
    query.prepare("DELETE FROM fattura_vendita WHERE id='"+id->text()+"'");
    query1.prepare("DELETE FROM fatture_righe_vendita_art WHERE id='"+id->text()+"'");
    query2.prepare("DELETE FROM fatture_vendita_righe where id='"+id->text()+"'");

    if (query.exec() & query1.exec() & query2.exec()) //Se esito OK Eliminato da DB
    {
        id->setText("");
    }
    else
    {
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile eliminare il record ")+id->text());
    }
    lista_libri();
    vis_dati_fattura_libri();
    textEdit->setText("");
}

void fattura::lista_libri(){
    mod_grid->setTable("fattura_vendita");
    model->setSourceModel(mod_grid);
    model->sort(0,Qt::AscendingOrder);

    mod_grid->setHeaderData(0,Qt::Horizontal,tr("ID"));
    mod_grid->setHeaderData(1,Qt::Horizontal,tr("Data"));
    mod_grid->setHeaderData(2,Qt::Horizontal,tr("Fornitore"));
    mod_grid->setHeaderData(3,Qt::Horizontal,tr("Tipo fattura"));
    mod_grid->setHeaderData(4,Qt::Horizontal,tr("Totale"));

    tab_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_view->setSortingEnabled(true);
    tab_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tab_view->verticalHeader()->hide();
    tab_view->setModel(model);
    mod_grid->select();
    vis_dati_fattura_libri();
}

void fattura::cerca_libri(){
    QSqlQuery query;
    query.prepare("select * from fattura_vendita where id=:id");
    query.bindValue(":id",id->text());
    query.exec();
    if(query.next()){
        textEdit->setText(QString::fromUtf8(tr("Informazioni della fattura di vendita: ").toStdString().c_str())+"\n"
                          +tr("ID:")+query.value(0).toString()+"\n"
                          +tr("Data: ")+query.value(1).toString()+"\n"
                          +tr("Fornitore: ")+query.value(2).toString()+"\n"
                          +tr("Tipo fattura: ")+query.value(3).toString()+"\n"
                          +tr("Totale: ")+QString::fromUtf8("\u20AC")+" "+query.value(4).toString()+"\n");
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Impossibile trovare le informazioni..."));
    }
}

void fattura::stampa_libri()
{
        if(tab_view->selectionModel()->currentIndex().isValid()){
        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer);
        preview.setWindowFlags(Qt::Window);
        preview.setWindowTitle(tr("Anteprima di stampa fattura libri."));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/document-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
        preview.setWindowIcon(icon);
        connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(preview_libri(QPrinter*)));
        preview.exec();
        }
        else{
            QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una fattura da stampare..."));
        }
}

void fattura::preview_libri(QPrinter *printer)
{
    Q_UNUSED(printer);
    QModelIndex in = tab_view->selectionModel()->currentIndex();
    int riga = in.row();
    QModelIndex newindex = in.sibling(riga,0);
    tab_view->setCurrentIndex(newindex);
    tab_view->selectRow(riga);
    Stampe *pr = new Stampe();
    pr->stampa_fatt_vendita_libri(printer,mod_grid,newindex,id->text().toInt());
}

void fattura::esporta_pdf_libri()
{
    if(tab_view->selectionModel()->currentIndex().isValid()){
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
       printer->setOrientation(QPrinter::Portrait);
       printer->setOutputFileName(fileName);
       Stampe *st = new Stampe();
       st->stampa_fatt_vendita_libri(printer,mod_grid,tab_view->currentIndex(),id->text().toDouble());
    }
    else{
        QMessageBox::warning(this,tr("LyLibrary"),tr("Selezionare una riga da esportare"));
    }

}

fattura::~fattura()
{

}
