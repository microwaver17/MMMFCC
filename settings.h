#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS_INT(key) (Settings::getInstance().getSettingItem(key).value.toInt())
#define SETTINGS_DOUBLE(key) (Settings::getInstance().getSettingItem(key).value.toDouble())
#define SETTINGS_BOOL(key) (Settings::getInstance().getSettingItem(key).value.toBool())
#define SETTINGS_SET(key, value) (Settings::getInstance().setValue(key, value))

#include <QObject>
#include <QAudioFormat>
#include <QCoreApplication>
#include <QMap>
#include <QVector>
#include <QVariant>

class SettingItem{
public:
    SettingItem();
    SettingItem(QString key, QVariant defaultValue, QString type, QString displayName);

    QString key;
    QVariant value;
    QVariant defaultValue;
    QString type;
    QString displayName;
};

class Settings : QObject{

public:
    static Settings &getInstance();
    void setValue(QString key, QVariant value);
    QList<QString> getKeys();
    SettingItem getSettingItem(QString key);


private:
    Settings();

    inline QString getSettingsPsth(){
        return QCoreApplication::applicationDirPath() + "/settings.ini";
    }
    void save();
    void load();

    QMap<QString, SettingItem> settings = {
        { "windowLength",       SettingItem("windowLength", windowLength, "int", "windowLength") },
        { "filterNumber",       SettingItem("filterNumber", filterNumber, "int", "filterNumber") },
        { "cepstramNumber",     SettingItem("cepstramNumber", cepstramNumber, "int", "cepstramNumber") },
        { "maxFreq",            SettingItem("maxFreq", maxFreq, "int", "maxFreq") },
        { "minFreq",            SettingItem("minFreq", minFreq, "int", "minFreq") },
        { "fps",                SettingItem("fps", fps, "int", "fps") },
        { "scale_multiple",     SettingItem("scale_multiple", scale_multiple, "double", "scale_multiple") },
        { "default_scale",      SettingItem("default_scale", default_scale, "double", "default_scale") },
        { "isAutoScale",        SettingItem("isAutoScale", isAutoScale, "bool", "isAutoScale") },
        { "movingAverageSize",  SettingItem("movingAverageSize", movingAverageSize, "int", "movingAverageSize") },
    };

    // MFCC
    static constexpr int windowLength = 25;
    static constexpr int filterNumber = 24;      // 圧縮するスペクト次元
    static constexpr int cepstramNumber = 12 * 2;    // 表示するスペクトル次元
    static constexpr int maxFreq = 6500;
    static constexpr int minFreq = 50;

    // 画面表示系
    static constexpr int fps = 30;
    static constexpr double scale_multiple = 100;
    static constexpr double default_scale = 0.35;
    static constexpr bool isAutoScale = true;
    static constexpr int movingAverageSize = fps / 5;
};
#endif // SETTINGS_H
