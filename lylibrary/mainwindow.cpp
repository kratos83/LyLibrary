
/************************************
  Classi QT
 ************************************/
#include <QDateTime>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QtSql/QtSql>
#include <QToolTip>
#include <QMessageBox>
#include <QDate>
#include <QSettings>
#include <QDesktopWidget>
#include <QMenu>
#include <QWidget>
#include <QToolButton>
#include <QToolTip>
#include <QFileDialog>

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
    interfaccia();
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

void MainWindow::interfaccia(){
    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->dumpObjectTree();
    setCentralWidget(mdiArea);
    
    cl1 = new classe(this);

    connect(actionAbout_2,SIGNAL(triggered()),this,SLOT(onactionAbout()));
    actionAbout_2->setIconVisibleInMenu(true);
    actionAbout_2->setShortcut(tr("Ctrl+H"));
    connect(close_ac,SIGNAL(triggered()),this,SLOT(onclose()));
    close_ac->setIconVisibleInMenu(true);
    close_ac->setShortcut(tr("Ctrl+ESC"));
    connect(action_articoli,SIGNAL(triggered()),this,SLOT(onarticoli()));
    action_articoli->setIconVisibleInMenu(true);
    action_articoli->setShortcut(tr("Ctrl+A"));
    connect(clienti,SIGNAL(triggered()),this,SLOT(onclienti()));
    clienti->setIconVisibleInMenu(true);
    clienti->setShortcut(tr("Ctrl+G"));
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
    connect(preferenze_2,SIGNAL(clicked()),this,SLOT(settings()));
    connect(about_luxury_2,SIGNAL(clicked()),this,SLOT(onactionAbout()));
    connect(about_qt_2,SIGNAL(clicked()),this,SLOT(onactiondatabasetriggered()));
    connect(categoria_2,SIGNAL(clicked()),this,SLOT(categorie()));
    connect(cl2_2,SIGNAL(clicked()),this,SLOT(presta_libri()));
    connect(scadenze_2,SIGNAL(clicked()),this,SLOT(libri_pr()));
    connect(clienti_3,SIGNAL(clicked()),this,SLOT(onclienti()));
    connect(libri_2,SIGNAL(clicked()),this,SLOT(onarticoli()));
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
    connect(actionCerca_CAP,SIGNAL(triggered()),this,SLOT(cerca_cap_comune()));
    connect(actionVerifica_patrtita_iva,SIGNAL(triggered()),this,SLOT(verifica_part_iva()));
    connect(actionRicerca_aggiornamento,SIGNAL(triggered()),this,SLOT(gest_update()));
    connect(actionVerifica_codice_fiscale_2,SIGNAL(triggered()),this,SLOT(verifica_cod_fisc()));
    connect(donazione,SIGNAL(clicked()),this,SLOT(dona()));
    onwid();
    azienda_ok();
    reload_data();
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(messageClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::orologio(){
    cl = new AnalogClock(m_orologio);
    gridLayout_3->addWidget(cl);
    cl->show();
}

void MainWindow::dona()
{
    QString apri_link = "http://www.codelinsoft.it/sito/2015-02-18-23-59-25/donate.html";
    QDesktopServices::openUrl(QUrl(apri_link));
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
        else if(query.value(1).toString().mid(0, 1) == "-"){
            cl1->setText("");
            st1.clear();
            str2 = query.value(1).toString();
            st1 = tr("Sono passati ")+str2+tr(" giorni alla scadenza del prestito");
            cl1->setText(st1);
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

void MainWindow::gestioneplugin(){
    new pluginmanager(this);

    connect(this,SIGNAL(pluginLoaded(const QString &,bool)),manager,SLOT(pluginLoaded(const QString &,bool)));
    connect(manager,SIGNAL(pluginLoad(QObject*,QTreeWidgetItem*)),this,SLOT(pluginLoad(QObject*,QTreeWidgetItem*)));
    connect(manager,SIGNAL(pluginUnload(QObject*,QTreeWidgetItem*)),this,SLOT(pluginUnload(QObject*,QTreeWidgetItem*)));

    manager->leggiplugin();
    manager->show();
}

void MainWindow::readPlug(){
    new pluginmanager(this);

    connect(this,SIGNAL(pluginLoaded(const QString&, bool)),manager,SLOT(pluginLoaded(const QString&, bool)));
    connect(manager,SIGNAL(pluginLoad(QObject*,QTreeWidgetItem*)),this,SLOT(pluginLoad(QObject*,QTreeWidgetItem*)));
    connect(manager,SIGNAL(pluginUnload(QObject*,QTreeWidgetItem*)),this,SLOT(pluginUnload(QObject*,QTreeWidgetItem*)));

    manager->leggiplugin();
}

void MainWindow::pluginLoad(QObject *plugin, QTreeWidgetItem *item){

    bool loaded = true;

    if (loaded && qobject_cast<Interface_plugin*>(plugin))
        loaded = loadImPlugin( plugin );

    if(loaded){
        item->setText( 1, tr("Avviato...") );
        QString ic = ":/images/dialog-ok-apply.png";
        QIcon icona(ic);
        item->setIcon(1,icona);

        emit pluginLoaded( plugin->metaObject()->className(), true );
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
    emit pluginLoaded( plugin->metaObject()->className(), false );
    delete plugin;
}

bool MainWindow::unloadImPlugin(QObject *pl){

    QString cname = pl->metaObject()->className();
    Interface_plugin *in = qobject_cast<Interface_plugin*>(pl);
    //Disabilito plugin
    removePopulateMenus(pl);
    in->pluginUnload();
    return false;
}

bool MainWindow::loadImPlugin(QObject *plug){
    QString cname = plug->metaObject()->className();
    Interface_plugin *inter = qobject_cast<Interface_plugin*>(plug);
    //Avvio plugin
    populateMenus(plug);
    inter->pluginLoad();
    return true;
}

void MainWindow::populateMenus(QObject *plugin){
    Interface_plugin *in =  qobject_cast<Interface_plugin*>(plugin);
    if(in->name_menu() == tr("Strumenti")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuStrumenti_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Anagrafica")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuAnagrafica_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Plugin")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPlugins_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Prestiti")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPrestiti,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Scadenze")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuScadenze,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Documenti")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuDocumenti,SLOT(avvia_widget()));
    }
}

void MainWindow::removePopulateMenus(QObject *plugin)
{
    Interface_plugin *in =  qobject_cast<Interface_plugin*>(plugin);
    if(in->name_menu() == tr("Strumenti")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuStrumenti_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Anagrafica")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuAnagrafica_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Plugin")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPlugins_2,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Prestiti")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuPrestiti,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Scadenze")){
        addToMenu(plugin,in->icona(),in->nameplugin(),menuScadenze,SLOT(avvia_widget()));
    }
    else if(in->name_menu() == tr("Documenti")){
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
    iBrush->pluginTranslator();
    iBrush->creaWidget()->show();
}

void MainWindow::avvia_backup(){
    #ifdef Q_OS_LINUX
    QString fileName = "libbackup_restore_db.so";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        plif->pluginTranslator();
        if(plif)
            plif->creaWidget()->show();
    }
    #elif defined Q_OS_WIN
    QString fileName = "backup_restore_db.dll";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        plif->pluginTranslator();
        if(plif)
            plif->creaWidget()->show();
    }
    #elif defined Q_OS_MACX
    QString fileName = "backup_restore_db.dylib";
    loader->setFileName( pluginDir->absoluteFilePath(fileName) );
    QObject *plugin=loader->instance();
    if(plugin){
        Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
        plif->pluginTranslator();
        if(plif)
            plif->creaWidget()->show();
    }
    #endif
}

