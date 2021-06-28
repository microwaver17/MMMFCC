#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS (Settings::getInstance())

#include <QObject>
#include <QAudioFormat>
#include <QCoreApplication>

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
    int filterNumber = 24;      // 圧縮するスペクト次元
    int cepstramNumber = 12 * 2;    // 表示するスペクトル次元
    int maxFreq = 6500;
    int minFreq = 50;

    // 画面表示系
    int fps = 30;
    double scale_multiple = 100;
    double default_scale = 0.35;
    bool isAutoScale = true;
    int movingAverageSize = fps / 5;

    int playerTimeUnit = 100;

private:
    Settings();
    inline QString getSettingsPsth(){
        return QCoreApplication::applicationDirPath() + "/settings.ini";
    }
    void save();
    void load();

};
#endif // SETTINGS_H
