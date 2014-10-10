#ifndef FORNITORI_H
#define FORNITORI_H

#include <QDialog>
#include <QtSql>
#include <QPrinter>
#include "ui_fornitori.h"

namespace Ui {
    class fornitori;
}

class fornitori : public QDialog, public Ui::fornitori
{
    Q_OBJECT

public:
    explicit fornitori(QWidget *parent = 0);
    ~fornitori();

private:
    QSqlRelationalTableModel *mod_grid;

private slots:
    void nuovo();
    void salva();
    void inserisci();
    void aggiorna();
    void elimina();
    void clickgrid();
    void cerca();
    void stampa();
    void lista();
    void anteprima(QPrinter *);

signals:
    void vis_forn();
};

#endif // FORNITORI_H
