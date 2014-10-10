#ifndef PRESTITO_APP_DIG_H
#define PRESTITO_APP_DIG_H

#include <QDialog>
#include <QtGui>
#include <QtSql/QtSql>
#include <QLocale>
#include <QCompleter>
#include "user.h"
#include "prodotti_dvd.h"

namespace Ui {
class prestito_app_dig;
}

class prestito_app_dig : public QDialog
{
    Q_OBJECT
    
public:
    explicit prestito_app_dig(QWidget *parent = 0);
    ~prestito_app_dig();
    QString versione;
    
private slots:
    void delete_art();
    void cerca();
    void lista();
    void clickgrid();
    void aggiungi();
    void stampa();
    void preview_new(QPrinter*);
    void inserisci();
    void nuovo();
    void combo_iva();
    void combo_um();
    void aggiorna(QModelIndex index);
    void agg_ass();
    void cerca_libri(const QString &);
    void time_win();
    void connetti();
    void filtro();
    void reloadframe();
    void open_clienti();
    void open_articoli();

private:
    Ui::prestito_app_dig *ui;
    QSqlTableModel *model;
    QSqlRelationalTableModel *mod_grid;
    QLocale *local_settings;
    QCompleter *complete;
    user *utente;
    prodotti_dvd *dvd_pro;

signals:
    void prestitodvd();
};

#endif // PRESTITO_APP_DIG_H
