#ifndef STAMPAUTENTI_H
#define STAMPAUTENTI_H

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

class StampaUtenti : public QObject
{
    Q_OBJECT
public:
    explicit StampaUtenti(QObject *parent = nullptr);
    ~StampaUtenti();
    void stampaUtente(QPrinter *printer, QSqlQueryModel *model, 
                        QTextDocument *docu);
    void stampaUtentePag(int pagina, QTextDocument *docu, QPrinter *printer);
    
private:
    QPrinter *printer;
    QString html, htmlRow;
    QSqlDatabase m_db;
};

#endif // STAMPAUTENTI_H
