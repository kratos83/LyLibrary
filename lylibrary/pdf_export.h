#ifndef PDF_EXPORT_H
#define PDF_EXPORT_H

#include <QtSql>
#include <QPrinter>
#include <QPainter>
#include <QDialog>
#include <QPrintDialog>
#include <QAbstractPrintDialog>
#include <QLocale>
#include <QModelIndex>

class pdf_export
{
    
public:
    pdf_export();
    void StampaVoci1(QPrinter *printer,QSqlQueryModel *dataset);
    void creaPagVoci1(int pagNo);
    void stampa_banca(QPrinter *printer,QSqlRelationalTableModel *dataset);
    void banca(int pagNo);


    QPrinter *printer;
    QPainter *painter;
    QLocale *local_settings;
    QPrintDialog *stampaDlg;
};

#endif // PDF_EXPORT_H
