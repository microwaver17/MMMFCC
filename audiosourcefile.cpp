#include "audiosourcefile.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>

AudioSourceFile::AudioSourceFile(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setCodec(codec);
    format.setSampleRate(sampleRate);
    format.setChannelCount(channels);
    format.setSampleSize(sampleSize);
    format.setSampleType(sampleType);
    format.setByteOrder(sampleEndian);

    decoder.setAudioFormat(format);

    connect(&decoder,&QAudioDecoder::bufferReady, this, &AudioSourceFile::readDecodedAudioBuffer);
    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AudioSourceFile::notifyDecodeError);
    connect(&decoder,&QAudioDecoder::finished, this, &AudioSourceFile::finalizeDecode);
}

void AudioSourceFile::startDecode(QString path)
{
    this->path = path;
    rawSamples.clear();
    decoder.setSourceFilename(path);
    decoder.start();
    qDebug("decode start");
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AudioSourceFile::notifyDecodeError(QAudioDecoder::Error error){
    qDebug() << "decode error";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AudioSourceFile::readDecodedAudioBuffer(){
    QAudioBuffer buffer = decoder.read();
    const qint16 *data = buffer.constData<qint16>();
    for (int i = 0; i < buffer.sampleCount(); i++){
        rawSamples.append(data[i]);
    }
    qDebug() << "decode buffer";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AudioSourceFile::finalizeDecode()
{
    qDebug() << "decode finish";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
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
    emit audioReady();
}
