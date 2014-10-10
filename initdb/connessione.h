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
    void load_style(const QString &sheetName);

private:
    QSqlDatabase db;
    QTimer *timer;
    int prog;
    QDir *settingsDir;
    QSettings *general;

public slots:
    void creadb();
    void barra_progresso();
    void connettidatabase();
    void confread();
    bool add_vis_groupbox(bool vis);
    void messaggio();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // CONNESSIONE_H
