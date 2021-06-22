#include "audiosourcedevice.h"
#include "consts.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QtDebug>
#include <QFile>

AudioSourceDevice::AudioSourceDevice(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setCodec(Consts::codec);
    format.setSampleRate(Consts::sampleRate);
    format.setChannelCount(Consts::channels);
    format.setSampleSize(Consts::sampleSize);
    format.setSampleType(Consts::sampleType);
    format.setByteOrder(Consts::sampleEndian);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << info.deviceName();
    audioInput = new QAudioInput(info, format, this);
    audioInput->setNotifyInterval(Consts::windowLength);
    connect(audioInput, &QAudioInput::notify, this, &AudioSourceDevice::readRawSamples);

    audioDevice = audioInput->start();
}

QVector<qint16> AudioSourceDevice::getRawSamples()
{
    QMutexLocker locker(&rawSamplesMutex);
    return rawSamples;
}

void AudioSourceDevice::readRawSamples()
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
