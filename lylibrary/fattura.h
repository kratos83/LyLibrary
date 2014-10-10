#ifndef FATTURA_H
#define FATTURA_H

#include <QWidget>
#include <QtSql>
#include <QtGui>
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
    void nuova_fattura_vendita_prodotti_digitali();
    void lista_libri();
    void lista_prod_dig();
    void stampa_fat();
    void preview_view(QPrinter *);
    void elimina_fattura();
    void elimina_riga();
    void elimina_fattura_prod_dig();
    void elimina_riga_pr_dig();
    void vis_dati_fattura_libri();
    void vis_dati_fattura_prod_dig();
    void cerca_libri();
    void cerca_prod_dig();
    void aggiorna_fattura_libri();
    void agg_fatt_libri(QModelIndex index);
    void aggiorna_fattura_prod_dig();
    void agg_fatt_prod_dig(QModelIndex index);
    void clickgrid_libri();
    void clickgrid_prod_dig();
    void filtro();
    void cerca_dxt_libri(const QString &);
    void cerca_dxt_prod_dig(const QString &);
    bool clicca_check();
    bool clicca_true();
    void disconnetti_button_libri();
    void disconnetti_button_prod_dig();
    void esporta_pdf_libri();
    void esporta_pdf_prod_dig();
    void stampa_libri();
    void stampa_prod_dig();
    void preview_libri(QPrinter *printer);
    void preview_prod_dig(QPrinter *printer);

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
