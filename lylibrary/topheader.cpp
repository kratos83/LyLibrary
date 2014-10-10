#include "topheader.h"

#include <QtGui>

topHeader::topHeader(QWidget *p)
        :QWidget(p)
{
    setFixedHeight(140);
}

void topHeader::paintEvent ( QPaintEvent * /*event*/ )
{
        QPainter p(this);
        p.fillRect(rect(), QBrush(qApp->palette().window() ));

        QLinearGradient headerBackgroundGrd(0,0, 0,100);

        headerBackgroundGrd.setColorAt(0, QColor(115,115,115));
        headerBackgroundGrd.setColorAt(1, QColor(65,65,65));
        p.fillRect(QRect(0,0,width(),200), QBrush(headerBackgroundGrd));
        p.drawPixmap(0,0,QPixmap(":/images/logo_maj.png"));
}
