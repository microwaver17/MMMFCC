#ifndef AUDIOSOURCEFILE_H
#define AUDIOSOURCEFILE_H

#include <QAudioDecoder>
#include <QAudioFormat>
#include <QObject>
#include <QMediaPlayer>
#include <QMutex>

class AudioSourceFile : public QObject
{
    Q_OBJECT
public:
    explicit AudioSourceFile(QObject *parent = nullptr);
    void startDecode(QString path);
    QVector<qint16> getRawSamples(size_t from);
    QMediaPlayer &getPlayer();

private:
    QString path;
    QAudioDecoder decoder;
    QMediaPlayer player;
    QVector<qint16> rawSamples;
    QMutex rawSampleMutex;

signals:
    void audioReady();

private slots:
    void readDecodedAudioBuffer();
    void finalizeDecode();
    void notifyDecodeError(QAudioDecoder::Error error);
    void playAgain();

};

#endif // AUDIOSOURCEFILE_H
