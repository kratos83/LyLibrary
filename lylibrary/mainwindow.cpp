
/************************************
  Classi QT
 ************************************/
#include <QDateTime>
#include <QCloseEvent>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMdiArea>
#include <QtSql/QtSql>
#include <QToolTip>
#include <QMessageBox>
#include <QDate>
#include <QSettings>
#include <QtWebKit/QtWebKit>
#include <QDesktopWidget>

/*******************************
  Classi c++
 ******************************/
#include <fstream>
#include <iostream>

/*********************************
  Classi Lylibrary
 ********************************/
#include "topheader.h"
#include "classe.h"
#include "pref.h"
#include "prest_lib.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "user.h"
#include "articoli.h"
#include "esci.h"
#include "presto.h"
#include "settingsmanager.h"
#include "pluginmanager.h"
#include "luxuryim.h"
#include "../initdb/connessione.h"
#include "fattura.h"


MainWindow *form = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    if(form)
        delete form;
    form = this;

    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QString st = "";
    titolo(st);
    interface();
    read();
    core();
    orologio();
    createStatusBar();
    readPlug();
    load_style(settingsManager->generalValue("Tema/sel_tema",QVariant()).toString());
}
void MainWindow::createActions(){

    minimizeAction = new QAction(tr("Mi&nimiza"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ssimiza"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Ripristina"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Esci"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(onclose()));

    ag_tray = new QAction(tr("Aggiorna"),this);

    impo = new QAction(tr("Impostazioni"),this);
    connect(impo,SIGNAL(triggered()),this,SLOT(settings()));
}

void MainWindow::createTrayIcon(){
    trayMenu = new QMenu(this);
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(maximizeAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(impo);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/icons32x32.png")), QIcon::Normal, QIcon::Off);

    trayIcon = new QSystemTrayIcon(icon,this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

}

void MainWindow::messageClicked(QSystemTrayIcon::ActivationReason reason){

    switch(reason){
    case QSystemTrayIcon::Trigger:
        hide();
        break;
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    default:
        ;
    }
}

void MainWindow::setVisible(bool visible){
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::interface(){
    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->dumpObjectTree();
    setCentralWidget(mdiArea);

    cl1 = new classe(this);
    cl2 = new classe(this);
    dock = new QDockWidget(this);
    dock->setObjectName("Top");
    dock->setFloating(false);
    dock->setTitleBarWidget(new QWidget);
    dockWidget->setObjectName("Top");
    dockWidget->setFloating(false);
    dockWidget->setTitleBarWidget(new QWidget);
    topHeader *topw = new topHeader;
    dock->setWidget(topw);
    addDockWidget(Qt::TopDockWidgetArea, dock);

    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/application-exit.png"));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/images/accessories-dictionary.png"));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/images/svn_status.png"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/documentation.png"));
    toolButton_9 = new QToolButton(topw);
    toolButton_9->setObjectName(QString::fromUtf8("toolButton_9"));
    toolButton_9->setProperty("kind","toolbar");
    QIcon icon14;
    icon14.addFile(QString::fromUtf8(":/images/preferences-system.png"));
    toolButton_9->setIcon(icon14);
    toolButton_9->setText(tr("Strumenti"));
    toolButton_9->setToolTip(tr("Strumenti di amministrazione \n di Lylibrary"));
    toolButton_9->setIconSize(QSize(80, 80));
    toolButton_9->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_9->move(650,30);
    toolButton_9->show();
    p_art = new QToolButton(topw);
    p_art->setObjectName(QString::fromUtf8("p_art"));
    p_art->setText(tr("Anagrafica"));
    p_art->setToolTip(tr("Anagrafica di Lylibrary"));
    p_art->move(430,30);;
    p_art->setIcon(icon3);
    p_art->setIconSize(QSize(80, 80));
    p_art->setProperty("kind","toolbar");
    p_art->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    p_art->show();
    toolButton = new QToolButton(topw);
    toolButton->setObjectName(QString::fromUtf8("toolButton"));
    toolButton->move(870,30);
    toolButton->setText(tr("Esci"));
    toolButton->setToolTip(tr("Esci da Lylibrary"));
    toolButton->setIcon(icon1);
    toolButton->setIconSize(QSize(80, 80));
    toolButton->setProperty("kind","toolbar");
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton->show();
    doc_fatt = new QToolButton(topw);
    doc_fatt->setObjectName(QString::fromUtf8("doc_fatt"));
    doc_fatt->setText("Documenti");
    doc_fatt->setToolTip("Visualizza documenti \n da utilizzare");
    doc_fatt->setIcon(icon4);
    doc_fatt->setIconSize(QSize(80,80));
    doc_fatt->setProperty("kind","toolbar");
    doc_fatt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    doc_fatt->move(540,30);
    doc_fatt->show();
    about_tool = new QToolButton(topw);
    about_tool->setObjectName(QString::fromUtf8("about_tool"));
    about_tool->move(760,30);
    about_tool->setText(tr("Informazioni"));
    about_tool->setToolTip(tr("Informazioni"));
    about_tool->setIcon(icon6);
    about_tool->setProperty("kind","toolbar");
    about_tool->setIconSize(QSize(80, 80));
    about_tool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    about_tool->show();
    //Menu toolbar about
    menu = new QMenu(this);
    menu->addAction(actionAbout_2);
    menu->addSeparator();
    menu->addAction(actionAbout_Qt);
    about_tool->setMenu(menu);
    about_tool->setPopupMode(QToolButton::InstantPopup);

    //Menu toolbar anagrafica
    menu_p = new QMenu(this);
    menu_ana = new QMenu("Gestione",this);
    menu_pr = new QMenu("Prestiti",this);
    menu_sc = new QMenu("Scadenze",this);
    menu_p->addMenu(menu_ana);
    menu_p->addMenu(menu_pr);
    menu_p->addMenu(menu_sc);
    menu_ana->addAction(action_articoli);
    menu_ana->addSeparator();
    menu_ana->addSeparator();
    menu_ana->addAction(actionAnagrafica_prodotti_digitali);
    menu_ana->addSeparator();
    menu_ana->addAction(clienti);
    menu_ana->addSeparator();
    menu_ana->addAction(actionCaterorie_libri);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionCausali_di_trasporto);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionAnagrafica_pagamenti);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionAnagrafica_iva);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionAnagrafica_azienda);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionAnagrafica_banche);
    menu_ana->addSeparator();
    menu_ana->addAction(this->actionAnagraficafornitori);
    menu_sc->addAction(actionLibri_prestati);
    menu_pr->addSeparator();
    menu_pr->addAction(actionPrestito_libri);
    menu_pr->addSeparator();
    menu_pr->addAction(actionPrestito_prodotti_digitali);
    menu_sc->addSeparator();
    menu_sc->addAction(this->actionScadenze_prodotti_digitali);
    menu_sc->addSeparator();
    p_art->setMenu(menu_p);
    p_art->setPopupMode(QToolButton::InstantPopup);

    menu_d = new QMenu(this);
    menu_plugins = new QMenu("Plugins",this);
    menu_d->addMenu(menu_plugins);
    menu_d->addAction(actionPreferenze);
    menu_d->addSeparator();
    menu_d->addAction(actionGestione_plugin);
    menu_d->addSeparator();
    menu_d->addAction(actionCalcola_codicefiscale);
    menu_d->addSeparator();
    menu_d->addAction(actionInstalla_plugin);
    toolButton_9->setMenu(menu_d);
    toolButton_9->setPopupMode(QToolButton::InstantPopup);

    menu_doc = new QMenu(this);
    menu_doc->addAction(this->actionFattura_d_aquisto);
    menu_doc->addSeparator();
    menu_doc->addAction(this->actionFattura_di_vendita_2);
     menu_doc->addSeparator();
    menu_doc->addAction(this->actionCarico_magazzino);
    menu_doc->addSeparator();
    menu_doc->addAction(this->actionScarico_magazzino);
    doc_fatt->setMenu(menu_doc);
    doc_fatt->setPopupMode(QToolButton::InstantPopup);

    connect(about_tool,SIGNAL(clicked()),this,SLOT(onactionAbout()));
    connect(actionAbout_2,SIGNAL(triggered()),this,SLOT(onactionAbout()));
    actionAbout_2->setIconVisibleInMenu(true);
    actionAbout_2->setShortcut(tr("Ctrl+H"));
    connect(close_ac,SIGNAL(triggered()),this,SLOT(onclose()));
    close_ac->setIconVisibleInMenu(true);
    close_ac->setShortcut(tr("Ctrl+ESC"));
    connect(toolButton,SIGNAL(clicked()),this,SLOT(onclose()));
    connect(action_articoli,SIGNAL(triggered()),this,SLOT(onarticoli()));
    action_articoli->setIconVisibleInMenu(true);
    action_articoli->setShortcut(tr("Ctrl+A"));
    connect(p_art,SIGNAL(clicked()),this,SLOT(onarticoli()));
    connect(clienti,SIGNAL(triggered()),this,SLOT(onclienti()));
    clienti->setIconVisibleInMenu(true);
    clienti->setShortcut(tr("Ctrl+G"));
    connect(toolButton_9,SIGNAL(clicked()),this,SLOT(onactiondatabasetriggered()));
    connect(actionCaterorie_libri,SIGNAL(triggered()),this,SLOT(categorie()));;
    actionCaterorie_libri->setIconVisibleInMenu(true);
    connect(actionPrestito_libri,SIGNAL(triggered()),this,SLOT(presta_libri()));
    actionPrestito_libri->setIconVisibleInMenu(true);
    connect(actionAbout_Qt,SIGNAL(triggered()),this,SLOT(onactiondatabasetriggered()));
    actionAbout_Qt->setIconVisibleInMenu(true);
    connect(actionPreferenze,SIGNAL(triggered()),this,SLOT(settings()));
    actionPreferenze->setIconVisibleInMenu(true);
    connect(actionLibri_prestati,SIGNAL(triggered()),this,SLOT(libri_pr()));
    actionLibri_prestati->setIconVisibleInMenu(true);
    connect(actionScadenze_prodotti_digitali,SIGNAL(triggered()),this,SLOT(prestiti_digitali_app()));
    actionScadenze_prodotti_digitali->setIconVisibleInMenu(true);
    connect(preferenze_2,SIGNAL(clicked()),this,SLOT(settings()));
    connect(about_luxury_2,SIGNAL(clicked()),this,SLOT(onactionAbout()));
    connect(about_qt_2,SIGNAL(clicked()),this,SLOT(onactiondatabasetriggered()));
    connect(categoria_2,SIGNAL(clicked()),this,SLOT(categorie()));
    connect(cl2_2,SIGNAL(clicked()),this,SLOT(presta_libri()));
    connect(scadenze_2,SIGNAL(clicked()),this,SLOT(libri_pr()));
    connect(clienti_3,SIGNAL(clicked()),this,SLOT(onclienti()));
    connect(libri_2,SIGNAL(clicked()),this,SLOT(onarticoli()));
    connect(actionAnagrafica_prodotti_digitali,SIGNAL(triggered()),this,SLOT(prodotti_digitali()));
    connect(actionPrestito_prodotti_digitali,SIGNAL(triggered()),this,SLOT(prestiti_digitali()));
    connect(gest_plugin,SIGNAL(clicked()),this,SLOT(gestioneplugin()));
    connect(actionGestione_plugin,SIGNAL(triggered()),this,SLOT(gestioneplugin()));
    connect(actionCalcola_codicefiscale,SIGNAL(triggered()),this,SLOT(gest_codfisc()));
    connect(actionAnagrafica_pagamenti,SIGNAL(triggered()),this,SLOT(gest_pagamento()));
    connect(actionCausali_di_trasporto,SIGNAL(triggered()),this,SLOT(gest_causali()));
    connect(actionAnagrafica_iva,SIGNAL(triggered()),this,SLOT(gest_iva()));
    connect(actionAnagrafica_azienda,SIGNAL(triggered()),this,SLOT(azienda_cod()));
    connect(actionAnagrafica_banche,SIGNAL(triggered()),this,SLOT(gest_banca()));
    connect(actionFattura_d_aquisto,SIGNAL(triggered()),this,SLOT(gest_fatt_acq()));
    connect(actionAnagraficafornitori,SIGNAL(triggered()),this,SLOT(gest_fornitori()));
    connect(actionFattura_di_vendita_2,SIGNAL(triggered()),this,SLOT(gest_fattura_v()));
    connect(actionCarico_magazzino,SIGNAL(triggered()),this,SLOT(vis_carico_magazzino()));
    connect(actionScarico_magazzino,SIGNAL(triggered()),this,SLOT(vis_scarico_magazzino()));
    connect(cod_fiscale,SIGNAL(clicked()),this,SLOT(gest_codfisc()));
    connect(actionInstalla_plugin,SIGNAL(triggered()),this,SLOT(installa_plugin()));
    onwid();
    azienda_ok();
    reload_data();
    riavvia_data();
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(messageClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::orologio(){
    cl = new AnalogClock(dockWidget);
    cl->show();
}

void MainWindow::onwid(){

    QMdiSubWindow *s = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    s->setWidget(groupBox);

    mdiArea->addSubWindow(s);

    groupBox->showMaximized();
}

void MainWindow::reload_data(){
    QString str,str2, st1,st;
    QSqlQuery query;
    connect(cl1,SIGNAL(clicked()),this,SLOT(libri_pr()));
    cl1->clear();
    query.prepare("select count(id),(data_rientro-now()) from prestiti where rientro='Non rientrato'");
    if(query.exec()){
      while(query.next()){

        if(query.value(1).toInt() == 0){
            cl1->clear();
            st.clear();
            str = query.value(0).toString();
            st = tr("Vi sono ")+str+ tr(" scadenze");
            cl1->setText(st);
        }
        else{
            cl1->setText("");
            st1.clear();
            str2 = query.value(1).toString();
            st1 = tr("Mancano ")+str2+tr(" giorni alla scadenza del prestito");
            cl1->setText(st1);
            }
    }
    }
    cl1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font("Arial",14,75);
    font.setUnderline(true);
    cl1->setFont(font);
    cl1->setCursor(QCursor(Qt::PointingHandCursor));
    cl1->show();
    gridLayout_15->addWidget(cl1,6, 0, 1, 8);
}

void MainWindow::riavvia_data(){

    QString str,str2, st1,st;
    QSqlQuery query;
    connect(cl2,SIGNAL(clicked()),this,SLOT(prestiti_digitali_app()));
    cl2->clear();
    query.prepare("select count(id),(data_rientro-now()) from prestiti_dvd where rientro='Non rientrato'");
    if(query.exec()){
      while(query.next()){

        if(query.value(1).toInt() == 0){
            cl2->clear();
            st.clear();
            str = query.value(0).toString();
            st = tr("Vi sono ")+str+ tr(" scadenze");
            cl2->setText(st);
        }
        else{
            cl2->setText("");
            st1.clear();
            str2 = query.value(1).toString();
            st1 = tr("Mancano ")+str2+tr(" giorni alla scadenza del prestito");
            cl2->setText(st1);
            }
    }
    }
    cl2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font("Arial",14,75);
    font.setUnderline(true);
    cl2->setFont(font);
    cl2->setCursor(QCursor(Qt::PointingHandCursor));
    cl2->show();
    gridLayout_4->addWidget(cl2,6, 0, 1, 8);
}


void MainWindow::gestioneplugin(){
    new pluginmanager(this);

    connect(this,SIGNAL(pluginLoaded(const QString &,bool)),manager,SLOT(pluginLoaded(const QString &,bool)));
    connect(manager,SIGNAL(pluginLoad(QObject*,QTreeWidgetItem*)),this,SLOT(pluginLoad(QObject*,QTreeWidgetItem*)));
    connect(manager,SIGNAL(pluginUnload(QObject*,QTreeWidgetItem*)),this,SLOT(pluginUnload(QObject*,QTreeWidgetItem*)));

    manager->leggiplugin();
    manager->show();
}

void MainWindow::readPlug(){
    pluginmanager *manager=new pluginmanager(this);

    connect(this,SIGNAL(pluginLoaded(const QString&, bool)),manager,SLOT(pluginLoaded(const QString&, bool)));
    connect(manager,SIGNAL(pluginLoad(QObject*,QTreeWidgetItem*)),this,SLOT(pluginLoad(QObject*,QTreeWidgetItem*)));
    connect(manager,SIGNAL(pluginUnload(QObject*,QTreeWidgetItem*)),this,SLOT(pluginUnload(QObject*,QTreeWidgetItem*)));

    manager->readPlugins();
}

void MainWindow::pluginLoad(QObject *plugin, QTreeWidgetItem *item){

    bool loaded = TRUE;

    if (loaded && qobject_cast<Interface_plugin*>(plugin))
        loaded = loadImPlugin( plugin );

    if(loaded){
        item->setText( 1, tr("Avviato...") );
        QString ic = ":/images/dialog-ok-apply.png";
        QIcon icona(ic);
        item->setIcon(1,icona);

        emit pluginLoaded( plugin->metaObject()->className(), TRUE );
    } else {
       pluginUnload( plugin, item );
    }

}


void MainWindow::pluginUnload(QObject *plugin, QTreeWidgetItem *item){

    if( qobject_cast<Interface_plugin*>(plugin) )
        unloadImPlugin( plugin );

    item->setText( 1, tr("Disattivato...") );
    QString ic = ":/images/list-remove.png";
    QIcon icona(ic);
    item->setIcon(1,icona);
    item->setSelected(false);
    emit pluginLoaded( plugin->metaObject()->className(), FALSE );
    delete plugin;
}

bool MainWindow::unloadImPlugin(QObject *pl){

    QString cname = pl->metaObject()->className();
    Interface_plugin *in = qobject_cast<Interface_plugin*>(pl);
    //settingsManager->setGeneralValue("plugin/loaded",cname);
    //Disabilito plugin
    in->pluginUnload();

    return FALSE;
}

bool MainWindow::loadImPlugin(QObject *plug){
    QString cname = plug->metaObject()->className();
    Interface_plugin *inter = qobject_cast<Interface_plugin*>(plug);
    //settingsManager->setGeneralValue("plugin/loaded",cname);
    //Avvio plugin
    if(plug->objectName().latin1()){
        populateMenus(plug);
    }
    inter->pluginLoad();

    return TRUE;
}

void MainWindow::populateMenus(QObject *plugin){
    Interface_plugin *in =  qobject_cast<Interface_plugin*>(plugin);
    if(in->name_menu() == "Strumenti"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_d,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuStrumenti_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == "Anagrafica"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_ana,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuAnagrafica_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == "Plugin"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_plugins,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPlugins_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == "Prestiti"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_pr,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPrestiti,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == "Scadenze"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_sc,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuScadenze,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == "Documenti"){
        addToMenu(plugin,in->icona(),in->nameplugin(),menu_doc,SLOT(avvia_widget()));
        addToMenu(plugin,in->icona(),in->nameplugin(),menuDocumenti,SLOT(avvia_widget()));
    }
}

void MainWindow::addToMenu(QObject *plugin, QIcon ico, const QString &name, QMenu *menu_add, const char *member){

        QAction *action = new QAction(ico,name, plugin);
        connect(action, SIGNAL(triggered()), this, member);
             menu_add->addAction(action);
             menu_add->addSeparator();
}

void MainWindow::avvia_widget(){

    QAction *action = qobject_cast<QAction *>(sender());
    Interface_plugin *iBrush = qobject_cast<Interface_plugin *>(action->parent());
    QString brush = action->text();
    brush = iBrush->nameplugin();

    iBrush->creaWidget()->show();
}

void MainWindow::avvia_backup(){
    #ifdef Q_OS_LINUX
    QString fileName = "libbackup_restore_db.lux";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        if(plif)
            plif->creaWidget()->show();
    }
    #elif defined Q_OS_WIN
    QString fileName = "backup_restore_db.lyb";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        if(plif)
            plif->creaWidget()->show();
    }
    #elif defined Q_OS_MACX
    QString fileName = "backup_restore_db.lap";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        if(plif)
            plif->creaWidget()->show();
    }
    #endif
}

