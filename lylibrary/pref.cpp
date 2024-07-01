#include "pref.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>
#include "mainwindow.h"
#include <QtXml/QtXml>
#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QGraphicsView>
#include <QSysInfo>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "articoli.h"
#include "settingsmanager.h"
#include "Process.h"

pref::pref(QWidget *parent) :
    QDialog(parent), Ui::pref()
{
    setupUi(this);
    setModal(true);
    connect(chiudi,SIGNAL(clicked()),this,SLOT(esci()));
    connect(apply,SIGNAL(clicked()),this,SLOT(applica()));
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
    connect(en_update,SIGNAL(clicked(bool)),this,SLOT(agg_en()));
    connect(ds_update,SIGNAL(clicked(bool)),this,SLOT(agg_dis()));
    connect(update_b,SIGNAL(clicked()),this,SLOT(self_update()));
    connect(open_db,SIGNAL(clicked()),this,SLOT(connetti_database()));
    pwd->setEchoMode(QLineEdit::Password);
    settings = new SettingsManager(this);

    menu_pref();
    interfaccia();
    readsettings();
    visagg();
    combolanguage();
}


void pref::interfaccia(){
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setIconSize(QSize(84,84));
    listWidget->setMovement(QListView::Static);
    listWidget->setMaximumWidth(128);
    listWidget->setSpacing(12);
    listWidget->setCurrentRow(0);
}

void pref::readsettings(){

    settings->generalValue("Version/version",QVariant()).toString();
    en_update->setChecked(settings->generalValue("Update/agg_en",QVariant()).toBool());
    ds_update->setChecked(settings->generalValue("Update/agg_dis",QVariant()).toBool());

    ip->setText(settings->generalValue("DatabaseServer/indirizzoip",QVariant()).toString());
    porta->setValue(settings->generalValue("DatabaseServer/portadb",QVariant()).toInt());
    db_rete->setText(settings->generalValue("DatabaseServer/nomedb",QVariant()).toString());
    utente->setText(settings->generalValue("DatabaseServer/userdb",QVariant()).toString());
    pwd->setText(settings->generalValue("DatabaseServer/passdb",QVariant()).toString());

    //FONT APPLICATION
    cmbFontName->setCurrentText(settings->generalValue("Application/font",QVariant()).toString());
    QFont fnt;
    fnt.fromString(cmbFontName->currentText());
    cmbFontName->setFont(fnt);
    spinFontSize->setValue(settings->generalValue("ApplicationSizeFont/value",QVariant()).toInt());

    cmbApplicationFontName->setCurrentText(settings->generalValue("Application/font",QVariant()).toString());
    QFont appfnt;
    appfnt.fromString(cmbApplicationFontName->currentText());
    cmbApplicationFontName->setFont(appfnt);
    spinApplicationFontSize->setValue(settings->generalValue("ApplicationSizeFont/value",QVariant()).toInt());
    int sizeFont = spinApplicationFontSize->value();

#ifdef Q_OS_UNIX
     qApp->setFont(QFont(appfnt.toString(),sizeFont)); //FIXME
#elif defined Q_OS_WIN
    qApp->setFont(QFont(appfnt.toString(),sizeFont,10,false)); //FIXME
#endif

    combo_language->setCurrentText(settings->generalValue("Language/language",QVariant()).toString());
}

void pref::agg_en(){
    if(en_update->isChecked() == true){
        update_b->setVisible(true);
    }
    else
        agg_dis();
}

void pref::agg_dis(){
    if(ds_update->isChecked() == true){
        update_b->setVisible(false);
    }
    else
        agg_en();
}

void pref::visagg(){

    if(settings->generalValue("Update/agg_en",QVariant()).toBool() == true){
        update_b->setVisible(true);
    }
    else{
        update_b->setVisible(false);
    }

    if(settings->generalValue("Update/agg_dis",QVariant()).toBool() == true){
        update_b->setVisible(false);
    }
    else{
        update_b->setVisible(true);
    }

}

