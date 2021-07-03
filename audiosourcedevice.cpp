#include "audiosourcedevice.h"
#include "settings.h"
#include "log.h"
#include "consts.h"

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QtDebug>
#include <QFile>

AudioSourceDevice::AudioSourceDevice(QObject *parent) : QObject(parent)
  , audioInput(nullptr)
  , format(Consts::getFormat())
{
}

AudioSourceDevice::~AudioSourceDevice()
{
    delete audioInput;
}

QVector<qint16> AudioSourceDevice::getRawSamples(int fromSamples)
{
    QMutexLocker locker(&rawSamplesMutex);
    if (fromSamples > rawSamples.size()){
        return QVector<qint16>();
    }

    return rawSamples.mid(fromSamples, SETTINGS_INT("windowLength") * Consts::sampleRate);
}

void AudioSourceDevice::setSource(QAudioDeviceInfo info)
{
    if (!info.isFormatSupported(format)){
//        QAudioFormat new_format = info.nearestFormat(format);

//        if (new_format.channelCount() != 1 || new_format.sampleSize() != Consts::sampleSize){
//            LOG.addLog(u8"フォーマットが未対応", this);
//            return;
//        }
        LOG.addLog(u8"未対応のフォーマット（デバイス） " + info.deviceName(), this);
        return;
    }

    delete audioInput;
    audioInput = new QAudioInput(info, format, this);
    audioInput->setNotifyInterval(SETTINGS_INT("windowLength"));
    audioDevice = audioInput->start();
    connect(audioInput, &QAudioInput::notify, this, &AudioSourceDevice::readAudioBuffer);

    LOG.addLog(u8"オープン（デバイス） " + info.deviceName(), this);
}


void AudioSourceDevice::readAudioBuffer()
{
    QMutexLocker locker(&rawSamplesMutex);

    QByteArray bytes = audioDevice->readAll();
//    // MFCCのウィンドウ分の長さが溜まっているか
//    if (bytes.size() == 0){
//        return;
//    }

    rawSamples.clear();
    for (int i = 0; i < bytes.size() / 2; i++){
        char buf[2];
        buf[0] = bytes.at(i * 2);
        buf[1] = bytes.at(i * 2 + 1);
        qint16 val = *(qint16*)buf;
        rawSamples.append(val);
    }

    // 正しく読み込めたかテストするために書き出す
    // ffmpeg -f s16le -ar 48000 -ac 1 -i ***.raw test.wav
    // みたいにすると聞けるファイルができる
    /*
    QFile file("test.raw");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
       for (int i = 0; i < rawSamples.size(); i++){
           const char *data = (char*)&(rawSamples.at(i));
           file.write(data, 2);
       }
       file.close();
    }
    */

}

