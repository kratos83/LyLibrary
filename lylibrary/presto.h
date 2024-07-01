#ifndef PRESTO_H
#define PRESTO_H

#include <QtGui>
#include <QDialog>
#include <QtSql/QtSql>
#include <QLocale>
#include <QCompleter>
#include "user.h"
#include "articoli.h"
#include <QtWidgets>

namespace Ui {
    class presto;
}

class presto : public QDialog
{
    Q_OBJECT

public:
    explicit presto(QWidget *parent = 0);
    ~presto();
    QString versione;
    bool eventFilter(QObject *o, QEvent *e);

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
    void mousePressEvent(QMouseEvent *);
    void Popup(const QPoint& pt);
    void importExcel();

private:
    Ui::presto *ui;
    QSqlTableModel *model;
    QSqlRelationalTableModel *mod_grid;
    QLocale *local_settings;
    QCompleter *complete;
    user *utente;
    articoli *product;

signals:
    void avvia_data();
};

#endif // PRESTO_H