void pref::applica(){

    //Salvataggio
    //FONT
    settings->setGeneralValue("Application/font",cmbFontName->currentText());
    settings->setGeneralValue("ApplicationSizeFont/value",spinFontSize->value());

    //FONT APPLICATION
    settings->setGeneralValue("Application/font",cmbApplicationFontName->currentText());
    settings->setGeneralValue("ApplicationSizeFont/value",spinApplicationFontSize->value());

    settings->setGeneralValue("Update/agg_en",en_update->isChecked());
    settings->setGeneralValue("Update/agg_dis",ds_update->isChecked());

    settings->setGeneralValue("AGGIOR/auto_en",en_update->isChecked());
    settings->setGeneralValue("AGGIOR/auto_ds",ds_update->isChecked());

    settings->setGeneralValue("Language/language",combo_language->currentText());

    readsettings();
    emit load_plugins();
}

void pref::menu_pref(){

        configButton = new QListWidgetItem(listWidget);
        configButton->setIcon(QIcon(":/images/preferences-system.png"));
        configButton->setText(tr("Impostazioni\ngenerali"));
        configButton->setTextAlignment(Qt::AlignHCenter);
        configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        databaseButton = new QListWidgetItem(listWidget);
        databaseButton->setIcon(QIcon(":/images/svn-commit.png"));
        databaseButton->setText(tr("Database"));
        databaseButton->setTextAlignment(Qt::AlignHCenter);
        databaseButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        updateButton = new QListWidgetItem(listWidget);
        updateButton->setIcon(QIcon(":/images/system-software-update.png"));
        updateButton->setText(tr("Aggiornamento"));
        updateButton->setTextAlignment(Qt::AlignHCenter);
        updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void pref::changePage(QListWidgetItem *corrente,QListWidgetItem *indietro){
    if(!corrente)
        corrente = indietro;
    stackedWidget->setCurrentIndex(listWidget->row(corrente));
}

void pref::self_update(){

#if defined(Q_OS_LINUX) && defined(__x86_64__)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-lin64.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_LINUX) && defined(__x86)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-lin32.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_WIN64)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-win64.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_WIN32)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-win32.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_MACX)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-macx.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_FREEBSD)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-unix.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#endif
}

void pref::self_update_parse(QNetworkReply* reply){

            // Check file type
            QDomDocument doc( "XML" );
            if (!doc.setContent(reply->readAll(), true)) {
            return;
            }

            QDomElement root = doc.documentElement();
            if( root.tagName() != "appname" ){
                    return;
            }

            // Loop over main nodes
            latest_version.clear();
            filelist.clear();
            QDomNode mainnode = root.firstChild();
            while( !mainnode.isNull() ){
                    QDomNode subnode = mainnode.firstChild();
            if( !subnode.isNull() ) {
                // Loop over each elements in subnodes
                while(! subnode.isNull()){
                    QDomElement e = subnode.toElement();
                    if(!e.isNull()){
                        // Start parsing, finally
                                            if( e.tagName() == "version" ){
                                                    latest_version = e.attribute("value","");

                                            }
                                            else if( e.tagName() == "file" ){
                                                    filelist.push_back(e.attribute("value",""));
                                            }
                                    }
                                    subnode = subnode.nextSibling();
                            }
                    }
                    mainnode = mainnode.nextSibling();
            }

            // Compare current version with the one on the server
            if( settings->generalValue("Version/version",QVariant()).toString() < latest_version){
                    // Add the list of files to download list
                QString txtt= "http://www.codelinsoft.it/package/";

                QMessageBox *box= new QMessageBox(this);
                box->setWindowTitle(tr("LyLibrary"));
                box->setText(tr("Aggiornamento"));
                box->setInformativeText(tr("E' disponibile la nuova versione ")+versione+ tr(",se vuoi aggiornare clicca per aggiornare.\n"
                                        "Se clicchi ok si chiude il programma e si aggiorna il software alla nuova versione"));
                box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                box->setDefaultButton(QMessageBox::Ok);
                int ret = box->exec();
                switch(ret){
                      case QMessageBox::Ok:
                       //Open update
                       up_dw(QString::fromLatin1(filelist.data()->toStdString().c_str()),txtt);
                       box->close();
                       exit(0);
                       break;
                      case QMessageBox::Cancel:
                       //Close
                       box->close();
                       break;
                       }

            }
            else if(reply->error()){
                QMessageBox::warning(this,tr("LyLibrary"),QString(tr("Errore...\n")).arg(reply->errorString()));
            }
            else{
                    // Update success dialog
                QSystemTrayIcon::MessageIcon is1 = QSystemTrayIcon::Information;
                QMessageBox::information(this,tr("LyLibrary"),tr("Stai usando la nuova versione"));
            }
}


void pref::up_dw(QString package, QString url){

    process = new Process(this);
#if defined(Q_OS_LINUX)
    process->start("./update -u "+url+" -p "+package);
#elif defined(Q_OS_WIN)
    process->start("runas /TrustLevel:0x40000 C:\\LyLibrary\\update.exe -u "+url+" -p "+package);
#elif defined(Q_OS_MACX)
    process->start("./update.app/Contents/MacOS/update -u "+url+" -p "+package);
#elif defined (Q_OS_FREEBSD)
    process->start("./update -u "+url+" -p "+package);
#endif
}

bool pref::dir_server(){
    QString sgf = tr("Devi riavviare per rendere effettive le modifiche....");
    txt1->setText(sgf);
    return sgf.replace("'","''").toStdString().c_str();
}

void pref::connetti_database()
{
    if(ip->text().length() == 0 || db_rete->text().length() == 0 || utente->text().length() == 0 || pwd->text().length() == 0){
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8(tr("Errore di connessione al DB").replace("'","''").toStdString().c_str()));
        MsgBox2.setInformativeText(QString::fromUtf8(tr("Verificare che i dati siano corretti").replace("'","''").toStdString().c_str()));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
        ip->setStyleSheet("background-color: rgb(249, 22, 5)");
        db_rete->setStyleSheet("background-color: rgb(249, 22, 5)");
        utente->setStyleSheet("background-color: rgb(249, 22, 5)");
        pwd->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip->text());
    db.setPort(porta->value());
    db.setDatabaseName(db_rete->text());
    db.setUserName(utente->text());
    db.setPassword(pwd->text());

    settingsManager->setGeneralValue("DatabaseServer/indirizzoip",ip->text());
    settingsManager->setGeneralValue("DatabaseServer/portadb",porta->value());
    settingsManager->setGeneralValue("DatabaseServer/nomedb",db_rete->text());
    settingsManager->setGeneralValue("DatabaseServer/userdb",utente->text());
    settingsManager->setGeneralValue("DatabaseServer/passdb",pwd->text());
    settingsManager->setGeneralValue("Database/databaselocale","server");

    if (db.open())
    {
        QSqlQuery query1;
        query1.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'"+ip->text()+"' IDENTIFIED BY 'lylibrary'");
        messaggio();
    }
    else{
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8(tr("Errore di connessione al DB").replace("'","''").toStdString().c_str()));
        MsgBox2.setInformativeText(QString::fromUtf8(tr("Impossibile connettersi al db.Controllare le impostazioni.").replace("'","''").toStdString().c_str()));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
    }
    }
}