void MainWindow::core(){
    if(settingsManager->generalValue("Backup/date",QVariant()).toDate().addDays(14) < QDate::currentDate() ){
            QMessageBox *box= new QMessageBox(this);
            box->setWindowTitle("Lylibrary");
            box->setText("Backup");
            box->setInformativeText("Sono passati 15 giorni senza eseguire un backup.\nVuoi eseguirlo adesso?");
            box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            box->setDefaultButton(QMessageBox::Ok);
            int ret = box->exec();
            switch(ret){
            case QMessageBox::Ok:
                 //Open backup
                 avvia_backup();
                 settingsManager->setGeneralValue("Backup/date",QDate::currentDate());
                 box->close();
                 break;
            case QMessageBox::Cancel:
                 //Close
                 settingsManager->setGeneralValue("Backup/date",QDate::currentDate());
                 box->close();
                 break;
            }
          }
}

void MainWindow::avvio_licenza(){

        lic = new license(this);
        lic->read_conf();
        lic->load_style(settingsManager->generalValue("Tema/sel_tema",QVariant()).toString());
        lic->exec();
}

void MainWindow::azienda_ok(){
    QSqlQuery query;
            query.prepare("select * from azienda");
            query.exec();
            if(query.next()){

            }
            else{
                QMessageBox MsgBox;
                MsgBox.setWindowTitle("LyLibrary");
                MsgBox.setText("Errore");
                MsgBox.setInformativeText("Inserire i campi dell'azienda");
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
                azienda_cod();
            }
}

