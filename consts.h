#ifndef CONSTS_H
#define CONSTS_H

#include <QAudioFormat>

class Consts
{
private:
    Consts();

public:
    static constexpr char appTitle[] = "MMMFCC - Realtime MFCC Analyzer";;

    // デコード形式
    static constexpr char codec[] = "audio/pcm";
    static constexpr int sampleRate = 48000;
    static constexpr int channels = 1;
    static constexpr int sampleSize = 16;
    static constexpr QAudioFormat::SampleType sampleType = QAudioFormat::SampleType::SignedInt;
    static constexpr QAudioFormat::Endian sampleEndian = QAudioFormat::Endian::LittleEndian;

    static QAudioFormat getFormat();

    static constexpr int playerTimeUnit = 100;

};

#endif // CONSTS_H
