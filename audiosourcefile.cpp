#include "audiosourcefile.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>
#include "consts.h"

AudioSourceFile::AudioSourceFile(QObject *parent) : AbstractAudioSource(parent)
  , bufferReady(false)
{
    player.setNotifyInterval(100);
    connect(&player, &QMediaPlayer::stateChanged, this, &AudioSourceFile::playAgain);
}

QMediaPlayer &AudioSourceFile::getPlayer()
{
    return player;
}

void AudioSourceFile::playAgain()
{
    if (player.state() == QMediaPlayer::StoppedState){
        player.play();
    }
}

void AudioSourceFile::setSource(QString path)
{
    bufferReady = false;

    player.setMedia(QUrl::fromLocalFile(path));
    player.play();

    decoder.setSourceFilename(path);
    startDecode();
}
