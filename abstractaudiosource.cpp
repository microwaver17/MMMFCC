#include "abstractaudiosource.h"
#include "consts.h"

AbstractAudioSource::AbstractAudioSource(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setCodec(Consts::codec);
    format.setSampleRate(Consts::sampleRate);
    format.setChannelCount(Consts::channels);
    format.setSampleSize(Consts::sampleSize);
    format.setSampleType(Consts::sampleType);
    format.setByteOrder(Consts::sampleEndian);

    decoder.setAudioFormat(format);
    decoder.setNotifyInterval(Consts::windowLength * 1.5);

    connect(&decoder,&QAudioDecoder::bufferReady, this, &AbstractAudioSource::readDecodedAudioBuffer);
    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AbstractAudioSource::notifyDecodeError);
    connect(&decoder,&QAudioDecoder::finished, this, &AbstractAudioSource::finalizeDecode);
}

QVector<qint16> AbstractAudioSource::getRawSamples(size_t fromSamples)
{
    QMutexLocker locker(&rawSampleMutex);
    return rawSamples.mid(fromSamples, Consts::windowLength * Consts::sampleRate);
}

void AbstractAudioSource::startDecode()
{
    rawSamples.clear();
    decoder.start();
}

void AbstractAudioSource::notifyDecodeError(QAudioDecoder::Error error){
    qDebug() << "decode error";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AbstractAudioSource::readDecodedAudioBuffer(){
    QMutexLocker locker(&rawSampleMutex);

    QAudioBuffer buffer = decoder.read();
    const qint16 *data = buffer.constData<qint16>();
    for (int i = 0; i < buffer.sampleCount(); i++){
        rawSamples.append(data[i]);
    }
//    qDebug() << "decode buffer";
//    qDebug() << decoder.duration();
//    qDebug() << decoder.errorString();
}

void AbstractAudioSource::finalizeDecode()
{
//    qDebug() << "decode finish";
//    qDebug() << decoder.duration();
//    qDebug() << decoder.errorString();
    /*
    QFile file(path + ".raw");
    if(file.open(QIODevice::WriteOnly)){
       for (int i = 0; i < rawSamples.size(); i++){
           const char *data = (char*)&(rawSamples.at(i));
           file.write(data, 2);
       }
       file.close();
    }
    */
}
