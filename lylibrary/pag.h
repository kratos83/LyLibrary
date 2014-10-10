#ifndef PAG_H
#define PAG_H

#include <QDialog>
#include <QtSql>
#include "ui_pag.h"

namespace Ui {
    class pag;
}

class pag : public QDialog, public Ui::pag
{
    Q_OBJECT

public:
    explicit pag(QWidget *parent = 0);
    ~pag();

private slots:
    void clicca();
    void inserisci();
    void nuovo_id();
    void lista();
    void cerca();
    void clickgrid();
    void elimina();

private:
    QSqlQueryModel *mod_grid;

signals:
    void paga_acq();
};

#endif // PAG_H
