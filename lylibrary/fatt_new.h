#ifndef FATT_NEW_H
#define FATT_NEW_H

#include <QDialog>
#include <QtSql>
#include <QLocale>
#include <QCompleter>
#include <QtGui>
#include "ui_fatt_new.h"

namespace Ui {
    class fatt_new;
}

class fatt_new : public QDialog, public Ui::fatt_new
{
    Q_OBJECT

public:
    explicit fatt_new(QWidget *parent = 0);
    ~fatt_new();
    QSqlQueryModel *mod_grid;
    double cl,cn,ts;

private:
    QCompleter *complete;
    QLocale *local_settings;
    QTableWidgetItem *inserisciRiga(const char *, int alig=Qt::AlignLeft);
    QTableWidgetItem *inserisciRiga(QString, int alig=Qt::AlignLeft);

public slots:
    void fattura_riga_open();
    void formatta_num();
    void clickgrid();
    void salva_fattura_ric();
    void open_fornitore();
    void modifica_riga();
    void modifica_riga_one(QModelIndex index);
    void combo_fornitori();
    void combo_pagamenti();
    void elimina_riga();
    void eli_riga();
    void elimina();
    void sconto_1t(const QString &);
    void sconto_2t(const QString &);
    void sconto_3t(const QString &);
    void spesa_trasp(const QString &);
    void spesa_in(const QString &);
    void fatt_aggiorna();
    void fatt_inserisci();
    void lista();
    void tot_imp_iva();
    void insert_f_libri();
    void insert_f_prod_dig();
    void update_f_libri();
    void update_f_prod_dig();
    double s_iva(double psiva);
    double c_iva(double pciva);
    double iva_tot(double ivatot);
    void chiudi();
    void tot_imp_iva_rg();

signals:
    void salvafatturanew();
    void savefatt();
};

#endif // FATT_NEW_H
