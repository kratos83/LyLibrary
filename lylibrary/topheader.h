#ifndef TOPHEADER_H
#define TOPHEADER_H

#include <QWidget>

class topHeader : public QWidget
{
public:
    topHeader(QWidget *p = 0);

protected:
        void paintEvent ( QPaintEvent * event );
};

#endif // TOPHEADER_H
