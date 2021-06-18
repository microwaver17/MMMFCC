#ifndef MMMFCC_H
#define MMMFCC_H

#include <QAudioDecoder>
#include <QMediaPlayer>
#include <QObject>

#include "audiosourcefile.h"

class MmMfcc : public QObject
{
    Q_OBJECT
public:
    explicit MmMfcc(QObject *parent = nullptr);
    ~MmMfcc();

    void setAudioFilePath(QString path);
    int getPlayDuration();
    int getPlayPositon();
    void setPlayPosition(int position);
    void MmMfcc::togglePlayPause();

private:
    AudioSourceFile audioSourceFile;
    QMediaPlayer player;
    QString audioFilePath;

    void notifyPositionChanged();

signals:
    void updatePosition();

private slots:
    void initPlayer();
    void test();

};

#endif // MMMFCC_H
