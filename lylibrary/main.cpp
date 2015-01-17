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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsManager *settings = new SettingsManager();
    QPixmap img(":/images/splash.png");
    splashscreen *splash = new splashscreen(img);
        splash->setMask(img.mask());
        splash->show();

    time_t t;
    t = time(NULL);


    while( difftime(time(NULL),t) < 3.0  ) {
        splash->showMessage(QString(QObject::tr("Avvio applicazione...")),Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
    }

    if(settings->generalValue("DatabaseServer/portadb",QVariant()).toInt() == 0){
        int x = 3306;
        settings->setGeneralValue("DatabaseServer/portadb",x);
    }
    else{
        settings->generalValue("DatabaseServer/portadb",QVariant()).toInt();
    }

    if(settings->generalValue("Version/version",QVariant()).toString().length() == 0){
        settings->setGeneralValue("Version/version","2.1.1");
    }
    else if(settings->generalValue("Version/version",QVariant()).toString() <= "2.1"){
        settings->setGeneralValue("Version/version","2.1.1");
    }
    else{
        settings->generalValue("Version/version",QVariant()).toString();
    }

    if(settings->generalValue("Backup/date",QVariant()).toString().length() == 0){
            QDate curDate = QDate::currentDate();
            settings->setGeneralValue("Backup/date",curDate);
        }
        else{
            settings->generalValue("Backup/date",QVariant());
        }

    if(settings->generalValue("Language/language",QVariant()).toString().length() == 0){
        settings->setGeneralValue("Language/language","English");
    }
    else{
        settings->generalValue("Language/language",QVariant()).toString();
    }

    splash->showMessage(QString(QObject::tr("Avvio database...")),Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

    // Traduzione
    QTranslator translator;
    QString locale = settings->generalValue("Language/language",QVariant()).toString();
    translator.load(":/language/lylibrary/"+locale+".qm");
    a.installTranslator(&translator);

         /*************************
          *Impostazione del tema
          *************************/
         if(settingsManager->generalValue("Tema/sel_tema",QVariant()).toString().length() == 0)
         {
             settingsManager->setGeneralValue("Tema/sel_tema","Modern");
         }
         else
         {
             settingsManager->generalValue("Tema/sel_tema",QVariant()).toString();
         }
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
                 MainWindow *w = new MainWindow();
                 w->leggi_posizione();
                 w->show();
                 }
                 delete splash;


    return a.exec();
}
