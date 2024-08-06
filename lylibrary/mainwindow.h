#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
  Definizioni delle librerie QT
  */
#include <QMdiArea>
#include <QtGui>
#include <QtWidgets>
#include <QProcess>
#include <QMainWindow>
#include <QPluginLoader>
#include <QDir>
#include <QPluginLoader>
#include <QSystemTrayIcon>
#include <QTreeWidgetItem>
#include <QTreeWidget>

/*
  Definizioni delle classi luxury
  */

#include "../analogclock/analogclock.h"
#include "../log_man/license.h"
#include "about.h"
#include "esci.h"
#include "user.h"
#include "articoli.h"
#include "categoria.h"
#include "presto.h"
#include "ui_mainwindow.h"
#include "classe.h"
#include "pref.h"
#include "prest_lib.h"
#include "cod_fisc.h"
#include "azienda.h"
#include "find_cap_italian.h"
#include "verify_piva.h"
#include "verify_codicefiscale.h"
#include "sound.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMdiArea *mdiArea;
    QString versione;
    QSystemTrayIcon *trayIcon;
    classe *cl1,*cl2;
    about *aboutUi;
    esci *chiudi;
    user *userUi;
    articoli *art;
    porto *po;
    presto *sh;
    pref *sh1;
    prest_lib *lib;
    license *lic;
    AnalogClock *cl;
    QDockWidget *dock;
    QComboBox *box;
    QMenu *menu,*menu_p,*menu_d,*trayMenu,*menu_doc,*menu_ana,*menu_pr,*menu_sc,*menu_plugins,*menu_verifica;
    cod_fisc *fisc;
    azienda *azUi;
    find_cap_italian *find_cap;
    verify_piva *verifica_iva;
    verify_codicefiscale *verifica_codice;
    sound *sn;

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    bool loadImPlugin( QObject* );
    bool unloadImPlugin( QObject* );

private:
    QDir *pluginDir;
    QPluginLoader *loader;
    QAction *action;
public slots:
        void onclose();
        void onactionAbout();
        void onarticoli();
        void createStatusBar();
        void onactiondatabasetriggered();
        void onclienti();
        void titolo(QString testo);
        void categorie();
        void presta_libri();
        void settings();
        void interfaccia();
        void onwid();
        void read();
        void libri_pr();
        void reload_data();
        void core();
        void gestioneplugin();
        QString getLineFromCommandOutput( QString command );
        void agg_automatico();
        void dis_agg_automatico();
        void avvio_licenza();
        void orologio();
        void gest_codfisc();
        void azienda_cod();
        void azienda_ok();
        void populateMenus(QObject *plugin);
	void removePopulateMenus(QObject *plugin);
        void addToMenu(QObject *plugin, QIcon ico, const QString &name, QMenu *menu_add,
                       const char *member);

        void installa_plugin();
        void scrivi_posizione();
        void leggi_posizione();
        void cerca_cap_comune();
        void verifica_part_iva();
        void verifica_cod_fisc();
        void gest_update();
	void avvia_audio();
	void dona();

protected slots:
        void pluginLoad(QObject *plugin, QString m_file );
        void pluginUnload(QObject *plugin, QString m_file );
        void avvia_widget();
        void readPlug();
        void avvia_backup();
        void moveEvent(QMoveEvent *);
        void resizeEvent(QResizeEvent *);

Q_SIGNALS:
        void pluginLoaded(const QString&, bool);
};
#endif
