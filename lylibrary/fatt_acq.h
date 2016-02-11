#ifndef FATT_ACQ_H
#define FATT_ACQ_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include <QPrinter>
#include "ui_fatt_acq.h"

namespace Ui {
    class fatt_acq;
}

class fatt_acq : public QDialog, public Ui::fatt_acq
{
    Q_OBJECT

public:
    explicit fatt_acq(QWidget *parent = 0);
    ~fatt_acq();

public slots:
    void nuova_fattura_libri();
    void salva_fattura();
    void aggiorna_fattura();
    void agg_fatt(QModelIndex index);
    void cerca();
    void lista();
    void clickgrid();
    void elimina_fatt();
    void stampa_fatt();
    void elimina_riga();
    void preview_view(QPrinter *);
    void filtro();
    void cerca_dxt(const QString &);
private:
    QSqlRelationalTableModel *mod_grid;
    QSortFilterProxyModel *model;
};

#endif // FATT_ACQ_H
