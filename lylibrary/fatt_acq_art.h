#ifndef FATT_ACQ_ART_H
#define FATT_ACQ_ART_H

#include <QDialog>
#include <QtGui>
#include <QCompleter>
#include <QLocale>
#include <QStatusBar>
#include "ui_fatt_acq_art.h"
#include "fatt_new.h"
#include "fatt_acq.h"

namespace Ui {
    class fatt_acq_art;
}

class fatt_acq_art : public QDialog, public Ui::fatt_acq_art
{
    Q_OBJECT

public:
    explicit fatt_acq_art(QWidget *parent = 0);
    ~fatt_acq_art();

private:
    QCompleter *complete;
    QLocale *local_settings;
    fatt_new *fatt;
    fatt_acq *acq;
    bool found;
    QStatusBar *bar;
private slots:


public slots:
    void inserisci_riga_art();
    void inserisci();
    void aggiorna_riga();
    void complete_tot(const QString &);
    void text(const QString &);
    void sconto_tot(const QString &);
    void prezzosiva(const QString &);
    void formattanum();
    void combo_iva();
    void combo_um();
    void nuovo();
    void cerca();
    void clickgrid();
    void prezzo_senza_iva();
    void prezzo_con_iva();
    void iva_sum();
    void combo_categ();
    void insert_articoli();
    void insert_prod_dig();
    void insert_carico_libri();
    void insert_carico_prod_dig();
    void update_articoli(double qua);
    void update_prod_dig(double qad);
    void update_carico_libri( double ca, double _pr_u, double _p_siva, double _p_civa, double _tot  );
    void update_carico_prod_dig( double pro, double _pr_u, double _p_siva, double _p_civa, double _tot);
    void image_but();
    void p_iv(double dg);
    void cerca_codart(const QString &);
    void clear_testo();
signals:
    void save_fatt();
};

#endif // FATT_ACQ_ART_H
