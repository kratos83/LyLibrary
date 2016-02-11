#ifndef SCARICO_MAG_H
#define SCARICO_MAG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include <QPrinter>
#include "ui_scarico_mag.h"

namespace Ui {
class scarico_mag;
}

class scarico_mag : public QDialog, public Ui::scarico_mag
{
    Q_OBJECT
    
public:
    explicit scarico_mag(QWidget *parent = 0);
    ~scarico_mag();
    
private:
    QSqlRelationalTableModel *mod_grid;
public slots:
    void lista_libri();
    void clickgrid();
    void cerca();
    void quan_art();
    void esporta_csv();
    void esporta_pdf();
    void elimina();
    void elimina_record_libri();
    void elimina_riga_libri();
    void stampa_carico();
    void print_carico(QPrinter *printer);
    void totale_venduto();
};

#endif // SCARICO_MAG_H
