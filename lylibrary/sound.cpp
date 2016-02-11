#include <QDebug>
#include "sound.h"


sound::sound(QObject *parent) :
    QObject(parent)
{
}

void sound::avvia(QString resource){

      inputFile.setFileName(resource);
      inputFile.open(QIODevice::ReadOnly);

      QAudioFormat format;
      // Set up the format, eg.
      format.setSampleRate(96000);
      format.setChannelCount(1);
      format.setSampleSize(16);
      format.setCodec("audio/pcm");
      format.setByteOrder(QAudioFormat::LittleEndian);
      format.setSampleType(QAudioFormat::UnSignedInt);

      QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
      if (!info.isFormatSupported(format)) {
          qWarning()<<tr("Formato raw non supportato. Impossibile avviare audio...");
          return;
      }

      audioOutput = new QAudioOutput(format, this);
      connect(audioOutput,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
      audioOutput->start(&inputFile);
}

void sound::finishedPlaying(QAudio::State state)
{
    if (state == QAudio::IdleState) {
          audioOutput->stop();
          inputFile.close();
          delete audioOutput;
    }
}
