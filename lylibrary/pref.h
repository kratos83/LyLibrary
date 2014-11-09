#ifndef PREF_H
#define PREF_H
/******************************
 *Classi pref
 *************************/
#include "ui_pref.h"
#include "settingsmanager.h"
#include "imagescene.h"
#include "image_resize.h"

/******************************
 *Classi QT
 *************************/
#include <QBuffer>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtGui>
#include <QVector>
#include <QtSql>
#include <QTableWidget>
#include <QTableWidgetItem>


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
    QGraphicsItem * getPixmapItem();
    QListWidgetItem *configButton,*plugins_button,*databaseButton,*updateButton,*fatturaButton;
    QTableWidgetItem *box_1,*box_2,*box_3;
    SettingsManager *settings;
    int k,y,mn;
    QProcess *process;
    QDir *dir;
    QSqlDatabase db;

protected:
    void changeEvent(QEvent *e);

signals:
    void load_plugins();
    void menu_mod();

private:
    image_resize images;
    void updateDisplay();
    QGraphicsPixmapItem *imagePixmapItem;
    QGraphicsView *graphicsView;
    ImageScene *imageScene;
    QLabel *alt_larg;

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void menu_pref();
    void interface();
    void applica();
    void menu_ex();
    void self_update();
    void self_update_parse(QNetworkReply *reply);
    void agg_en();
    void agg_dis();
    void visagg();
    bool dir_server();
    void up_dw(QString package, QString url);
    void apri();
    void save_file();
    void showResizeDialog();
    void resizeSwitch(QSize size, int filter);
    void label_file();
    void seleziona_tema(const QString &sheetName);
    void connetti_database();
    void messaggio();
    void esci();
};

#endif // PREF_H
