#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QtGui>
#include <QCloseEvent>

class splashscreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit splashscreen(const QPixmap& pixmap);
    QProgressBar *bar;
    
signals:
    
public slots:
    virtual void drawContents(QPainter *painter);
    void closeEvent(QCloseEvent *);
    void mousePressEvent(QMouseEvent *);
    
};

#endif // SPLASHSCREEN_H
