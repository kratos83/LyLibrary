#ifndef STAMPABIBLIOTECA_H
#define STAMPABIBLIOTECA_H

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

class StampaBiblioteca : public QObject
{
    Q_OBJECT
public:
    explicit StampaBiblioteca(QObject *parent = nullptr);
    ~StampaBiblioteca();
    void stampaBiblioteca(QPrinter *printer, QSqlQueryModel *model, 
                        QTextDocument *docu);
    void stampaBibliotecaPag(int pagina, QTextDocument *docu, QPrinter *printer);
    
private:
    QPrinter *printer;
    QString html, htmlRow;
    QSqlDatabase m_db;
};

#endif // STAMPABIBLIOTECA_H
