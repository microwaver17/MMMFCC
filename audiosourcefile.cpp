#include "audiosourcefile.h"
#include "log.h"
#include "status.h"

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

    player.setNotifyInterval(SETTINGS.playerTimeUnit);
    connect(&player, &QMediaPlayer::stateChanged, this, &AudioSourceFile::playAgain);

    Status::getInstance().setState(Status::Subject::PlayerReady, false);
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

    LOG.addLog(u8"デコード開始", this);
    if (decoder.state() == QAudioDecoder::DecodingState){
        decoder.stop();
    }
    rawSamples.clear();
    decoder.setSourceFilename(path);
    decoder.start();
    Status::getInstance().setState(Status::Subject::Decoder, true);
    Status::getInstance().setState(Status::Subject::PlayerReady, true);
}

void AudioSourceFile::notifyDecodeError(QAudioDecoder::Error error){
    LOG.addLog(u8"デコードエラー " + decoder.errorString(), this);
    Status::getInstance().setState(Status::Subject::Decoder, Status::State::Secondary);
}

void AudioSourceFile::readDecodedAudioBuffer(){
    QMutexLocker locker(&rawSamplesMutex);

    QAudioBuffer buffer = decoder.read();
    const qint16 *data = buffer.constData<qint16>();
    for (int i = 0; i < buffer.sampleCount(); i++){
        rawSamples.append(data[i]);
    }
}

void AudioSourceFile::finalizeDecode()
{
    LOG.addLog(u8"デコード終了", this);

    // 正しく読み込めたかテストするために書き出す
    // ffmpeg -f s16le -ar 48000 -ac 1 -i ***.raw test.wav
    // みたいにすると聞けるファイルができる
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
    Status::getInstance().setState(Status::Subject::Decoder, Status::State::Secondary);
}
