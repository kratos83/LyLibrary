#ifndef FATTURA_H
#define FATTURA_H

#include <QWidget>
#include <QtSql>
#include <QtGui>
#include <QPrinter>
#include "ui_fattura.h"

namespace Ui {
class fattura;
}

class fattura : public QWidget, public Ui::fattura
{
    Q_OBJECT
    
public:
    explicit fattura(QWidget *parent = 0);
    ~fattura();
    
public slots:
    void nuova_fattura_vendita_libri();
    void lista_libri();
    void stampa_fat();
    void preview_view(QPrinter *);
    void elimina_fattura();
    void elimina_riga();
    void vis_dati_fattura_libri();
    void cerca_libri();
    void aggiorna_fattura_libri();
    void agg_fatt_libri(QModelIndex index);
    void clickgrid_libri();
    void filtro();
    void cerca_dxt_libri(const QString &);
    void esporta_pdf_libri();
    void stampa_libri();
    void preview_libri(QPrinter *printer);

private:
    QSqlRelationalTableModel *mod_grid;
    QSortFilterProxyModel *model;
    QMenu *menu,*menu1,*menu2,*menu3,*menu4;
    QAction * agg_fattura_libri;
    QAction * agg_fattura_prod_dig;
    QAction *mod_fat_libri;
    QAction *mod_fat_prod_dig;
    QAction *el_libri;
    QAction *el_prod_dig;
    QAction *print_libri,*print_prod_dig;
    QAction *ex_pdf_libri,*ex_pdf_prod_diag;
};

#endif // FATTURA_H