void MainWindow::core(){
    if(settingsManager->generalValue("Backup/date",QVariant()).toDate().addDays(14) < QDate::currentDate() ){
            QMessageBox *box= new QMessageBox(this);
            box->setWindowTitle(tr("Lylibrary"));
            box->setText(tr("Backup"));
            box->setInformativeText(tr("Sono passati 15 giorni senza eseguire un backup.\nVuoi eseguirlo adesso?"));
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
        lic->traduzione();
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
                MsgBox.setWindowTitle(tr("LyLibrary"));
                MsgBox.setText(tr("Errore"));
                MsgBox.setInformativeText(tr("Inserire i campi dell'azienda"));
                MsgBox.setIcon(QMessageBox::Warning);
                MsgBox.exec();
                azienda_cod();
            }
}

void MainWindow::azienda_cod(){

        azUi = new azienda(this);
        azUi->setWindowModality(Qt::WindowModal);
        azUi->exec();
        QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica azienda...."));
}

void MainWindow::vis_carico_magazzino()
{
    carico_mag *car_mag = new carico_mag(this);
    car_mag->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura carico magazzino...."));
}

void MainWindow::vis_scarico_magazzino()
{
    scarico_mag *scar_mag = new scarico_mag(this);
    scar_mag->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura scarico magazzino...."));
}

