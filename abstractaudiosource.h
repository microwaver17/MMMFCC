#ifndef ABSTRACTAUDIOSOURCE_H
#define ABSTRACTAUDIOSOURCE_H

#include <QAudioDecoder>
#include <QMutex>
#include <QObject>

class AbstractAudioSource : public QObject
{
    Q_OBJECT
public:
    QVector<qint16> getRawSamples(int from);

protected:
    explicit AbstractAudioSource(QObject *parent = nullptr);

    QVector<qint16> rawSamples;
    QMutex rawSamplesMutex;
    QAudioDecoder decoder;
    QAudioFormat format;
    QIODevice *device;
    QString path;

    void startDecode();

protected slots:
    void readDecodedAudioBuffer();
    void finalizeDecode();
    void notifyDecodeError(QAudioDecoder::Error error);
};

#endif // ABSTRACTAUDIOSOURCE_H
