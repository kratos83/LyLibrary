#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "db_bk_rs.h"
#include "ui_db_bk_rs.h"
#include <QProcess>
#include <QLineEdit>
#include <QtGui>
#include <QTextStream>
#include <QtSql>

db_bk_rs::db_bk_rs(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(bak_but,SIGNAL(clicked()),this,SLOT(crea_backup()));
    connect(ripr_but,SIGNAL(clicked()),this,SLOT(restore_db()));
    connect(chiudi,SIGNAL(clicked()),this,SLOT(close()));
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/luxury/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.luxury/");
#endif
    password->setEchoMode(QLineEdit::Password);
    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
}

db_bk_rs::~db_bk_rs()
{
}

void db_bk_rs::crea_backup()
{
    if(password->text().length() == 0){
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Backup database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Inserisci la password, prima di effettuare il backup del database..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
    }
    else{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "*.bkp", "Database(*.bkp);;Tutti i file(*.*)");

            // Aggiunge estensione alla fine del file se non c'è
            if (fileName.indexOf(".bkp") > 0) {
                fileName += "_"+QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss")+".bkp";
            }


    if(!fileName.isNull()){
        QString comando;
        QStringList args;
        if(general->value("Database/databaselocale").toString() == "locale"){
        #ifdef Q_OS_LINUX
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
        #ifdef Q_OS_LINUX
        comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("mysqldump --database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysqldump.exe --database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+"> "+fileName);
        #endif
        }
        args << comando;
        backup = new QProcess(this);
        backup->start(comando,args);

        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Backup database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Backup database effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        qDebug("Backup database effettuato correttamente");
        MsgBox.exec();
        this->close();
    }
    else{
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Backup database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Backup database non effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }
    }

}

void db_bk_rs::restore_db()
{
    if(password->text().length() == 0){
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Ripristino database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Inserisci la password, prima di effettuare il ripristino del database..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
    }
    else{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "Database(*.bkp);;Tutti i file(*.*)");
    fileName = QDir::toNativeSeparators(fileName);

    if (!fileName.isNull()){
        QString comando;
        if(general->value("Database/databaselocale").toString() == "locale"){
        #ifdef Q_OS_LINUX
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
        #ifdef Q_OS_LINUX
        comando=getLineFromCommandOutput("mysql --one-database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        #ifdef Q_OS_MAC
        comando=getLineFromCommandOutput("/usr/local/bin/mysql --one-database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        #ifdef Q_OS_WIN
        comando=getLineFromCommandOutput("mysql.exe --one-database lylibrary -u root -p"+general->value("DatabaseServer/passdb").toString()+" --host="+general->value("DatabaseServer/indirizzoip").toString()+" < "+fileName);
        #endif
        }
        QStringList args;
        args << comando;
        backup = new QProcess(this);
        backup->start(comando,args);
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Ripristino database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Ripristino del database effettuato correttamente..."));
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.exec();
        this->close();
    }
    else
    {
        QMessageBox MsgBox;
        MsgBox.setText(QString::fromUtf8("Ripristino database..."));
        MsgBox.setInformativeText(QString::fromUtf8("Impossibile ripristinare il database..."));
        MsgBox.setIcon(QMessageBox::Information);
        qDebug("Impossibile ripristinare il database...");
        MsgBox.exec();
        this->close();
    }
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
