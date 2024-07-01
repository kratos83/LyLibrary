#include <QDebug>
#include "sound.h"


sound::sound(QObject *parent) :
    QObject(parent)
{
}

void sound::avvia(QString resource){

    QSound::play(resource);
}