void MainWindow::gest_fattura_v(){
    fattura *fa = new fattura(this);
    fa->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura fattura di vendita...."));
}

void MainWindow::gest_banca(){

        banca = new banche(this);
        banca->exec();
        QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica banca...."));
}

void MainWindow::read(){

    /***********************************************************
     * Impostazione font mainWindow
     ***********************************************************/
    QFont appfnt;
    appfnt.fromString(settingsManager->generalValue("Application/applicationFont",QVariant()).toString());
    QApplication::setFont(appfnt);

    settingsManager->generalValue("Version/version",QVariant()).toString();
    settingsManager->generalValue("Database/initialize",QVariant()).toString();


    if(settingsManager->generalValue("main/versione",QVariant()).toString() == "open source"){
            QString t = tr("open source");
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

void MainWindow::settings(){

        sh1 = new pref(this);
        sh1->setModal(true);
        connect(sh1,SIGNAL(menu_mod()),this,SLOT(read()));

        if(sh1->isEnabled()){
            impo->setEnabled(false);
        }

        sh1->readsettings();
        sh1->exec();

        QMainWindow::statusBar()->showMessage(tr("Apertura preferenze...."));
}

void MainWindow::agg_automatico(){
    QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
    trayIcon->showMessage(tr("Lylibrary"), tr("Aggiornamento automatico attivato"),ts, 15*10000);
    sh1 = new pref;
    sh1->self_update();
    ag_tray->setVisible(true);
}

void MainWindow::dis_agg_automatico(){
    QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
    trayIcon->showMessage(tr("Lylibrary"), tr("Aggiornamento automatico disattivato"),ts, 15*10000);
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
        title = tr("Lylibrary ");
        str = tr("rilasciato in licenza a ")+query.value(1).toString()+"  v. "+settingsManager->generalValue("Version/version",QVariant()).toString()+"  ";
    setWindowTitle(title+str+testo);
    }
}

void MainWindow::presta_libri(){

    sh = new presto(this);
    connect(sh,SIGNAL(avvia_data()),this,SLOT(reload_data()));
    sh->setWindowModality(Qt::WindowModal);
    sh->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura Prestito libri...."));

}

void MainWindow::categorie(){

        po = new porto(this);
        po->setWindowModality(Qt::WindowModal);
        po->exec();
        QMainWindow::statusBar()->showMessage(tr("Apertura categorie...."));
}

void MainWindow::onclose()
{
    esci *chiudi = new esci(this);
    chiudi->setWindowModality(Qt::WindowModal);
    chiudi->exec();
    QMainWindow::statusBar()->showMessage(tr("Chiudi Lylibrary...."));
}

void MainWindow::onactionAbout()
{
    about *about1 = new about(this);
    about1->setWindowModality(Qt::WindowModal);
    about1->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura about...."));
}

void MainWindow::gest_causali(){

    caus = new causali(this);
    caus->setWindowModality(Qt::WindowModal);
    caus->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura causali di trasporto...."));
}

void MainWindow::gest_codfisc(){

    fisc = new cod_fisc(this);
    fisc->setWindowModality(Qt::WindowModal);
    fisc->show();

    QMainWindow::statusBar()->showMessage(tr("Apertura calcolo codice fiscale italiano...."));

}

void MainWindow::gest_pagamento(){

    pagamento = new pag(this);
    pagamento->setWindowModality(Qt::WindowModal);
    pagamento->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica pagamento...."));
}

void MainWindow::gest_iva(){

    iv = new iva(this);
    iv->setWindowModality(Qt::WindowModal);
    iv->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica iva..."));
}

void MainWindow::gest_fatt_acq(){

    fatt = new fatt_acq(this);
    fatt->setWindowModality(Qt::WindowModal);
    fatt->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura fattura d'acquisto...."));
}

void MainWindow::gest_fornitori(){

    forn = new fornitori(this);
    forn->setWindowModality(Qt::WindowModal);
    forn->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica fornitori..."));
}

void MainWindow::onclienti()
{
    user *user1 = new user(this);
    user1->setWindowModality(Qt::WindowModal);
    user1->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica utenti...."));
}



void MainWindow::onarticoli()
{
    articoli *art = new articoli(this);
    art->setWindowModality(Qt::WindowModal);
    art->exec();
    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica libri...."));
}

void MainWindow::onactiondatabasetriggered()
{
    //Da avviare documentazione

}

void MainWindow::closeEvent(QCloseEvent *event){

    scrivi_posizione();
    event->ignore();
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(tr("Lylibrary"),
                                 (QString::fromUtf8(tr("Il programma continuer\E0  a funzionare nella "
                                    "barra delle applicazioni. Per terminare il programma"
                                    " scegliere <b> Esci </b> nel menu contestuale"
                                    " del vassoio di sistema... ").toStdString().c_str())),icon,15 * 1000);
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
    pr->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura anagrafica prestiti libri...."));
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
#if defined (Q_OS_LINUX)
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Plugin lylibrary(*.zip);;Tutti i file(*.*)"));
    if(!fileName.isNull()){

            QProcess *proc = new QProcess(this);
            proc->start("pkexec unzip "+fileName+" -d /opt/lylibrary/plugin");
            if(proc->waitForFinished()){
            QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
            trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin è stato installato correttamente...").toStdString().c_str()),ts, 15*10000);
            ag_tray->setVisible(false);
            }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin non è stato installato correttamente... ").toStdString().c_str()),ts, 15*10000);
        ag_tray->setVisible(false);
    }


#elif defined (Q_OS_WIN)
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Plugin lylibrary(*.zip);;Tutti i file(*.*)"));
    fileName = QDir::toNativeSeparators(fileName);
    if(!fileName.isNull()){
        QProcess *proc = new QProcess(this);
        QString comando = "C:\\LyLibrary\\windows\\unzip "+fileName+" -d C:\\LyLibrary\\plugin";
        proc->start(comando);
        if(proc->waitForFinished()){
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin è stato installato correttamente...").toStdString().c_str()),ts, 15*10000);
        ag_tray->setVisible(false);
        }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin non è stato installato correttamente... ").toStdString().c_str()),ts, 15*10000);
        ag_tray->setVisible(false);
    }
#elif defined (Q_OS_MACX)
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Plugin lylibrary(*.zip);;Tutti i file(*.*)"));
    if(!fileName.isNull()){

            QProcess *proc = new QProcess(this);
            proc->start("unzip "+fileName+" -d ../plugin");
            if(proc->waitForFinished()){
            QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
            trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin è stato installato correttamente...").toStdString().c_str()),ts, 15*10000);
            ag_tray->setVisible(false);
            }
    }
    else{
        QSystemTrayIcon::MessageIcon ts = QSystemTrayIcon::Information;
        trayIcon->showMessage(tr("Lylibrary"), QString::fromUtf8(tr("Il plugin non è stato installato correttamente... ").toStdString().c_str()),ts, 15*10000);
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
    restoreState(settingsManager->generalValue("main/salvastato",saveState()).toByteArray());
    move(settingsManager->generalValue("main/pos",pos()).toPoint());
    resize(settingsManager->generalValue("main/size",size()).toSize());

    if(settingsManager->generalValue("main/massimizza",isMaximized()).toBool())
        showMaximized();
}

void MainWindow::cerca_cap_comune()
{
    find_cap = new find_cap_italian(this);
    find_cap->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura finestra per cercare il cap..."));
}

void MainWindow::verifica_part_iva()
{
    verifica_iva = new verify_piva(this);
    verifica_iva->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura finestra per verificare la partita iva..."));
}

void MainWindow::verifica_cod_fisc()
{
    verifica_codice = new verify_codicefiscale(this);
    verifica_codice->exec();

    QMainWindow::statusBar()->showMessage(tr("Apertura finestra per verificare il codice fiscale..."));
}

void MainWindow::gest_update()
{
    sh1 = new pref(this);
    sh1->self_update();
}

void MainWindow::avvia_audio()
{
    sn = new sound(this);
    sn->avvia(":/images/login.wav");
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

