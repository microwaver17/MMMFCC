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
    QVector<qint16> getRawSamples();

private:
    QAudioInput *audioInput;
    QIODevice *audioDevice;
    QVector<qint16> rawSamples;
    QMutex rawSamplesMutex;

signals:
    void audioReady();

private slots:
    void readRawSamples();

signals:

};

#endif // AUDIOSOURCEDEVICE_H
