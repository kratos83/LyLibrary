#include "upgrade.h"
#include "ui_upgrade.h"
#include <QtNetwork/QNetworkRequest>
#include <QDir>
#include <QStringList>

upgrade::upgrade(QWidget *parent) :
    QDialog(parent)
{

}

upgrade::upgrade(QWidget *wig, QString url, QString agg):
    QDialog(wig), downloadedCount(0), totalCount(0), sizepause(0)
{
    setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(down_agg,SIGNAL(clicked()),this,SLOT(download()));
    connect(dw_ps,SIGNAL(clicked()),this,SLOT(pause()));
    connect(inst_agg,SIGNAL(clicked()),this,SLOT(install_package()));
    url_up = url;
    txts = agg;
    dw_ps->setEnabled(false);
}

void upgrade::download(){

    dw_ps->setEnabled(true);
    inst_agg->setEnabled(false);
    QUrl url(url_up+txts);
    QString filename = QFileInfo(url.path()).fileName();
    sizepause =0;
    mCurrentRequest = QNetworkRequest(url);
    output = new QFile(filename+".part");
    output->open(QIODevice::ReadWrite);
    QString dw = "Scaricamento in corso di:  ";
    QString w = filename;
    textEdit->append(dw+" "+w);
    startNextDownload(mCurrentRequest);
    riavvio = true;
    stop = false;
}

QString upgrade::download(QString av_dw){

   return av_dw;
}

void upgrade::startNextDownload(QNetworkRequest& request){

        currentDownload = manager->get(request);

        connect(currentDownload, SIGNAL(finished()),SLOT(downloadFinished()));
        connect(currentDownload, SIGNAL(readyRead()),SLOT(downloadReadyRead()));
        connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),SLOT(downloadProgress(qint64,qint64)));
        connect(currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(error(QNetworkReply::NetworkError)));


        // prepare the output

        downloadTime.start();
}

void upgrade::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){

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
        test_agg->setText(QString::fromUtf8(tr("Velocità di scaricamento: ")+ts+"  Dimensione: "+ts1+" Tempo stimato: "+QString("%1:%2:%3").arg(hours,2,10, QLatin1Char('0')).arg(mins,2,10, QLatin1Char('0')).arg(secs,2,10, QLatin1Char('0'))));
        progressBar->update();
}

void upgrade::downloadFinished(){
                output->close();
                QUrl url(txts);
                QString filename = QFileInfo(url.path()).fileName();
                QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
                QStringList fileNames=direct->entryList( QStringList("*.part"), QDir::Files, QDir::Name);
                QString fl = fileNames.at(0);

                if(currentDownload->error()){
                    QString ts = "Download fallito: "+currentDownload->errorString();
                    textEdit->append(ts);
                    output->remove(fl);
                }
                else{
                output->rename(fl,filename);
                downloadReadyRead();
                inst_agg->setEnabled(true);
                ++downloadedCount;
                QString txs = "Scaricamento completato";
                textEdit->append(txs);
                emit finished();
                }

}

void upgrade::downloadReadyRead(){
    output->write(currentDownload->readAll());
     QUrl url(txts);
    QString filename = QFileInfo(url.path()).fileName();

    QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
    QStringList fileNames=direct->entryList( QStringList("*.run"), QDir::Files, QDir::Name);
    QProcess *lin_start = new QProcess(this);
    fileNames << filename;
#if defined (Q_OS_LINUX)
    lin_start->start("chmod 777 "+filename);
#elif defined (Q_OS_MAC)
    lin_start->start("chmod 777 "+filename);
#endif
}

void upgrade::pause(){


    QString p_s = tr("Scaricamento annullato");
    textEdit->append(p_s);

    if( currentDownload == 0 ) {
        textEdit->append("Errore scaricamento");
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

void upgrade::install_package(){

    QUrl url(txts);
   QString filename = QFileInfo(url.path()).fileName();

   QDir *direct = new QDir( QCoreApplication::applicationDirPath() );
   QStringList fileNames=direct->entryList( QStringList("*.run"), QDir::Files, QDir::Name);
   fileNames << filename;
#if defined(Q_OS_LINUX)
    QProcess *lin_start = new QProcess(this);
    QString kdesu = "kdesu";
    QString kdesudo = "kdesudo";
    QString gksu = "gksu";
    QString gksudo = "gksudo";
    QString sudo = "sudo";
    if(kdesu == "kdesu"){
        lin_start->start(kdesu+" ./"+filename);
    }
    else if(kdesu != "kdesu"){
        lin_start->start(kdesu+" ./"+filename);
    }
    else if(kdesudo == "kdesudo"){
        lin_start->start(kdesudo+" ./"+filename);
    }
    else if(kdesudo != "kdesudo"){
        lin_start->start(kdesudo+" ./"+filename);
    }
    else if(gksu == "gksu"){
        lin_start->start(gksu+" ./"+filename);
    }
    else if(gksu != "gksu"){
        lin_start->start(gksu+" ./"+filename);
    }
    else if(gksudo == "gksudo"){
        lin_start->start(gksudo+" ./"+filename);
    }
    else if(gksudo != "gksudo"){
        lin_start->start(gksudo+" ./"+filename);
    }
    else if(sudo == "sudo"){
        lin_start->start(sudo+" ./"+filename);
    }
    else if(sudo != "sudo"){
        lin_start->start(sudo+" ./"+filename);
    }
#elif defined(Q_OS_WIN)
    QProcess *win_start = new QProcess(this);
    win_start->start(filename);
#elif defined(Q_OS_MAC)
    QProcess *mac_start = new QProcess(this);
    QString sudo_cm = "sudo";
    QString open_cm = "open";
    if(sudo_cm == "sudo"){
        mac_start->start(sudo_cm+" -b ./"+filename);
    }
    else if(sudo_cm != "sudo"){
        mac_start->start(sudo_cm+" ./"+filename);
    }
    else if(open_cm == "open"){
        mac_start->start(open_cm+" ./"+filename);
    }
    else if(open_cm != "open"){
        mac_start->start(open_cm+" ./"+filename);
    }
#endif
}

void upgrade::error(QNetworkReply::NetworkError code){
    textEdit->append("Download fallito "+code);
}

upgrade::~upgrade()
{
}