void pref::messaggio()
{
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle(tr("Lylibrary"));
    box->setText(tr("Database"));
    box->setInformativeText(tr("Creazione db strutturata con successo...."));
    box->setStandardButtons(QMessageBox::Ok);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Close
        #ifdef Q_OS_LINUX
        Process *proc = new Process();
        proc->start("/opt/lylibrary/lylibrary");
        #endif
        #ifdef Q_OS_WIN
        QProcess *proc = new QProcess();
        proc->start("C:\\Lylibrary\\lylibrary.exe");
        #endif
        #ifdef Q_OS_MAC
        QProcess *proc = new QProcess();
        proc->start("/Applications/lylibrary.app");
        #endif
        #ifdef Q_OS_FREEBSD
        QProcess *proc = new QProcess();
        proc->start("/opt/lylibrary/lylibrary");
        #endif
         box->close();
         exit(0);
         break;
    }
}

void pref::esci()
{
    close();
}

void pref::combolanguage()
{
	const QDir lang_dir(":/language/");
        const QStringList files = lang_dir.entryList(QStringList() << "*.qm");
        foreach (QString lang_file, files) {
            lang_file = lang_file.split('.').first();
            combo_language->addItem(lang_file);
        }

        default_language = set_language();
        if(files.contains(default_language+".qm"))
            default_language = "English";
}

QString pref::set_language()
{
    return QLocale::languageToString(QLocale().language());
}

void pref::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
