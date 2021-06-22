#ifndef ABSTRACTAUDIOSOURCE_H
#define ABSTRACTAUDIOSOURCE_H

#include <QAudioDecoder>
#include <QMutex>
#include <QObject>

class AbstractAudioSource : public QObject
{
    Q_OBJECT
public:
    QVector<qint16> getRawSamples(size_t from);

protected:
    explicit AbstractAudioSource(QObject *parent = nullptr);

    QVector<qint16> rawSamples;
    QAudioDecoder decoder;

    void startDecode();

private:
    QIODevice *device;
    QString path;
    QMutex rawSampleMutex;


protected slots:
    void readDecodedAudioBuffer();
    void finalizeDecode();
    void notifyDecodeError(QAudioDecoder::Error error);
};

#endif // ABSTRACTAUDIOSOURCE_H
