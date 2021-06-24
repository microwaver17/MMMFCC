#include "abstractaudiosource.h"
#include "settings.h"
#include "log.h"

AbstractAudioSource::AbstractAudioSource(QObject *parent) : QObject(parent)
{
    Settings &stgs = Settings::getInstance();
    format.setCodec(stgs.codec);
    format.setSampleRate(stgs.sampleRate);
    format.setChannelCount(stgs.channels);
    format.setSampleSize(stgs.sampleSize);
    format.setSampleType(stgs.sampleType);
    format.setByteOrder(stgs.sampleEndian);

    decoder.setAudioFormat(format);
    decoder.setNotifyInterval(stgs.windowLength);

    connect(&decoder,&QAudioDecoder::bufferReady, this, &AbstractAudioSource::readDecodedAudioBuffer);
    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AbstractAudioSource::notifyDecodeError);
    connect(&decoder,&QAudioDecoder::finished, this, &AbstractAudioSource::finalizeDecode);
}

QVector<qint16> AbstractAudioSource::getRawSamples(int fromSamples)
{
    QMutexLocker locker(&rawSamplesMutex);
    if (fromSamples > rawSamples.size()){
        return QVector<qint16>();
    }

    Settings &stgs = Settings::getInstance();
    return rawSamples.mid(fromSamples, stgs.windowLength * stgs.sampleRate);
}

void AbstractAudioSource::startDecode()
{
    Log::getInstance().addLog(u8"デコード開始", this);
    if (decoder.state() == QAudioDecoder::DecodingState){
        decoder.stop();
    }
    rawSamples.clear();
    decoder.setSourceFilename(path);
    qDebug() << path;
    decoder.start();
}

void AbstractAudioSource::notifyDecodeError(QAudioDecoder::Error error){
    Log::getInstance().addLog(u8"デコードエラー " + decoder.errorString(), this);
    qDebug() << "decode error";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AbstractAudioSource::readDecodedAudioBuffer(){
    QMutexLocker locker(&rawSamplesMutex);

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
    Log::getInstance().addLog(u8"デコード終了", this);
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
