#ifndef STAMPAPrestiti_H
#define STAMPAPrestiti_H

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

class StampaPrestiti : public QObject
{
    Q_OBJECT
public:
    explicit StampaPrestiti(QObject *parent = nullptr);
    ~StampaPrestiti();
    void stampaPrestiti(QPrinter *printer, QSqlQueryModel *model, 
                        QTextDocument *docu);
    void stampaPrestitiPag(int pagina, QTextDocument *docu, QPrinter *printer);
    
private:
    QPrinter *printer;
    QString html, htmlRow;
    QSqlDatabase m_db;
};

#endif // STAMPAPrestiti_H
