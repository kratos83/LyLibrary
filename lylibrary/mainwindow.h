#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
  Definizioni delle librerie QT
  */
#include <QMdiArea>
#include <QtGui>
#include <QProcess>
#include <QMainWindow>
#include <QPluginLoader>
#include <QDir>
#include <QPluginLoader>
#include <QSystemTrayIcon>

/*
  Definizioni delle classi luxury
  */

#include "../analogclock/analogclock.h"
#include "../poppler_pdf/read_pdf.h"
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
#include "prodotti_dvd.h"
#include "prestito_app_dig.h"
#include "prestito_dig.h"
#include "cod_fisc.h"
#include "pag.h"
#include "causali.h"
#include "iva.h"
#include "azienda.h"
#include "banche.h"
#include "fatt_acq.h"
#include "fornitori.h"
#include "carico_mag.h"
#include "scarico_mag.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void load_style(const QString &sheetName);
    void enable_menu();
    void disable_menu();
    void setVisible(bool visible);
    void createActions();
    void createTrayIcon();
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
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *impo;
    QAction *quitAction;
    QAction *ag_tray;
    QComboBox *box;
    QToolButton *p_art,*toolButton,*about_tool,*toolButton_9,*doc_fatt;
    QMenu *menu,*menu_p,*menu_d,*trayMenu,*menu_doc,*menu_ana,*menu_pr,*menu_sc,*menu_plugins;
    prodotti_dvd *digitale;
    prestito_app_dig *prestoapp;
    prestito_dig *dig_app;
    cod_fisc *fisc;
    pag *pagamento;
    causali *caus;
    iva *iv;
    azienda *azUi;
    banche *banca;
    fatt_acq *fatt;
    fornitori *forn;
    carico_mag *car_mag;
    read_pdf *pdf;

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    bool loadImPlugin( QObject* );
    bool unloadImPlugin( QObject* );

private:
    QDir *pluginDir;
    QPluginLoader *loader;
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
        void interface();
        void onwid();
        void read();
        void messageClicked(QSystemTrayIcon::ActivationReason reason);
        void libri_pr();
        void reload_data();
        void core();
        void gestioneplugin();
        void prodotti_digitali();
        void prestiti_digitali();
        QString getLineFromCommandOutput( QString command );
        void agg_automatico();
        void dis_agg_automatico();
        void avvio_licenza();
        void orologio();
        void prestiti_digitali_app();
        void riavvia_data();
        void gest_pagamento();
        void gest_causali();
        void gest_codfisc();
        void gest_iva();
        void azienda_cod();
        void azienda_ok();
        void gest_banca();
        void gest_fatt_acq();
        void gest_fornitori();
        void gest_fattura_v();
        void populateMenus(QObject *plugin);
        void addToMenu(QObject *plugin, QIcon ico, const QString &name, QMenu *menu_add,
                       const char *member);

        void vis_carico_magazzino();
        void vis_scarico_magazzino();
        void installa_plugin();
        void scrivi_posizione();
        void leggi_posizione();

protected slots:
        void pluginLoad(QObject *plugin, QTreeWidgetItem *item);
        void pluginUnload(QObject *plugin, QTreeWidgetItem *item);
        void avvia_widget();
        void readPlug();
        void avvia_backup();
        void moveEvent(QMoveEvent *);
        void resizeEvent(QResizeEvent *);

Q_SIGNALS:
        void pluginLoaded(const QString&, bool);
};

extern MainWindow *form;
#endif
