#include <QMessageBox>
#include <QSqlDatabase>
#include <QtSql>
#include "conn_db.h"
#include "../initdb/connessione.h"
#include "mainwindow.h"
#include <QSettings>
#include "settingsmanager.h"

QSqlDatabase Database::connect(){

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
        /*************************
         *Impostazione del tema
         *************************/
        if(settingsManager->generalValue("Tema/sel_tema",QVariant()).toString().length() == 0)
        {
            settingsManager->setGeneralValue("Tema/sel_tema","Style");
        }
        else
        {
            settingsManager->generalValue("Tema/sel_tema",QVariant()).toString();
        }


        if (!db.open())
        {
            QMessageBox MsgBox;
            MsgBox.setText(QString::fromUtf8("Errore di connessione al DB"));
            MsgBox.setInformativeText(QString::fromUtf8("Controllare di aver installato MySql e di aver creato il DB LyLibrary"));
            MsgBox.setIcon(QMessageBox::Warning);
            MsgBox.exec();
            connessione *conn = new connessione();
            conn->exec();
        }
        else{
        if(db.open()){
        MainWindow *w = new MainWindow();
        w->showMaximized();
        }
        }
        return db;
}

void Database::closeDb(){
    QSqlDatabase::removeDatabase("QMYSQL");
}

