#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "db_bk_rs.h"
#include "ui_db_bk_rs.h"
#include <QLineEdit>
#include <QtGui>
#include <QTextStream>
#include <QTranslator>
#include <QtSql>

db_bk_rs::db_bk_rs(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/lylibrary/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.lylibrary/");
#endif
    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);

    setAttribute(Qt::WA_DeleteOnClose);
    connect(bak_but,SIGNAL(clicked()),this,SLOT(crea_backup()));
    connect(ripr_but,SIGNAL(clicked()),this,SLOT(restore_db()));
    connect(chiudi,SIGNAL(clicked()),this,SLOT(close()));

    password->setEchoMode(QLineEdit::Password);

}

db_bk_rs::~db_bk_rs()
{
}

void db_bk_rs::crea_backup()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "*.bkp", tr("Database(*.bkp);;Tutti i file(*.*)"));

            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".bkp") > 0) {
                fileName += "_"+QDateTime::currentDateTime().toString(tr("dd-MM-yyyy-hh-mm-ss"))+".bkp";
            }


    if(!fileName.isNull()){
        QString comando;
        QStringList args;
        if(general->value("Database/databaselocale").toString() == "locale"){
        #if defined(Q_OS_LINUX) || defined (Q_OS_FREEBSD)
            comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+password->text()+"> "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+password->text()+"> "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysqldump.exe --database lylibrary -u root -p"+password->text()+"> "+fileName);
        #endif
        }
        else if(general->value("Database/databaselocale").toString() == "server"){
        #if defined(Q_OS_LINUX) || defined (Q_OS_FREEBSD)
        comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysqldump.exe --database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        }
        args << comando;
        backup = new Process(this);
        backup->start(comando,args);

        QMessageBox MsgBox;
        MsgBox.setText(tr("Backup database..."));
        MsgBox.setInformativeText(tr("Backup database effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }
    else{
        QMessageBox MsgBox;
        MsgBox.setText(tr("Backup database..."));
        MsgBox.setInformativeText(tr("Backup database non effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }


}

void db_bk_rs::restore_db()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Database(*.bkp);;Tutti i file(*.*)"));
    fileName = QDir::toNativeSeparators(fileName);

    if (!fileName.isNull()){
        QString comando;
        if(general->value("Database/databaselocale").toString() == "locale"){
        #if defined(Q_OS_LINUX) || defined (Q_OS_FREEBSD)
        comando=getLineFromCommandOutput("mysql --one-database lylibrary -u root -p"+password->text()+" < "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("/usr/local/mysql/bin/mysql --one-database lylibrary -u root -p"+password->text()+" < "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysql.exe --one-database lylibrary -u root -p"+password->text()+" < "+fileName);
        #endif
        }
        else if(general->value("Database/databaselocale").toString() == "server"){
        #if defined(Q_OS_LINUX) || defined (Q_OS_FREEBSD)
        comando=getLineFromCommandOutput("mysql --one-database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("/usr/local/bin/mysql --one-database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysql.exe --one-database lylibrary -u root -p"+password->text()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        }
        QStringList args;
        args << comando;
        backup = new Process(this);
        backup->start(comando,args);
        QMessageBox MsgBox;
        MsgBox.setText(tr("Ripristino database..."));
        MsgBox.setInformativeText(tr("Ripristino del database effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }
    else
    {
        QMessageBox MsgBox;
        MsgBox.setText(tr("Ripristino database..."));
        MsgBox.setInformativeText(tr("Impossibile ripristinare il database..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }


}

QString db_bk_rs::getLineFromCommandOutput(QString command){
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
