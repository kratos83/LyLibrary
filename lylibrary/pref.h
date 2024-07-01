#ifndef PREF_H
#define PREF_H
/******************************
 *Classi pref
 *************************/
#include "ui_pref.h"
#include "settingsmanager.h"

/******************************
 *Classi QT
 *************************/
#include <QBuffer>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtGui>
#include <QCheckBox>
#include <QVector>
#include <QtSql>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "Process.h"

class QPluginLoader;

using namespace std;

class pref : public QDialog, public Ui::pref
{
    Q_OBJECT
//static const QString VERSION; // Current version

public:
    explicit pref(QWidget *parent = 0);
    void readsettings();
    // Self Update
    QBuffer xmldata;
    int getRequestId;
    int filedownloaded;
    QString latest_version;
    QString versione;
    QVector<QString> filelist; // The list of files to be downloaded
    QFile *file;
    QNetworkAccessManager *manager;
    QCheckBox *box,*box1,*box2;
    QList<QCheckBox*> box3;
    QListWidgetItem *configButton,*plugins_button,*databaseButton,*updateButton,*fatturaButton;
    QTableWidgetItem *box_1,*box_2,*box_3;
    SettingsManager *settings;
    int k,y,mn;
    Process *process;
    QDir *dir;
    QSqlDatabase db;
    QString default_language;

protected:
    void changeEvent(QEvent *e);

signals:
    void load_plugins();
    void menu_mod();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void menu_pref();
    void interfaccia();
    void applica();
    void self_update();
    void self_update_parse(QNetworkReply *reply);
    void agg_en();
    void agg_dis();
    void visagg();
    bool dir_server();
    void up_dw(QString package, QString url);
    void connetti_database();
    void messaggio();
    void esci();
    void combolanguage();
    QString set_language();
};

#endif // PREF_H
