#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS (Settings::getInstance())

#include <QObject>
#include <QAudioFormat>

class Settings{

public:
    static Settings &getInstance();

    // デコード形式
    const char codec[64] = "audio/pcm";
    const int sampleRate = 48000;
    const int channels = 1;
    const int sampleSize = 16;
    const int windowLength = 25;
    const QAudioFormat::SampleType sampleType = QAudioFormat::SampleType::SignedInt;
    const QAudioFormat::Endian sampleEndian = QAudioFormat::Endian::LittleEndian;
    QAudioFormat getFormat();

    // 画面表示系
    const int fps = 30;
    const double scale_multiple = 100;
    const double default_scale = 0.35;
    const bool isAutoScale = true;
    const int lineSize = 2;
    const int movingAverageSize = fps / 2;
    const int defaultGraphSizeX = 1000;
    const int defaultGraphSizeY = 800;

private:
    Settings();

};
#endif // SETTINGS_H
