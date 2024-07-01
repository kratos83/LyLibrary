#include "connessione.h"
#include "ui_connessione.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>
#include <QSettings>
#include <QtGui/QtGui>
#include <QProcess>
#include <QTranslator>
#include "../lylibrary/Process.h"

connessione::connessione(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    connect(p_crea,SIGNAL(clicked()),this,SLOT(connetti()));
    connect(open_db,SIGNAL(clicked()),this,SLOT(connettidatabase()));
    connect(add_mysql_socket,SIGNAL(clicked(bool)),this,SLOT(add_vis_groupbox(bool)));
    timer = new QTimer(this);
    prog=0;
    textEdit->setReadOnly(true);
    setModal(true);
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/lylibrary/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.lylibrary/");
#endif

    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
    password->setEchoMode(QLineEdit::Password);
    pwd_lan->setEchoMode(QLineEdit::Password);
    confread();
}

connessione::~connessione()
{
}

QSqlDatabase connessione::connetti(){

    open_db->setEnabled(false);
    ip_dat->setEnabled(false);
    port->setEnabled(false);
    n_dat_lan->setEnabled(false);
    user->setEnabled(false);
    pwd_lan->setEnabled(false);
    QString db_lylibrary = "lylibrary";
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mysql");
    db.setUserName("root");
    db.setPassword(password->text());

    QDate date = QDate::currentDate();
    creadb(db,db_lylibrary,date.year());

    return db;
}

void connessione::setdate(int year){
    general->setValue("Esercizio/anno",year);
}

bool connessione::isNull(){
    return false;
}
void connessione::creadb(QSqlDatabase db, QString db_lylibrary, int year){


        if (db.open())
        {
            QSqlQuery query;
            connect(timer, SIGNAL(timeout()), this, SLOT(barra_progresso()));
            timer->start();
            query.exec("create database "+db_lylibrary+" CHARACTER SET=utf8");
            textEdit->append(tr("Creazione database lylibrary effettuata...."));
            db.close();

            db.setDatabaseName(db_lylibrary);
            db.open();
            textEdit->append(tr("Creazione database Lylibrary effettuata..."));

            QFile file(QDir::currentPath()+"/installdb/install_db.sql");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                qDebug() << "file non trovato" << file.fileName();

            QTextStream in(&file);
            QString linea;
            QString sql_stato = "";

            while(!in.atEnd()){
                linea = in.readLine();
                if((linea.startsWith("/*") && linea.startsWith("--")) || linea.isEmpty())
                    continue;
                sql_stato += linea;
                if(sql_stato.trimmed().endsWith(";")){

                    crea_tabelle(sql_stato);
                    sql_stato = "";
		    qint64 size = file.size();
		    progressBar->reset();
                    progressBar->setMaximum(size/1024);
                    for(qint64 byte = 0; byte < sql_stato.toInt(); byte+=1024){
                        progressBar->setValue(byte);
                    }
                    progressBar->setValue(progressBar->maximum());
                }
            }
	    
	    query.exec("INSERT INTO anno(anno_ins) VALUES("+QString::number(QDate::currentDate().year())+")");
	    
            general->setValue("Database/databaselocale","locale");
            general->setValue("Database/password",password->text().toLatin1().toBase64());
	    setdate(year);
            query.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'localhost' IDENTIFIED BY 'lylibrary'");
            textEdit->append(tr("Creazione database locale"));

            textEdit->append(tr("Configurazione database in corso..."));
            db.close();

            if(db.open()){
                p_crea->setEnabled(false);
            }
        }
        else
        {
           QMessageBox MsgBox1;
           MsgBox1.setText(QString::fromLatin1(tr("Errore di connessione al DB").replace("'","''").toStdString().c_str()));
           MsgBox1.setInformativeText(QString::fromLatin1(tr("Controllare di aver installato Mysql e di aver creato il DB lylibrary").replace("'","''").toStdString().c_str()));
           MsgBox1.setWindowTitle(tr("LyLibrary"));
           QIcon icon;
           icon.addFile(QString::fromLatin1(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
           MsgBox1.setWindowIcon(icon);
           MsgBox1.setIcon(QMessageBox::Warning);
           MsgBox1.exec();
        }
}

void connessione::barra_progresso(){
  textEdit->append(tr("Configurazione database completata con successo..."));
  messaggio();
}

void connessione::messaggio(){
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
        Process *proc = new Process();
        proc->start("C:\\Lylibrary\\lylibrary.exe");
        #endif
        #ifdef Q_OS_MAC
        Process *proc = new Process();
        proc->start("/Applications/lylibrary.app");
        #endif
         box->close();
         exit(0);
         break;
    }
}

void connessione::connettidatabase(){


    if(ip_dat->text().length() == 0 || n_dat_lan->text().length() == 0 || user->text().length() == 0 || pwd_lan->text().length() == 0){
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromLatin1(tr("Errore di connessione al DB").replace("'","''").toStdString().c_str()));
        MsgBox2.setInformativeText(QString::fromLatin1(tr("Verificare che i dati siano corretti").replace("'","''").toStdString().c_str()));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromLatin1(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
        ip_dat->setStyleSheet("background-color: rgb(249, 22, 5)");
        n_dat_lan->setStyleSheet("background-color: rgb(249, 22, 5)");
        user->setStyleSheet("background-color: rgb(249, 22, 5)");
        pwd_lan->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    p_crea->setEnabled(false);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip_dat->text());
    db.setPort(port->value());
    db.setDatabaseName(n_dat_lan->text());
    db.setUserName(user->text());
    db.setPassword(pwd_lan->text());


    general->setValue("DatabaseServer/indirizzoip",ip_dat->text());
    general->setValue("DatabaseServer/portadb",port->value());
    general->setValue("DatabaseServer/nomedb",n_dat_lan->text());
    general->setValue("DatabaseServer/userdb",user->text());
    general->setValue("DatabaseServer/passdb",pwd_lan->text().toLatin1().toBase64());
    general->setValue("Database/databaselocale","server");

    if (db.open())
    {
        QSqlQuery query1;
        query1.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'"+ip_dat->text()+"' IDENTIFIED BY 'lylibrary'");
        messaggio();
    }
    else{
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromLatin1(tr("Errore di connessione al DB").replace("'","''").toStdString().c_str()));
        MsgBox2.setInformativeText(QString::fromLatin1(tr("Impossibile connettersi al db.Controllare le impostazioni.").replace("'","''").toStdString().c_str()));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromLatin1(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
    }
    }
}

