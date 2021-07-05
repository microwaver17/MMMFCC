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
    void setSource(QString path);
    QMediaPlayer &getPlayer();
    QVector<qint16> getRawSamples(int fromSamles);

private:
    QVector<qint16> rawSamples;
    QMutex rawSamplesMutex;
    QAudioDecoder decoder;
    QAudioFormat format;
    QString path;
    QMediaPlayer player;
    bool bufferReady;

private slots:
    void readDecodedAudioBuffer();
    void finalizeDecode();
    void notifyDecodeError(QAudioDecoder::Error error);
    void playAgain();

};

#endif // AUDIOSOURCEFILE_H
