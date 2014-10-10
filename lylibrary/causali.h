#ifndef CAUSALI_H
#define CAUSALI_H

#include <QDialog>
#include <QtSql>
#include "ui_causali.h"

namespace Ui {
    class causali;
}

class causali : public QDialog, public Ui::causali
{
    Q_OBJECT

public:
    explicit causali(QWidget *parent = 0);
    ~causali();

private:
    QSqlQueryModel *mod_grid;

private slots:
    void salva();
    void elimina();
    void aggiorna();
    void nuovo();
    void lista();
    void clickgrid();
    void cerca();
    void inserisci();
};

#endif // CAUSALI_H
