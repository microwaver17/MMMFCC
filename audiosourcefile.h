#ifndef AUDIOSOURCEFILE_H
#define AUDIOSOURCEFILE_H

#include <QAudioDecoder>
#include <QAudioFormat>
#include <QObject>
#include <QMediaPlayer>
#include <QMutex>

#include "abstractaudiosource.h"

class AudioSourceFile : public AbstractAudioSource
{
    Q_OBJECT
public:
    explicit AudioSourceFile(QObject *parent = nullptr);
    void setSource(QString path);
    QMediaPlayer &getPlayer();

private:
    QString path;
    QMediaPlayer player;
    bool bufferReady;

private slots:
    void playAgain();

};

#endif // AUDIOSOURCEFILE_H
