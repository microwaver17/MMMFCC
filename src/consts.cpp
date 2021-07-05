#include "consts.h"

QAudioFormat Consts::getFormat()
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
