#include "audiosourcedevice.h"
#include "consts.h"

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QtDebug>
#include <QFile>

AudioSourceDevice::AudioSourceDevice(QObject *parent) : AbstractAudioSource(parent)
  , audioInput(nullptr)
{
}

AudioSourceDevice::~AudioSourceDevice()
{
    delete audioInput;
}

void AudioSourceDevice::setSource(QAudioDeviceInfo info)
{
    info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)){
        QAudioFormat new_format = info.nearestFormat(format);
        if (new_format.channelCount() != 1 || new_format.sampleSize() != Consts::sampleSize){
            qDebug() << "unsupported format";
            return;
        }
    }
    qDebug() << info.deviceName();

    delete audioInput;
    audioInput = new QAudioInput(info, format, this);
    audioInput->setNotifyInterval(Consts::windowLength * 1.5);
    audioDevice = audioInput->start();
    connect(audioInput, &QAudioInput::notify, this, &AudioSourceDevice::readAudioBuffer);
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

