#include "settings.h"

#include <QSettings>
#include <QFileInfo>

Settings::Settings(){
    load();
    save();
}

void Settings::load(){
    QString path = getSettingsPsth();
    if (QFileInfo::exists(path) == false){
        return;
    }
    QSettings ini(path, QSettings::IniFormat);

    windowLength = ini.value("windowLength", windowLength).toInt();
    cepstramNumber = ini.value("cepstramNumber", cepstramNumber).toInt();
    fps = ini.value("fps", fps).toInt();
    scale_multiple = ini.value("scale_multiple", scale_multiple).toDouble();
    default_scale = ini.value("default_scale", default_scale).toDouble();
    isAutoScale = ini.value("isAutoScale", isAutoScale).toBool();
    movingAverageSize = ini.value("movingAverageSize", movingAverageSize).toInt();
    filterNumber = ini.value("filterNumber", filterNumber).toInt();
}

void Settings::save(){
    QString path = getSettingsPsth();
    QSettings ini(path, QSettings::IniFormat);

    ini.setValue("windowLength", windowLength);
    ini.setValue("cepstramNumber", cepstramNumber);
    ini.setValue("fps", fps);
    ini.setValue("scale_multiple", scale_multiple);
    ini.setValue("default_scale", default_scale);
    ini.setValue("isAutoScale", isAutoScale);
    ini.setValue("movingAverageSize", movingAverageSize);
    ini.setValue("filterNumber", filterNumber);
    ini.sync();
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