void MainWindow::azienda_cod(){

        azUi = new azienda(this);
        azUi->setWindowModality(Qt::WindowModal);
        QMdiSubWindow *ws_az = new QMdiSubWindow(this,Qt::FramelessWindowHint);
        ws_az->setWidget(azUi);
        mdiArea->addSubWindow(ws_az);

        connect(azUi,SIGNAL(destroyed()),ws_az,SLOT(deleteLater()));
        azUi->showMaximized();
        QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica azienda...."));
}

void MainWindow::vis_carico_magazzino()
{
    carico_mag *car_mag = new carico_mag(this);
    QMdiSubWindow *ws_carico = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_carico->setWidget(car_mag);
    ws_carico->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(ws_carico);

    connect(car_mag,SIGNAL(destroyed()),ws_carico,SLOT(deleteLater()));
    car_mag->showMaximized();

    QMainWindow::statusBar()->showMessage(tr("Apertura carico magazzino...."));
}

void MainWindow::vis_scarico_magazzino()
{
    scarico_mag *scar_mag = new scarico_mag(this);
    QMdiSubWindow *ws_carico = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_carico->setWidget(scar_mag);
    ws_carico->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(ws_carico);

    connect(scar_mag,SIGNAL(destroyed()),ws_carico,SLOT(deleteLater()));
    scar_mag->showMaximized();

    QMainWindow::statusBar()->showMessage(tr("Apertura scarico magazzino...."));
}

