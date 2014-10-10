#include <QDebug>
#include "sound.h"


sound::sound(QObject *parent) :
    QObject(parent)
{
}

void sound::avvia(QString resource){

    addmusic = new Phonon::MediaObject(this);
    ouput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    ouput->setVolume(0.5);

    Phonon::createPath(addmusic,ouput);
    addmusic->setCurrentSource(QString(resource));
    addmusic->play();
}
