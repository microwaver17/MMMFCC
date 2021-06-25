#ifndef AUDIOSOURCEDEVICE_H
#define AUDIOSOURCEDEVICE_H

#include <QObject>
#include <QAudioInput>
#include <QVector>
#include <QMutex>

class AudioSourceDevice : public QObject
{
    Q_OBJECT
public:
    explicit AudioSourceDevice(QObject *parent = nullptr);
    ~AudioSourceDevice();

    QVector<qint16> getRawSamples(int fromSamples);
    void setSource(QAudioDeviceInfo info);

private:
    QAudioInput *audioInput;
    QIODevice *audioDevice;
    QVector<qint16> rawSamples;
    QMutex rawSamplesMutex;
    QAudioFormat format;

private slots:
    void readAudioBuffer();
};

#endif // AUDIOSOURCEDEVICE_H
