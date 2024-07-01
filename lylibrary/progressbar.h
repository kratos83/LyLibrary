#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QString>

class progressbar
{
public:
    progressbar();
    void clear();
    void update();
    void set_vis_msg(const QString &m);
    void set_vis_stato(qint64 val, qint64 max);

    private:
        QString msg;
        qint64 vl;
        qint64 mx;
        int iteration;
};

#endif // PROGRESSBAR_H
