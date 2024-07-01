#ifndef STAMPASCADENZE_H
#define STAMPASCADENZE_H

#include <QtSql/QtSql>
#include <QPainter>
#include <QDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QAbstractPrintDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QLocale>
#include <QModelIndex>
#include <QTextDocument>
#include <QTextTableFormat>
#include <QTextFrameFormat>
#include <QTextCursor>
#include "../settingsmanager.h"
#include <QLoggingCategory>

class StampaScadenze : public QObject
{
    Q_OBJECT
public:
    explicit StampaScadenze(QObject *parent = nullptr);
    ~StampaScadenze();
    void stampaScadenze(QPrinter *printer, QSqlQueryModel *model, 
                        QTextDocument *docu);
    void stampaScadenzePag(int pagina, QTextDocument *docu, QPrinter *printer);
    
private:
    QPrinter *printer;
    QString html, htmlRow;
    QSqlDatabase m_db;
};

#endif // STAMPASCADENZE_H
