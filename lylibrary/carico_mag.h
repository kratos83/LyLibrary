#ifndef CARICO_MAG_H
#define CARICO_MAG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include "ui_carico_mag.h"


namespace Ui {
class carico_mag;
}

class carico_mag : public QDialog, public Ui::carico_mag
{
    Q_OBJECT
    
public:
    explicit carico_mag(QWidget *parent = 0);
    ~carico_mag();
    
private:
    QSqlRelationalTableModel *mod_grid;
public slots:
    void lista_libri();
    void lista_prod_dig();
    void clickgrid();
    void cerca();
    void cerca_pr_dig();
    void quan_art();
    bool clicca_check();
    bool clicca_true();
    void esporta_csv();
    void esporta_pdf();
    void elimina();
    void elimina_record_libri();
    void elimina_riga_libri();
    void elimina_record_prodotti();
    void elimina_riga_prodotti();
    void stampa_carico();
    void print_carico(QPrinter *printer);
    void totale_magazzino();
};

#endif // CARICO_MAG_H
