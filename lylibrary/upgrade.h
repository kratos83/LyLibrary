#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QtGui>
#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "ui_upgrade.h"

namespace Ui {
class upgrade;
}

class upgrade : public QDialog, public Ui::upgrade
{
    Q_OBJECT
    
public:
    explicit upgrade(QWidget *parent = 0);
    upgrade(QWidget *wig,QString url, QString agg);
    ~upgrade();
    QString txts;
    QString url_up;
private:
    QNetworkAccessManager *manager;
    QNetworkReply *currentDownload;
    QNetworkRequest mCurrentRequest;
    QFile *output;
    QTime downloadTime;

    int downloadedCount;
    int totalCount;
    int sizepause;

    bool stop,riavvio;
public slots:
    QString download(QString av_dw);
    void startNextDownload(QNetworkRequest& request);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void downloadFinished();
    void downloadReadyRead();
    void pause();
    void install_package();
    void download();

signals:
    void finished();
};

#endif // UPDATE_H
