#ifndef CONSTS_H
#define CONSTS_H

#include <QObject>
#include <QAudioFormat>

class Consts{

private:
    Consts();

public:
    // デコード形式
    constexpr static char codec[] = "audio/pcm";
    constexpr static int sampleRate = 44100;
    constexpr static int channels = 1;
    constexpr static int sampleSize = 16;
    constexpr static int windowLength = 25;
    constexpr static QAudioFormat::SampleType sampleType = QAudioFormat::SampleType::SignedInt;
    constexpr static QAudioFormat::Endian sampleEndian = QAudioFormat::Endian::LittleEndian;

    constexpr static int fps = 60;
};
#endif // CONSTS_H
