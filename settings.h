#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS (Settings::getInstance())

#include <QObject>
#include <QAudioFormat>

class Settings{

public:
    static Settings &getInstance();

    // デコード形式
    char codec[64] = "audio/pcm";
    int sampleRate = 48000;
    int channels = 1;
    int sampleSize = 16;
    QAudioFormat::SampleType sampleType = QAudioFormat::SampleType::SignedInt;
    QAudioFormat::Endian sampleEndian = QAudioFormat::Endian::LittleEndian;

    QAudioFormat getFormat();

    // MFCC
    int windowLength = 25;
    int cepstramNumber = 12;

    // 画面表示系
    int fps = 30;
    double scale_multiple = 100;
    double default_scale = 0.35;
    bool isAutoScale = true;
    int movingAverageSize = fps / 5;

private:
    Settings();
    void save();
    void load();

};
#endif // SETTINGS_H