void MainWindow::gest_fattura_v(){
    fattura *fa = new fattura(this);
    QMdiSubWindow *ws_fa = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_fa->setWidget(fa);
    ws_fa->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(ws_fa);

    connect(fa,SIGNAL(destroyed()),ws_fa,SLOT(deleteLater()));
    fa->show();
    QMainWindow::statusBar()->showMessage(tr("Apertura fattura di vendita...."));
}

void MainWindow::gest_banca(){

        banca = new banche(this);
        banca->setWindowModality(Qt::WindowModal);
        QMdiSubWindow *sid = new QMdiSubWindow(this,Qt::FramelessWindowHint);
        sid->setWidget(banca);
        sid->setGeometry(0,0,600,500);
        mdiArea->addSubWindow(sid);

        connect(banca,SIGNAL(destroyed()),sid,SLOT(deleteLater()));

        banca->showMaximized();
        QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica banca...."));
}

void MainWindow::prodotti_digitali(){

    new prodotti_dvd(this);

    digital->setWindowModality(Qt::WindowModal);
    connect(digital,SIGNAL(realodprodotti()),this,SLOT(riavvia_data()));
    QMdiSubWindow *dig_sub = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    dig_sub->setWidget(digital);
    mdiArea->addSubWindow(dig_sub);

    connect(digital,SIGNAL(destroyed()),dig_sub,SLOT(deleteLater()));
    digital->exec();
}

