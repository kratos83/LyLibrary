#ifndef CONNESSIONE_H
#define CONNESSIONE_H

#include <QtGui>
#include <QDialog>
#include <QProcess>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QTimer>
#include <QDir>
#include <QSettings>
#include "ui_connessione.h"

namespace Ui {
    class connessione;
}

class connessione : public QDialog, public Ui::connessione
{
    Q_OBJECT

public:
    explicit connessione(QWidget *parent = 0);
    ~connessione();

private:
    QSqlDatabase db;
    QTimer *timer;
    int prog;
    QDir *settingsDir;
    QSettings *general;

public slots:
    void creadb(QSqlDatabase db, QString db_lylibrary, int year);
    void barra_progresso();
    void crea_tabelle(const QString &sql) const;
    void connettidatabase();
    void confread();
    bool add_vis_groupbox(bool vis);
    void messaggio();
    bool isNull();
    QSqlDatabase connetti();
    void setdate(int year);
    void traduzione();

protected:
    void closeEvent(QCloseEvent *);
    QString Base64ToAscii(QString String);
    QString getLineFromCommandOutput( QString command );
};

#endif // CONNESSIONE_H
