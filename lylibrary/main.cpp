#include <QApplication>
#include <QSplashScreen>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include "mainwindow.h"
#if defined(Q_OS_UNIX)
#include <sys/time.h>
#elif defined(Q_OS_LINUX)
#include <time.h>
#elif defined(Q_OS_WIN)
#include <time.h>
#endif
#include "../initdb/connessione.h"
#include "../initdb/update_db.h"
#include <QProgressBar>
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QVariant>
#include "settingsmanager.h"
#include "splashscreen.h"


static void aggiorna_db()
{
    update_db *update = new update_db();
    QMessageBox *box= new QMessageBox;
    box->setWindowTitle("LyLibrary");
    box->setText(QObject::tr("Aggiornamento database"));
    box->setInformativeText(QObject::tr("Prima di effettuare l'aggiornamento al\ndatabase accertati di averne salvato\n"
               "una copia..."));
    box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Open database update
         update->exec();
         box->close();
         break;
    case QMessageBox::Cancel:
         //Close
         box->close();
         break;
    }
}

static void impostazioni(SettingsManager *settings)
{
    /**********************************
     *Impostazione della porta del db
     **********************************/
    if(settings->generalValue("DatabaseServer/portadb",QVariant()).toInt() == 0){
        int x = 3306;
        settings->setGeneralValue("DatabaseServer/portadb",x);
    }
    else{
        settings->generalValue("DatabaseServer/portadb",QVariant()).toInt();
    }

    /**********************************
     *Impostazione della versione
     **********************************/
    if(settings->generalValue("Version/version",QVariant()).toString().length() == 0){
        settings->setGeneralValue("Version/version","3.1");
    }
    else if(settings->generalValue("Version/version",QVariant()).toString() <= "3.0"){
        settings->setGeneralValue("Version/version","3.1");
    }
    else{
        settings->generalValue("Version/version",QVariant()).toString();
    }

    /*************************
     *Impostazione dei plugin
     *************************/
    if(settings->generalValue("plugin/loaded",QVariant()).toStringList().length() == 0){
        QStringList list;
        list << "dbbkrs_plugin" << "checkdatabase_plugin" << "inventario_plugin";
        settings->setGeneralValue("plugin/loaded",list);
    }
    else{
        settings->generalValue("plugin/loaded",QVariant()).toStringList();
    }
    
    /**********************************
     *Impostazione dei backup
     **********************************/
    if(settings->generalValue("Backup/date",QVariant()).toString().length() == 0){
            QDate curDate = QDate::currentDate();
            settings->setGeneralValue("Backup/date",curDate);
    }
    else{
            settings->generalValue("Backup/date",QVariant());
    }
    /**********************************
     *Impostazione deglla lingua
     **********************************/
    if(settings->generalValue("Language/language",QVariant()).toString().length() == 0){
        settings->setGeneralValue("Language/language","English");
    }
    else{
        settings->generalValue("Language/language",QVariant()).toString();
    }
}

static void traduzione(SettingsManager *settings)
{
  // Traduzione
    QTranslator translator;
    QString locale = settings->generalValue("Language/language",QVariant()).toString();
    translator.load(":/language/"+locale+".qm");
    qApp->installTranslator(&translator);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsManager *settings = new SettingsManager();
    impostazioni(settings);
    traduzione(settings);
    QPixmap img(":/images/splash.png");
    SplashScreen *splash = new SplashScreen(img,nullptr);
    splash->show();

    time_t t;
    t = time(NULL);

    while( difftime(time(NULL),t) < 3.0  ) {
        splash->showMessages(QString(QObject::tr("Avvio applicazione...")));
    }

    splash->showMessages(QString(QObject::tr("Avvio database...")));
    connessione *conn = new connessione();
    conn->traduzione();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    /*************************
     *Impostazione del database
     *************************/
    if(settingsManager->generalValue("Database/databaselocale","locale").toString()== "locale"){
        db.setHostName("localhost");
        db.setDatabaseName("lylibrary");
        db.setUserName("lylibrary");
        db.setPassword("lylibrary");
    }
    else if(settingsManager->generalValue("Database/databaselocale","server").toString()== "server"){
        db.setHostName(settingsManager->generalValue("DatabaseServer/indirizzoip",QVariant()).toString());
        db.setPort(settingsManager->generalValue("DatabaseServer/portadb",QVariant()).toInt());
        db.setDatabaseName(settingsManager->generalValue("DatabaseServer/nomedb",QVariant()).toString());
        db.setUserName(settingsManager->generalValue("DatabaseServer/userdb",QVariant()).toString());
        db.setPassword(settingsManager->generalValue("DatabaseServer/passdb",QVariant()).toString());
    }

    if(!db.open()){
        QMessageBox MsgBox;
        MsgBox.setText(QObject::tr("Errore di connessione al DB"));
        MsgBox.setInformativeText(QObject::tr("Controllare di aver installato MySql e di aver creato il DB lylibrary"));
        MsgBox.setIcon(QMessageBox::Warning);
        MsgBox.exec();
        conn->exec();
    }
    else if(db.open()){
	QString query = "SHOW TABLES LIKE 'impostazioni'";
	if(!db.exec(query).next()){
		aggiorna_db();
	}
	else{
		MainWindow *w = new MainWindow();
		w->leggi_posizione();
		w->avvia_audio();
		w->show();
        	splash->finish(w);
	}
    }
    delete splash;

    return a.exec();
}