void MainWindow::prestiti_digitali_app(){

    dig_app = new prestito_dig(this);

    dig_app->setWindowModality(Qt::WindowModal);
    connect(dig_app,SIGNAL(riavvia()),this,SLOT(riavvia_data()));
    QMdiSubWindow *sub_app = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    sub_app->setWidget(dig_app);
    mdiArea->addSubWindow(sub_app);

    connect(dig_app,SIGNAL(destroyed()),sub_app,SLOT(deleteLater()));
    dig_app->exec();
}

void MainWindow::prestiti_digitali(){
    prestoapp = new prestito_app_dig(this);

    prestoapp->setModal(true);
    connect(prestoapp,SIGNAL(prestitodvd()),this,SLOT(reload_data()));
    QMdiSubWindow *pro = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    pro->setWidget(prestoapp);
    mdiArea->addSubWindow(pro);

    connect(prestoapp,SIGNAL(destroyed()),pro,SLOT(deleteLater()));
    prestoapp->exec();
}

void MainWindow::read(){

    if(settingsManager->generalValue("MainWindow/menu",QVariant()).toBool() == true){
        enable_menu();
    }
    else{
        disable_menu();
    }
    /***********************************************************
     * Impostazione font mainWindow
     ***********************************************************/
    QFont appfnt;
    appfnt.fromString(settingsManager->generalValue("Application/applicationFont","Verdana,-1,11,5,50,0,0,0,0,0").toString());
    QApplication::setFont(appfnt);

    settingsManager->generalValue("Version/version",QVariant()).toString();
    settingsManager->generalValue("Database/initialize",QVariant()).toString();


    if(settingsManager->generalValue("main/versione",QVariant()).toString() == "open source"){
            QString t = "open source";
            titolo(t);
            emit pluginLoaded(t,true);
        }

   if(settingsManager->generalValue("AGGIOR/auto_en",QVariant()).toString() == "true"){
       agg_automatico();
   }
   else{
       dis_agg_automatico();
   }

   if(settingsManager->generalValue("main/option1",QVariant()).toString().length() == 0){
       avvio_licenza();
   }
}

