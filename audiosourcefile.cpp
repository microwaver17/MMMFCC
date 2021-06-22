#include "audiosourcefile.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>
#include "consts.h"

AudioSourceFile::AudioSourceFile(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setCodec(Consts::codec);
    format.setSampleRate(Consts::sampleRate);
    format.setChannelCount(Consts::channels);
    format.setSampleSize(Consts::sampleSize);
    format.setSampleType(Consts::sampleType);
    format.setByteOrder(Consts::sampleEndian);

    decoder.setAudioFormat(format);
    player.setNotifyInterval(100);

    connect(&decoder,&QAudioDecoder::bufferReady, this, &AudioSourceFile::readDecodedAudioBuffer);
    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AudioSourceFile::notifyDecodeError);
    connect(&decoder,&QAudioDecoder::finished, this, &AudioSourceFile::finalizeDecode);
    connect(&player, &QMediaPlayer::stateChanged, this, &AudioSourceFile::playAgain);
}

QVector<qint16> AudioSourceFile::getRawSamples(size_t fromSamples)
{
    QMutexLocker locker(&rawSampleMutex);
    return rawSamples.mid(fromSamples, Consts::windowLength * Consts::sampleRate);
}

QMediaPlayer &AudioSourceFile::getPlayer()
{
    return player;
}

void AudioSourceFile::startDecode(QString path)
{
    this->path = path;
    rawSamples.clear();
    player.setMedia(QUrl::fromLocalFile(path));
    decoder.setSourceFilename(path);
    decoder.start();
//    qDebug("decode start");
//    qDebug() << decoder.duration();
//    qDebug() << decoder.errorString();
}

void AudioSourceFile::notifyDecodeError(QAudioDecoder::Error error){
//    qDebug() << "decode error";
//    qDebug() << decoder.duration();
    //    qDebug() << decoder.errorString();
}

void AudioSourceFile::playAgain()
{
    if (player.state() == QMediaPlayer::StoppedState){
        player.play();
    }
}

void AudioSourceFile::readDecodedAudioBuffer(){
    QMutexLocker locker(&rawSampleMutex);

    QAudioBuffer buffer = decoder.read();
    const qint16 *data = buffer.constData<qint16>();
    for (int i = 0; i < buffer.sampleCount(); i++){
        rawSamples.append(data[i]);
    }

    // デコードしたバッファが溜まったら再生
    if ((player.state() != QMediaPlayer::PlayingState) && (rawSamples.size() > (Consts::windowLength / 1000.0) * Consts::sampleRate * 2)){
        player.play();
    }
//    qDebug() << "decode buffer";
//    qDebug() << decoder.duration();
//    qDebug() << decoder.errorString();
}

void AudioSourceFile::finalizeDecode()
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
    emit audioReady();
}
