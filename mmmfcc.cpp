#include "mmmfcc.h"

#include <QFile>
#include <QtDebug>

MmMfcc::MmMfcc(QObject *parent) : QObject(parent)
{
    player.setNotifyInterval(100);

    connect(&player, &QMediaPlayer::positionChanged, this, &MmMfcc::updatePosition);
    connect(&audioSourceFile, &AudioSourceFile::audioReady, this, &MmMfcc::initPlayer);
}

MmMfcc::~MmMfcc()
{
}

void MmMfcc::setAudioFilePath(QString path)
{
    audioFilePath = path;
    audioSourceFile.startDecode(path);
}

void MmMfcc::initPlayer(){
    player.setMedia(QUrl::fromLocalFile(audioFilePath));
    player.play();
}

// プレイヤー操作
int MmMfcc::getPlayDuration()
{
    return player.duration();
}

int MmMfcc::getPlayPositon(){
    return player.position();
}

void MmMfcc::setPlayPosition(int position){
    player.setPosition(position);
}

void MmMfcc::togglePlayPause(){
    if (player.state() == QMediaPlayer::PlayingState){
        player.pause();
    }else{
        player.play();
    }
}

void MmMfcc::notifyPositionChanged(){
    // emit updatePosition();
}

void MmMfcc::test()
{

}