void MainWindow::enable_menu(){
    QMainWindow::menuBar()->setVisible(true);
    dock->setVisible(false);
    retranslateUi(this);
}

void MainWindow::disable_menu(){
    QMainWindow::menuBar()->setVisible(false);
    groupBox->setVisible(true);
    dock->setVisible(true);
    retranslateUi(this);
}

void MainWindow::settings(){

        sh1 = new pref(this);
        sh1->setModal(true);
        connect(sh1,SIGNAL(menu_mod()),this,SLOT(read()));

        if(sh1->isEnabled()){
            impo->setEnabled(false);
        }
        sh1->exec();
        QMainWindow::statusBar()->showMessage(tr("Apertura preferenze...."));
}

void MainWindow::agg_automatico(){
    QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
    trayIcon->showMessage("Lylibrary", "Aggiornamento automantico attivato",ts, 15*10000);
    sh1 = new pref;
    sh1->self_update();
    ag_tray->setVisible(true);
}

void MainWindow::dis_agg_automatico(){
    QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
    trayIcon->showMessage("Lylibrary", "Aggiornamento automantico disattivato",ts, 15*10000);
    ag_tray->setVisible(false);
}

void MainWindow::load_style(const QString &sheetName){

        QFile file(":/qss/" + sheetName.toLower() + ".qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());

        qApp->setStyleSheet(styleSheet);
}


void MainWindow::titolo(QString testo){

    QSqlQuery query;
    query.prepare("select * from azienda");
    query.exec();

    if(query.next()){
    QString str,title;
        title = "Lylibrary ";
        str = "rilasciato in licenza a "+query.value(0).toString()+"  v. "+settingsManager->generalValue("Version/version",QVariant()).toString()+"  ";
    setWindowTitle(title+str+testo);
    }
}

void MainWindow::presta_libri(){

    sh = new presto(this);
    connect(sh,SIGNAL(avvia_data()),this,SLOT(reload_data()));
    sh->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_sh = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_sh->setWidget(sh);
    ws_sh->setGeometry(0,0,500,500);
    mdiArea->addSubWindow(ws_sh);

    connect(sh,SIGNAL(destroyed()),ws_sh,SLOT(deleteLater()));

    sh->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura Prestito libri...."));

}

void MainWindow::categorie(){

        po = new porto(this);
        po->setWindowModality(Qt::WindowModal);
        QMdiSubWindow *ws_pot = new QMdiSubWindow(this,Qt::FramelessWindowHint);
        ws_pot->setWidget(po);
        ws_pot->setGeometry(6,6,700,500);
        mdiArea->addSubWindow(ws_pot);

        connect(po,SIGNAL(destroyed()),ws_pot,SLOT(deleteLater()));

        po->showMaximized();
        QMainWindow::statusBar()->showMessage(tr("Apertura categorie...."));
}

void MainWindow::onclose()
{
    esci *chiudi = new esci(this);
    chiudi->setWindowModality(Qt::WindowModal);
    chiudi->show();
    QMainWindow::statusBar()->showMessage(tr("Chiudi Lylibrary...."));
}

void MainWindow::onactionAbout()
{
    about *about1 = new about(this);
    about1->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_about = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_about->setWidget(about1);
    ws_about->setGeometry(10,10,430,430);
    mdiArea->addSubWindow(ws_about);

    connect(about1,SIGNAL(destroyed()),ws_about,SLOT(deleteLater()));
    about1->show();
    QMainWindow::statusBar()->showMessage(tr("Apertura about...."));
}

void MainWindow::gest_causali(){

    caus = new causali(this);
    caus->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_caus = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_caus->setWidget(caus);
    mdiArea->addSubWindow(ws_caus);

    connect(caus,SIGNAL(destroyed()),ws_caus,SLOT(deleteLater()));
    caus->show();
    QMainWindow::statusBar()->showMessage(tr("Apertura causali di trasporto...."));
}

void MainWindow::gest_codfisc(){

    fisc = new cod_fisc(this);
    fisc->setWindowModality(Qt::WindowModal);
    fisc->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura calcolo codice fiscale...."));

}

void MainWindow::gest_pagamento(){

    pagamento = new pag(this);
    pagamento->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_pagamento = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_pagamento->setWidget(pagamento);
    mdiArea->addSubWindow(ws_pagamento);

    connect(pagamento,SIGNAL(destroyed()),ws_pagamento,SLOT(deleteLater()));
    pagamento->show();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica pagamento...."));
}

void MainWindow::gest_iva(){

    iv = new iva(this);
    iv->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_iv = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_iv->setWidget(iv);
    mdiArea->addSubWindow(ws_iv);

    connect(iv,SIGNAL(destroyed()),ws_iv,SLOT(deleteLater()));
    iv->show();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica iva..."));
}

void MainWindow::gest_fatt_acq(){

    fatt = new fatt_acq(this);
    fatt->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_fatt = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_fatt->setWidget(fatt);
    mdiArea->addSubWindow(ws_fatt);

    connect(fatt,SIGNAL(destroyed()),ws_fatt,SLOT(deleteLater()));
    fatt->show();

    QMainWindow::statusBar()->showMessage(tr("Apertura fattura d'acquisto...."));
}

void MainWindow::gest_fornitori(){

    forn = new fornitori(this);
    forn->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_forn = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_forn->setWidget(forn);
    mdiArea->addSubWindow(ws_forn);

    connect(forn,SIGNAL(destroyed()),ws_forn,SLOT(deleteLater()));
    forn->show();
}

void MainWindow::onclienti()
{
    user *user1 = new user(this);
    user1->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_user = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_user->setWidget(user1);
    ws_user->setGeometry(10,10,600,520);
    mdiArea->addSubWindow(ws_user);

    connect(user1,SIGNAL(destroyed()),ws_user,SLOT(deleteLater()));
    user1->show();
    user1->showMaximized();
    QMainWindow::statusBar()->showMessage(tr("Apertura utenti...."));
}



void MainWindow::onarticoli()
{
    articoli *art = new articoli(this);
    art->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *ws_art = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    ws_art->setWidget(art);
    mdiArea->addSubWindow(ws_art);

    connect(art,SIGNAL(destroyed()),ws_art,SLOT(deleteLater()));

    art->show();
    art->showMaximized();
    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica libri...."));
}

void MainWindow::onactiondatabasetriggered()
{
    pdf = new read_pdf(this);
    pdf->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *sub_pdf = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    sub_pdf->setWidget(pdf);
    mdiArea->addSubWindow(sub_pdf);

    connect(pdf,SIGNAL(destroyed()),sub_pdf,SLOT(deleteLater()));

    pdf->show();
    QMainWindow::statusBar()->showMessage(tr("Apertura documentazione...."));

}

void MainWindow::closeEvent(QCloseEvent *event){

    scrivi_posizione();
    event->ignore();
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(tr("Lylibrary"),
                                 QString::fromAscii(tr("Il programma continuer\E0  a funzionare nella "
                                    "barra delle applicazioni. Per terminare il programma"
                                    " scegliere <b> Esci </b> nel menu contestuale"
                                    " del vassoio di sistema... ")),icon,15 * 1000);
        hide();
    }
    QMainWindow::closeEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *){
    scrivi_posizione();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    scrivi_posizione();
}

