#include "settings.h"

Settings::Settings(){

}

Settings &Settings::getInstance(){
    static Settings settings;

    return settings;
}

QAudioFormat Settings::getFormat()
{
    QAudioFormat format;
    format.setCodec(codec);
    format.setSampleRate(sampleRate);
    format.setChannelCount(channels);
    format.setSampleSize(sampleSize);
    format.setSampleType(sampleType);
    format.setByteOrder(sampleEndian);

    return format;
}
