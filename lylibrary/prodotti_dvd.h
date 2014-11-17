#ifndef PRODOTTI_DVD_H
#define PRODOTTI_DVD_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QLocale>
#include <QtGui>
#include <QCompleter>
#include <QKeyEvent>
#include "ui_prodotti_dvd.h"
#include "barcode.h"

namespace Ui {
class prodotti_dvd;
}

class prodotti_dvd : public QDialog, public Ui::prodotti_dvd
{
    Q_OBJECT
    
public:
    explicit prodotti_dvd(QWidget *parent = 0);
    ~prodotti_dvd();
    QString versione;
    barcode *codice_barre;

public slots:
    void close_art();
    void delete_art();
    void cerca();
    void lista();
    void clickgrid();
    void aggiungi();
    void stampa();
    void preview_new(QPrinter*);
    void inserisci();
    void nuovo();
    void combo_iva();
    void aggiorna(QModelIndex index);
    void agg_ass();
    void cerca_libri(const QString &);
    void filtro();
    void image_but();
    void vis_codbarre();
    void ist_codbarre();
    void view_barre(QModelIndex av);
    void bar_code_en();
    void stampacodbarre();
    void preview_printbarre(QPrinter *);
    void esporta_pdf();
    void esporta_cvs();
    
private:
    QSqlRelationalTableModel *mod_grid;
    QLocale *local_settings;
    QCompleter *complete;

signals:
    void realodprodotti();
};
#endif // PRODOTTI_DVD_H
