#ifndef BANCHE_H
#define BANCHE_H

#include <QDialog>
#include <QtSql>
#include "ui_banche.h"

namespace Ui {
    class banche;
}

class banche : public QDialog, public Ui::banche
{
    Q_OBJECT

public:
    explicit banche(QWidget *parent = 0);
    ~banche();

private:
    QSqlRelationalTableModel *model;

private slots:
    void clicca();
    void inserisci();
    void lista();
    void cerca();
    void clickgrid();
    void elimina();
    void nuovo();
    void esporta_pdf();
    void clear_lin_text();
signals:
   void  banca_add();
};

#endif // BANCHE_H
