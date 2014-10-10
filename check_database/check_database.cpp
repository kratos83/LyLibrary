#include "check_database.h"
#include "ui_check_database.h"
#include <QDebug>

check_database::check_database(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::check_database)
{
    ui->setupUi(this);
    this->setWindowTitle("Analizza e ripara database");
    connect(ui->analize_db,SIGNAL(clicked()),this,SLOT(analizza_database()));
    connect(ui->correct_db,SIGNAL(clicked()),this,SLOT(ripristina_database()));
    connect(ui->chiudi,SIGNAL(clicked()),this,SLOT(close()));
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/luxury/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.luxury/");
#endif

    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
}

void check_database::analizza_database()
{
    ui->outpuarea->clear();
    #ifdef Q_OS_LINUX
    QString comando="mysqlcheck --databases lylibrary -a -u root -p"+general->value("Database/password").toString();
    #endif
    #ifdef Q_OS_MAC
    QString comando="mysqlcheck --databases lylibrary -a -u root -p"+general->value("Database/password").toString();
    #endif
    #ifdef Q_OS_WIN
    QString comando="mysqlcheck --databases lylibrary -a -u root -p"+general->value("Database/password").toString();
    #endif
    analizza = new QProcess(this);
    analizza->setReadChannel(QProcess::StandardOutput);
    analizza->setProcessChannelMode(QProcess::MergedChannels);
    analizza->start(comando);

    if(!analizza->waitForStarted()){
        ui->outpuarea->append("Errore, impossbile avviare il servizio mysqlcheck...");
    }
    analizza->closeWriteChannel();
    ui->outpuarea->append("Analizzazione tabelle in corso...\n\n");
    while(analizza->state()!=QProcess::NotRunning){
        analizza->waitForReadyRead();
        QByteArray result = analizza->readAll();
        ui->outpuarea->append(QString(result));
    }
    ui->outpuarea->append("Fine analizzazione...");
}

void check_database::ripristina_database()
{
    ui->outpuarea->clear();
    #ifdef Q_OS_LINUX
    QString comando="mysqlcheck --databases lylibrary -r -u root -p"+general->value("Database/password").toString();
    #endif
    #ifdef Q_OS_MAC
    QString comando="mysqlcheck --databases lylibrary -r -u root -p"+general->value("Database/password").toString();
    #endif
    #ifdef Q_OS_WIN
    QString comando="mysqlcheck --databases lylibrary -r -u root -p"+general->value("Database/password").toString();
    #endif
    ripristina = new QProcess(this);
    ripristina->setReadChannel(QProcess::StandardOutput);
    ripristina->setProcessChannelMode(QProcess::MergedChannels);
    ripristina->start(comando);

    if(!ripristina->waitForStarted()){
    ui->outpuarea->append("Errore, impossbile avviare il servizio mysqlcheck...");
    }
    ripristina->closeWriteChannel();

    ui->outpuarea->append("Riparazione tabelle in corso...\n\n");
    while(ripristina->state()!=QProcess::NotRunning){
        ripristina->waitForReadyRead();
        QByteArray result = ripristina->readAll();
        ui->outpuarea->append(QString(result));
    }
    ui->outpuarea->append("Fine riparazione...");
}

QString check_database::getLineFromCommandOutput(QString command){
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

check_database::~check_database()
{
    delete ui;
}
