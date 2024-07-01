#ifndef ARTICOLI_H
#define ARTICOLI_H

#include <QtWidgets>
#include <QtSql/QtSql>
#include <QLocale>
#include <QtGui>
#include <QCompleter>
#include <QKeyEvent>
#include "barcode.h"
#include "ui_articoli.h"

namespace Ui {
    class articoli;
}

class articoli : public QDialog, public Ui::articoli {
    Q_OBJECT
public:
    explicit articoli(QWidget *parent = 0);
    ~articoli();
    bool eventFilter(QObject *o, QEvent *e);
    QString versione;
    barcode *codice_barre;

public slots:
    void close_art();
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
    void aggiorna(QModelIndex index);
    void agg_ass();
    void cerca_libri(const QString &);
    void filtro();
    void image_but();
    void vis_codbarre();
    void ist_codbarre();
    void view_barre(QModelIndex av);
    QString display_codbarre(QString text);
    void bar_code_en();
    void stampacodbarreart();
    void preview_printbarreart(QPrinter *);
    void mousePressEvent(QMouseEvent *);
    void Popup(const QPoint& pt);
    void reload_list();
    void esporta_pdf();
    void esporta_cvs();
    void clear_lin_text();
    void importExcel();

private:
    QSqlRelationalTableModel *mod_grid;
    QLocale *local_settings;
    QCompleter *complete;

signals:
    void article_prod();
};
#endif // ARTICOLI_H

