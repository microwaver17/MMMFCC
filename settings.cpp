#include "settings.h"

#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>

Settings::Settings(){
    load();
}

void Settings::load(){
    QString path = QCoreApplication::applicationDirPath() + "/settings.ini";
    if (QFileInfo::exists(path) == false){
        save();
        return;
    }
    QSettings ini(path, QSettings::IniFormat);

    windowLength = ini.value("windowLength").toInt();
    cepstramNumber = ini.value("cepstramNumber").toInt();
    fps = ini.value("fps").toInt();
    scale_multiple = ini.value("scale_multiple").toDouble();
    default_scale = ini.value("default_scale").toDouble();
    isAutoScale = ini.value("isAutoScale").toBool();
    movingAverageSize = ini.value("movingAverageSize").toInt();
}

void Settings::save(){
    QString path = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings ini(path, QSettings::IniFormat);

    ini.setValue("windowLength", windowLength);
    ini.setValue("cepstramNumber", cepstramNumber);
    ini.setValue("fps", fps);
    ini.setValue("scale_multiple", scale_multiple);
    ini.setValue("default_scale", default_scale);
    ini.setValue("isAutoScale", isAutoScale);
    ini.setValue("movingAverageSize", movingAverageSize);
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
