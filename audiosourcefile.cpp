#include "audiosourcefile.h"
#include "log.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>
#include "settings.h"

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
    Log::getInstance().addLog(u8"オープン（ファイル） " + path, this);
    bufferReady = false;

    player.setMedia(QUrl::fromLocalFile(path));
    player.play();

    this->path = path;
    qDebug() << path;
    startDecode();
}