void connessione::confread(){

    ip_dat->setText(general->value("DatabaseServer/indirizzoip").toString());
    port->setValue(general->value("DatabaseServer/portadb").toInt());
    n_dat_lan->setText(general->value("DatabaseServer/nomedb").toString());
    user->setText(general->value("DatabaseServer/userdb").toString());
    pwd_lan->setText(Base64ToAscii(general->value("DatabaseServer/passdb").toString()));
}

bool connessione::add_vis_groupbox(bool vis)
{
    if(vis == true){
        groupBox_2->setVisible(false);
        QDialog::setFixedSize(424,371);
    }
    else if(vis == false)
    {
        groupBox_2->setVisible(true);
        QDialog::setFixedSize(848,371);
    }

    return vis;
}

void connessione::crea_tabelle(const QString &sql) const{
    QSqlQuery query_prep;
    query_prep.prepare(sql);
    query_prep.exec();
    
    qDebug() << query_prep.lastError().text();
}

QString connessione::Base64ToAscii(QString String){

    QByteArray ByteArray1=String.toUtf8();

    const char* chArr1=ByteArray1.constData();

    QByteArray ByteArray2=QByteArray::fromBase64(chArr1);

    const char* chArr2=ByteArray2.constData();

    return QString::fromUtf8(chArr2);
}

void connessione::closeEvent(QCloseEvent *event){

    event->ignore();

}

void connessione::traduzione()
{
    QString locale = general->value("Language/language").toString();
    QTranslator *translator = new QTranslator(this);
    translator->load(":/language/"+locale+".qm");
    qApp->installTranslator(translator);
}
