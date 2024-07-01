#include <QtSql>
#include <QPrinter>
#include <QPainter>
#include <QDialog>
#include <QPrintDialog>
#include <QAbstractPrintDialog>
#include <QLocale>
#include <QModelIndex>
#include "barcode.h"

#ifndef PRINT_H
#define PRINT_H
#define BARCODE "Barcode"

class Stampe
{
public:
    Stampe();
    void stampabarcode(QPrinter *printer,QSqlQueryModel *dataset,QModelIndex *index, EAN13 *codice);
    void stampabarcodeart(QPrinter *printer, QSqlQueryModel *dataset, QModelIndex *index, EAN13 *codice);
    void print_codice(QPrinter *printer, QImage img, QString cognome, QString nome, bool maschio, bool femmina,
                      QString com_st, QString pr_st, QString codice, QDate date);


    QPrinter *printer;
    QPainter *painter;
    QLocale *local_settings;
    QPrintDialog *stampaDlg;
    barcode *bar;
};

#endif // PRINT_H
