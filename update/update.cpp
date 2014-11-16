#include "update.h"
#include "ui_update.h"
#include <QtNetwork/QNetworkRequest>
#include <QDir>
#include <QStringList>

update::update(QWidget *parent) :
    QDialog(parent)
{
}

update::update(QWidget *wig, QString url, QString agg):
    QDialog(wig), downloadedCount(0), totalCount(0), sizepause(0)
{
    setupUi(this);
    setWindowTitle(tr("Gestore aggiornamento"));
    manager = new QNetworkAccessManager(this);
    connect(down_agg,SIGNAL(clicked()),this,SLOT(download()));
    connect(dw_ps,SIGNAL(clicked()),this,SLOT(pause()));
    connect(inst_agg,SIGNAL(clicked()),this,SLOT(install_package()));
    url_up = url;
    txts = agg;
    dw_ps->setEnabled(false);
}

void update::download(){

    dw_ps->setEnabled(true);
    inst_agg->setEnabled(false);
    QUrl url(url_up+txts);
    QString filename = QFileInfo(url.path()).fileName();
    sizepause =0;
    mCurrentRequest = QNetworkRequest(url);
    output = new QFile(filename+".part");
    output->open(QIODevice::ReadWrite);
    QString dw = tr("Scaricamento in corso di:  ");
    QString w = filename;
    textEdit->setText(dw+" "+w);
    startNextDownload(mCurrentRequest);
    riavvio = true;
    stop = false;
}

QString update::download(QString av_dw){

   return av_dw;
}

void update::startNextDownload(QNetworkRequest& request){

        currentDownload = manager->get(request);

        connect(currentDownload, SIGNAL(finished()),SLOT(downloadFinished()));
        connect(currentDownload, SIGNAL(readyRead()),SLOT(downloadReadyRead()));
        connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),SLOT(downloadProgress(qint64,qint64)));
        connect(currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(error(QNetworkReply::NetworkError)));


        // prepare the output

        downloadTime.start();
}

void update::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){

        progressBar->setMaximum(bytesTotal);
        progressBar->setValue(bytesReceived);

        // calcolo la velocità di download
        double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
        QString unit;
        if (speed < 1024) {
            unit = "bytes/sec";
        } else if (speed < 1024*1024) {
            speed /= 1024;
            unit = "kB/s";
        } else {
            speed /= 1024*1024;
            unit = "MB/s";
        }

        //Calcolo la dimensione del file da scaricare
        double peso = bytesReceived;

        QString dt;
        if (peso < 1024) {
            dt = "bytes";
        } else if (peso < 1024*1024) {
            peso /= 1024;
            dt = "kB";
        } else {
            peso /= 1024*1024;
            dt = "MB";
        }

        int secs = downloadTime.elapsed()/1000;
        int mins = (secs/60)%60;
        int hours = (secs/3600);
        secs = secs%60;

        QString ts = QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit);
        QString ts1 = QString::fromLatin1("%1 %2").arg(peso,3,'f',1).arg(dt);
        test_agg->setText(QString::fromUtf8(tr("Velocità di scaricamento: "))+ts+tr("  Dimensione: ")+ts1+tr(" Tempo stimato: ")+QString("%1:%2:%3").arg(hours,2,10, QLatin1Char('0')).arg(mins,2,10, QLatin1Char('0')).arg(secs,2,10, QLatin1Char('0')));
        progressBar->update();
}

void update::downloadFinished(){
                output->close();
                QUrl url(txts);
                QString filename = QFileInfo(url.path()).fileName();
                QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
                QStringList fileNames=direct->entryList( QStringList("*.part"), QDir::Files, QDir::Name);
                QString fl = fileNames.at(0);

                if(currentDownload->error()){
                    QString ts = tr("Download fallito: ")+currentDownload->errorString();
                    textEdit->setText(ts);
                    output->remove(fl);
                }
                else{
                output->rename(fl,filename);
                downloadReadyRead();
                inst_agg->setEnabled(true);
                ++downloadedCount;
                QString txs = tr("Scaricamento completato");
                textEdit->setText(txs);
                emit finished();
                install_package();
                }

}

void update::downloadReadyRead(){
    output->write(currentDownload->readAll());
    QUrl url(txts);
    QString filename = QFileInfo(url.path()).fileName();

    QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
    QStringList fileNames=direct->entryList( QStringList("*.zip"), QDir::Files, QDir::Name);
    QProcess *lin_start = new QProcess(this);
    fileNames << filename;
#if defined (Q_OS_LINUX)
    lin_start->start("chmod 777 "+filename);
#endif
}

void update::pause(){


    QString p_s = tr("Scaricamento annullato");
    textEdit->setText(p_s);

    if( currentDownload == 0 ) {
        textEdit->setText(tr("Errore scaricamento"));
    }

    disconnect(currentDownload,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    disconnect(currentDownload,SIGNAL(finished()),this,SLOT(downloadFinished()));

    dw_ps->setEnabled(false);
    inst_agg->setEnabled(false);
    currentDownload->abort();
    output->remove();
    progressBar->setValue(0);
    progressBar->update();
    test_agg->clear();
}

void update::install_package(){

   QUrl url(txts);
   QString filename = QFileInfo(url.path()).fileName();

   QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
   QString file_dir=direct->currentDirPath()+"/"+filename;
   QStringList fileNames=direct->entryList( QStringList("*.zip"), QDir::Files, QDir::Name);
   fileNames << filename;
#if defined(Q_OS_LINUX)
    lin_start = new QProcess(this);
    connect(lin_start,SIGNAL(readyReadStandardOutput()),this,SLOT(display_progress_bar()));
    lin_start->start("pkexec unzip -o "+file_dir+" -d /opt/codicefiscale/");
#elif defined(Q_OS_WIN)
    win_start = new QProcess(this);
    connect(win_start,SIGNAL(readyReadStandardOutput()),this,SLOT(display_progress_bar()));
    win_start->start("C:\\CodiceFiscale\\windows\\unzip -o "+file_dir+" -d C:\\CodiceFiscale");
#elif defined(Q_OS_MAC64)
    mac_start = new QProcess(this);
    connect(mac_start,SIGNAL(readyReadStandardOutput()),this,SLOT(display_progress_bar()));
    mac_start->start("unzip -o "+file_dir);
#endif
}

void update::display_progress_bar()
{
#if defined (Q_OS_LINUX)
    int val = lin_start->readLine().toInt();
#elif defined(Q_OS_WIN)
    int val = win_start->readLine().toInt();
#elif defined(Q_OS_MAC64)
    int val = mac_start->readLine().toInt();
#endif
    for(val=0;val <= 100; val++){
        unzip_file->setValue(val);
    }
    unzip_file->setValue(100);
}

void update::error(QNetworkReply::NetworkError code){
    textEdit->setText(tr("Download fallito ")+code);
}

update::~update()
{
}

void update::changeEvent(QEvent *e)
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
