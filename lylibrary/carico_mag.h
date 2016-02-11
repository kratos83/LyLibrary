#ifndef CARICO_MAG_H
#define CARICO_MAG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include <QPrinter>
#include <QPrintPreviewDialog>
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
    void clickgrid();
    void cerca();
    void quan_art();
    void esporta_csv();
    void esporta_pdf();
    void elimina_record_libri();
    void elimina_riga_libri();
    void stampa_carico();
    void print_carico(QPrinter *printer);
    void totale_magazzino();
};

#endif // CARICO_MAG_H