void MainWindow::createStatusBar(){
    QMainWindow::statusBar()->showMessage(tr("Database inizializzato correttamente"));

}

void MainWindow::libri_pr(){

    prest_lib *pr = new prest_lib(this);
    connect(pr,SIGNAL(riavvia()),this,SLOT(reload_data()));
    pr->setWindowModality(Qt::WindowModal);
    QMdiSubWindow *pr_sub = new QMdiSubWindow(this,Qt::FramelessWindowHint);
    pr_sub->setWidget(pr);
    mdiArea->addSubWindow(pr_sub);

    connect(pr,SIGNAL(destroyed()),pr_sub,SLOT(deleteLater()));
    pr->show();
    pr->showMaximized();
}

QString MainWindow::getLineFromCommandOutput(QString command){
    FILE *file = popen(command.toLatin1(),"r");

                char buffer[100];

                QString line = "";
                char firstChar;

                if ((firstChar = fgetc(file)) != -1){
                        line += firstChar;
                        line += fgets(buffer,100,file);
                }

                pclose(file);


                return line;
}
MainWindow::~MainWindow()
{
    mdiArea->closeAllSubWindows();
    delete manager;
    delete settingsManager;
}


void MainWindow::installa_plugin()
{
    #if defined Q_OS_LINUX
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "Plugin lylibrary(*.zip);;Tutti i file(*.*)");
    if(!fileName.isNull()){

            QProcess *proc = new QProcess(this);
            proc->start("pkexec unzip "+fileName+" -d /opt/lylibrary/plugin");
            if(proc->waitForFinished()){
            QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
            trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin \E8 stato installato correttamente..."),ts, 15*10000);
            ag_tray->setVisible(false);
            }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin non \E8 stato installato correttamente... "),ts, 15*10000);
        ag_tray->setVisible(false);
    }


    #elif defined Q_OS_WIN
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "Plugin lylibrary(*.zip);;Tutti i file(*.*)");
    fileName = QDir::toNativeSeparators(fileName);
    if(!fileName.isNull()){
        QProcess *proc = new QProcess(this);
        QString comando = "C:\\LyLibrary\\windows\\unzip "+fileName+" -d C:\\LyLibrary\\plugin";
        proc->start(comando);
        if(proc->waitForFinished()){
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin \E8 stato installato correttamente..."),ts, 15*10000);
        ag_tray->setVisible(false);
        }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin non \E8 stato installato correttamente... "),ts, 15*10000);
        ag_tray->setVisible(false);
    }
    #elif defined Q_OS_MACX
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "Plugin lylibrary(*.zip);;Tutti i file(*.*)");
    if(!fileName.isNull()){

            QProcess *proc = new QProcess(this);
            proc->start("unzip "+fileName+" -d ../plugin");
            if(proc->waitForFinished()){
            QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
            trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin \E8 stato installato correttamente..."),ts, 15*10000);
            ag_tray->setVisible(false);
            }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage("Lylibrary", QString::fromAscii("Il plugin non \E8 stato installato correttamente... "),ts, 15*10000);
        ag_tray->setVisible(false);
    }
    #endif
}


void MainWindow::scrivi_posizione()
{
    settingsManager->setGeneralValue("main/geometria",saveGeometry());
    settingsManager->setGeneralValue("main/salvastato",saveState());
    settingsManager->setGeneralValue("main/massimizza",isMaximized());

    if(!isMaximized())
    {
        settingsManager->setGeneralValue("main/pos",pos());
        settingsManager->setGeneralValue("main/size",size());
    }
}

void MainWindow::leggi_posizione()
{
    restoreGeometry(settingsManager->generalValue("main/geometria",saveGeometry()).toByteArray());
    restoreState(settingsManager->generalValue("main/slavastato",saveState()).toByteArray());
    move(settingsManager->generalValue("main/pos",pos()).toPoint());
    resize(settingsManager->generalValue("main/size",size()).toSize());

    if(settingsManager->generalValue("main/massimizza",isMaximized()).toBool())
        showMaximized();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

