#include <QApplication>
#include <QSplashScreen>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include "mainwindow.h"
#include <time.h>
#include "../initdb/connessione.h"
#include <QProgressBar>
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QVariant>
#include "conn_db.h"
#include "settingsmanager.h"
#include "splashscreen.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsManager *settings = new SettingsManager();
    QPixmap img(":/images/splash.png");
       // QSplashScreen *splash = new QSplashScreen;
        //splash->setPixmap(img);
    splashscreen *splash = new splashscreen(img);
        splash->setMask(img.mask());
        splash->show();

    time_t t;
    t = time(NULL);


    while( difftime(time(NULL),t) < 3.0  ) {
        splash->showMessage(QString(QObject::tr("Application start")),Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
    }

    if(settings->generalValue("DatabaseServer/portadb",QVariant()).toInt() == 0){
        int x = 3306;
        settings->setGeneralValue("DatabaseServer/portadb",x);
    }
    else{
        settings->generalValue("DatabaseServer/portadb",QVariant()).toInt();
    }

    if(settings->generalValue("Version/version",QVariant()).toString().length() == 0){
        settings->setGeneralValue("Version/version","2.0");
    }
    else if(settings->generalValue("Version/version",QVariant()).toString() <= "2.0"){
        settings->setGeneralValue("Version/version","2.0");
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

    splash->showMessage(QString(QObject::tr("Start database")),Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

    // Traduzione
    QString locale = QLocale::system().name();
    QTranslator *translator = new QTranslator();
         translator->load(QString(":/language/biblio_")+locale);
         a.installTranslator(translator);

                 Database conn;
                 QSqlDatabase db = conn.connect();
                 db.open();
                 delete splash;


    return a.exec();
}
