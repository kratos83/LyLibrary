#ifndef FATTURA_RG_H
#define FATTURA_RG_H

#include <QDialog>
#include <QLocale>
#include <QtGui>
#include <QtSql>
#include "ui_fattura_rg.h"

namespace Ui {
class fattura_rg;
}

class fattura_rg : public QDialog, public Ui::fattura_rg
{
    Q_OBJECT
    
public:
    explicit fattura_rg(QWidget *parent = 0);
    ~fattura_rg();
    QSqlQueryModel *mod_grid;
    
public slots:
    void formatta_num();
    void cliente_add();
    void open_cliente();
    void open_fornitore();
    void combo_fornitore();
    int open_cl(int index);
    int open_fl(int index);
    int open_banca(int index);
    void filtro();
    void add_banca();
    void bank();
    void filtro_banca();
    void pagamento_fatt();
    void modifica_riga();
    void modifica_riga_one(QModelIndex index);
    void elimina_riga();
    void elimina();
    void sconto_1t(const QString &);
    void sconto_2t(const QString &);
    void sconto_3t(const QString &);
    void spesa_trasp(const QString &);
    void spesa_in(const QString &);
    void fatt_aggiorna();
    void fatt_inserisci();
    void lista_libri();
    void lista_prod_dig();
    void tot_imp_iva();
    void insert_f_libri();
    void insert_f_prod_dig();
    void update_f_libri();
    void update_f_prod_dig();
    void clickgrid();
    void fattura_riga_open();
    void chiudi();
    void eli_riga();
    void tot_imp_iva_pr();
    void tipo_pagamento();
    void visual_scadenze();

private:
    QLocale *local_settings;

signals:
    void salvafattura();
    void savefatt();
};

#endif // FATTURA_RG_H
