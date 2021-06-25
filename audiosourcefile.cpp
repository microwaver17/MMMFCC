#include "audiosourcefile.h"
#include "log.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>
#include "settings.h"

AudioSourceFile::AudioSourceFile(QObject *parent) : QObject(parent)
  , format(SETTINGS.getFormat())
  , bufferReady(false)
{
    decoder.setAudioFormat(format);
    decoder.setNotifyInterval(SETTINGS.windowLength);

    connect(&decoder,&QAudioDecoder::bufferReady, this, &AudioSourceFile::readDecodedAudioBuffer);
    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AudioSourceFile::notifyDecodeError);
    connect(&decoder,&QAudioDecoder::finished, this, &AudioSourceFile::finalizeDecode);

    player.setNotifyInterval(100);
    connect(&player, &QMediaPlayer::stateChanged, this, &AudioSourceFile::playAgain);
}

QVector<qint16> AudioSourceFile::getRawSamples(int fromSamples)
{
    QMutexLocker locker(&rawSamplesMutex);
    if (fromSamples > rawSamples.size()){
        return QVector<qint16>();
    }

    return rawSamples.mid(fromSamples, SETTINGS.windowLength * SETTINGS.sampleRate);
}

QMediaPlayer &AudioSourceFile::getPlayer()
{
    return player;
}

void AudioSourceFile::playAgain()
{
    if (player.state() == QMediaPlayer::StoppedState){
        player.play();
    }
}

void AudioSourceFile::setSource(QString path)
{
    LOG.addLog(u8"オープン（ファイル） " + path, this);
    bufferReady = false;

    player.setMedia(QUrl::fromLocalFile(path));
    player.play();

    this->path = path;
    qDebug() << path;

    LOG.addLog(u8"デコード開始", this);
    if (decoder.state() == QAudioDecoder::DecodingState){
        decoder.stop();
    }
    rawSamples.clear();
    decoder.setSourceFilename(path);
    qDebug() << path;
    decoder.start();
}

void AudioSourceFile::notifyDecodeError(QAudioDecoder::Error error){
    LOG.addLog(u8"デコードエラー " + decoder.errorString(), this);
    qDebug() << "decode error";
    qDebug() << decoder.duration();
    qDebug() << decoder.errorString();
}

void AudioSourceFile::readDecodedAudioBuffer(){
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

void AudioSourceFile::finalizeDecode()
{
    LOG.addLog(u8"デコード終了", this);
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
