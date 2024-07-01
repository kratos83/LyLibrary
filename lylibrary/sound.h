#ifndef SOUND_H
#define SOUND_H

#include <QObject>

/********************************************
  @class sound
  @author Angelo Scarna' <calang83@gmail.com>
*********************************************/
#include <QtMultimedia/QtMultimedia>


class sound : public QObject
{
    Q_OBJECT
public:
    explicit sound(QObject *parent = 0);
    
signals:
    
public slots:
    void avvia(QString resource);
};

#endif // SOUND_H
