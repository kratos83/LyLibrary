#ifndef AZIENDA_H
#define AZIENDA_H

#include <QDialog>
#include <QtSql>
#include "ui_azienda.h"

namespace Ui {
    class azienda;
}

class azienda : public QDialog, public Ui::azienda
{
    Q_OBJECT

public:
    azienda(QWidget *parent = 0);
    ~azienda();

private:
    QSqlQueryModel *mod_grid;

private slots:
    void clicca();
    void inserisci();
    void esci();
    void cerca();
    void elimina();
    void clear_lin_text();
};

#endif // AZIENDA_H
