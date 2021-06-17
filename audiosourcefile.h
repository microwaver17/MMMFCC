#ifndef AUDIOSOURCEFILE_H
#define AUDIOSOURCEFILE_H

#include <QAudioDecoder>
#include <QAudioFormat>
#include <QObject>

class AudioSourceFile : public QObject
{
    Q_OBJECT
public:
    explicit AudioSourceFile(QObject *parent = nullptr);
    void startDecode(QString path);

private:
    // デコード形式
    QString codec = "audio/pcm";
    const int sampleRate = 44100;
    const int channels = 1;
    const int sampleSize = 16;
    const QAudioFormat::SampleType sampleType = QAudioFormat::SampleType::SignedInt;
    const QAudioFormat::Endian sampleEndian = QAudioFormat::Endian::LittleEndian;

    QString path;
    QAudioDecoder decoder;
    QVector<qint16> rawSamples;

signals:
    void audioReady();

private slots:
    void readDecodedAudioBuffer();
    void finishDecode();
    void notifyDecodeError(QAudioDecoder::Error error);

};

#endif // AUDIOSOURCEFILE_H
