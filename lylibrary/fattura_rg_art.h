#ifndef FATTURA_RG_ART_H
#define FATTURA_RG_ART_H

#include <QDialog>
#include <QtGui>
#include <QCompleter>
#include <QLocale>
#include <QStatusBar>
#include "fattura_rg.h"
#include "ui_fattura_rg_art.h"

namespace Ui {
class fattura_rg_art;
}

class fattura_rg_art : public QDialog, public Ui::fattura_rg_art
{
    Q_OBJECT
    
public:
    explicit fattura_rg_art(QWidget *parent = 0);
    ~fattura_rg_art();
    
private:
    QCompleter *complete;
    QLocale *local_settings;
    fattura_rg *fatt;
    bool found;

public slots:
    void clear_Testo();
    void image_but();
    void nuovo();
    void formattanum();
    void comboiva();
    void comboum();
    void combo_categ();
    void cerca_codart(const QString &);
    void complete_tot(const QString &);
    void text(const QString &);
    void sconto_tot(const QString &);
    void prezzosiva(const QString &);
    void p_iv(double dg);
    void cerca();
    void clickgrid();
    void prezzo_senza_iva();
    void prezzo_con_iva();
    void iva_sum();
    void inserisci_riga_art();
    void inserisci();
    void insert_scarico_libri();
    void insert_scarico_prod_dig();
    void aggiorna_riga();
    void update_articoli(double qua);
    void update_prod_dig(double qad);
    void update_carico_libri( double ca, double _pr_u, double _p_siva, double _p_civa, double _tot  );
    void update_carico_prod_dig( double pro, double _pr_u, double _p_siva, double _p_civa, double _tot);
    void update_scarico_libri( double ca, double _pr_u, double _p_siva, double _p_civa, double _tot  );
    void update_scarico_prod_dig( double pro, double _pr_u, double _p_siva, double _p_civa, double _tot);

signals:
    void save_fatt();
};

extern fattura_rg_art *rg_art;
#endif // FATTURA_RG_ART_H
