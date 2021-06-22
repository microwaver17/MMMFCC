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
    decoder.setSourceFilename(path);
    player.setMedia(QUrl::fromLocalFile(path));
    player.play();
    startDecode();
    bufferReady = false;
}
