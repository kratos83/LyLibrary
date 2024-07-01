#ifndef IVA_H
#define IVA_H

#include <QDialog>
#include <QtSql>
#include "ui_iva.h"

namespace Ui {
    class iva;
}

class iva : public QDialog, public Ui::iva {
    Q_OBJECT
public:
    iva(QWidget *parent = 0);
    ~iva();

private slots:
    void aggiungi_iva();
    void cerca();
    void delete_iva();
    void lista();
    void ctrlkey();
    void clickgrid();
    void nuovo();
    void inserisci();
    void aggiorna();

private:
    QSqlQueryModel *mod_grid;

};

#endif // IVA_H

