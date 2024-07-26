#include "update_db.h"
#include "ui_update_db.h"
#include "../lylibrary/Process.h"

update_db::update_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::update_db)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Aggiornamento database"));
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    connect(ui->p_aggiorna,SIGNAL(clicked()),this,SLOT(connetti()));
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    ui->textEdit->setReadOnly(true);
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/lylibrary/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.lylibrary/");
#endif

    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
}


QSqlDatabase update_db::connetti()
{
    QString db_lylibrary = "lylibrary";
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mysql");
    db.setUserName("root");
    db.setPassword(Base64ToAscii(general->value("Database/password").toString()));


    aggiorna_db(db,db_lylibrary);

    return db;
}

void update_db::aggiorna_db(QSqlDatabase db, QString db_lylibrary)
{
    if (db.open())
    {
        QSqlQuery query;
        db.setDatabaseName(db_lylibrary);
        qDebug() << QDir::currentPath();
#if defined Q_OS_MACX
            QProcess processo;
            QString comando;
            comando = getLineFromCommandOutput("mysql -u "+db.userName()+" -p"+Base64ToAscii(general->value("Database/password").toString())+" lylibrary < /Applications/LyLibrary/updatedb/update_db.sql");
            QStringList args;
            args << comando;
            processo.start(comando,args);
#else
        db.open();
        QFile file(QDir::currentPath()+"/updatedb/update_db.sql");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << "file non trovato";

        QTextStream in(&file);
        QString linea;
        QString sql_stato = "";

        while(!in.atEnd()){
            linea = in.readLine();
            if((linea.startsWith("/*") && linea.startsWith("--")) || linea.isEmpty())
                continue;
            sql_stato += linea;
            if(sql_stato.trimmed().endsWith(";")){

                aggiorna_tabelle(sql_stato);
                sql_stato = "";
                ui->progressBar->setMaximum(linea.size());
                for(int byte = 0; byte < sql_stato.toInt(); byte++){
                    ui->progressBar->setValue(byte);
                }
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
        }

        query.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'localhost' IDENTIFIED BY 'lylibrary'");
        ui->textEdit->append(tr("Aggiornamento database locale"));

        ui->textEdit->append(tr("Aggiornamento database in corso..."));
        messaggio();
#endif
        db.close();

        if(db.open()){

            ui->p_aggiorna->setEnabled(false);
        }
    }

    else
    {
                    QMessageBox MsgBox1;
                    MsgBox1.setText(QString::fromUtf8("Errore di connessione al DB"));
                    MsgBox1.setInformativeText(QString::fromUtf8("Impossibile aggiornare il database"));
                    MsgBox1.setWindowTitle("LyLibrary");
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
                    MsgBox1.setWindowIcon(icon);
                    MsgBox1.setIcon(QMessageBox::Warning);
                    MsgBox1.exec();
     }
}

void update_db::aggiorna_tabelle(const QString &sql) const{
    QSqlQuery query_prep;
    query_prep.prepare(sql);
    query_prep.exec();
    if(query_prep.exec())
        qDebug() << "Inserimento effettuato con successo...";
    else
        qDebug() << query_prep.lastError().text();
}

void update_db::messaggio()
{
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle("LyLibrary");
    box->setText("Database");
    box->setInformativeText("Aggiornamento db strutturato con successo....");
    box->setStandardButtons(QMessageBox::Ok);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Close
        #if defined (Q_OS_LINUX) || defined (Q_OS_FREBSD)
        Process *proc = new Process();
        proc->start("./lylibrary");
        #endif
        #ifdef Q_OS_WIN
        Process *proc = new Process();
        proc->start("lylibrary.exe");
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

QString update_db::Base64ToAscii(QString String){

    QByteArray ByteArray1=String.toUtf8();

    const char* chArr1=ByteArray1.constData();

    QByteArray ByteArray2=QByteArray::fromBase64(chArr1);

    const char* chArr2=ByteArray2.constData();

    return QString::fromUtf8(chArr2);
}

update_db::~update_db()
{
    delete ui;
}

QString update_db::getLineFromCommandOutput(QString command){
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
