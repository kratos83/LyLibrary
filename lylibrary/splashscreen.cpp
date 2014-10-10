#include "splashscreen.h"

splashscreen::splashscreen(const QPixmap &pixmap) :
    QSplashScreen(pixmap)
{
    bar = new QProgressBar(this);
    bar->setGeometry(0, 265, 380, 19);
    bar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    bar->setMaximum(0);
    bar->setMaximum(100);
    bar->setEnabled(true);
}

void splashscreen::drawContents(QPainter *painter)
{
    bar->setValue(24);
    bar->render(painter);
    bar->update();
}

void splashscreen::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void splashscreen::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::LeftButton)
    {
        event->ignore();
    }
}
