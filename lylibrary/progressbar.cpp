#include "progressbar.h"
#include <QByteArray>
#include <stdio.h>
#include <iostream>

using namespace std;

progressbar::progressbar()
: vl(0), mx(-1), iteration(0)
{
}

void progressbar::clear(){

    printf("\n");
    fflush(stdout);

    iteration = 0;
    vl = 0;
    mx = -1;
}

void progressbar::update(){

    ++iteration;

       if (mx > 0) {
           // we know the maximum
           // draw a progress bar
           int percent = vl * 100 / mx;
           int hashes = percent / 2;

           QByteArray progressbar(hashes, '#');
           if (percent % 2)
               progressbar += '>';

           printf("\r[%-50s] %3d%% %s     ",
                  progressbar.constData(),
                  percent,
                  qPrintable(msg));
       } else {
           // we don't know the maximum, so we can't draw a progress bar
           int center = (iteration % 48) + 1; // 50 spaces, minus 2
           QByteArray before(qMax(center - 2, 0), ' ');
           QByteArray after(qMin(center + 2, 50), ' ');

           printf("\r[%s###%s]      %s      ",
                  before.constData(), after.constData(), qPrintable(msg));
       }
}

void progressbar::set_vis_msg(const QString &m){
    msg = m;
}

void progressbar::set_vis_stato(qint64 val, qint64 max){
    vl = val;
    mx = max;
}
