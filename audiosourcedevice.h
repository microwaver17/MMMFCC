#ifndef AUDIOSOURCEDEVICE_H
#define AUDIOSOURCEDEVICE_H

#include "abstractaudiosource.h"

#include <QObject>
#include <QAudioInput>
#include <QVector>
#include <QMutex>

class AudioSourceDevice : public AbstractAudioSource
{
    Q_OBJECT
public:
    explicit AudioSourceDevice(QObject *parent = nullptr);
    ~AudioSourceDevice();
    void setSource(QAudioDeviceInfo info);

private:
    QAudioInput *audioInput;
    QIODevice *audioDevice;

private slots:
    void readAudioBuffer();
};

#endif // AUDIOSOURCEDEVICE_H
