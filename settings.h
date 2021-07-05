#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS_INT(key) (Settings::getInstance().getSettingItem(key).value.toInt())
#define SETTINGS_DOUBLE(key) (Settings::getInstance().getSettingItem(key).value.toDouble())
#define SETTINGS_BOOL(key) (Settings::getInstance().getSettingItem(key).value.toBool())
#define SETTINGS_SET(key, value) (Settings::getInstance().setValue((key), (value)))

#include <QObject>
#include <QAudioFormat>
#include <QCoreApplication>
#include <QMap>
#include <QVector>
#include <QVariant>

class SettingItem{
public:
    SettingItem(QString key="", QVariant value=QVariant(), QVariant defaultValue=QVariant(), QString type="", QString displayName="undefined");

    QString key;
    QVariant value;
    QVariant defaultValue;
    QString type;
    QString displayName;
    int order;

private:
    static int itemCount;
};

bool operator<(const SettingItem &item1, const SettingItem &item2);
bool operator<=(const SettingItem &item1, const SettingItem &item2);
bool operator>(const SettingItem &item1, const SettingItem &item2);
bool operator>=(const SettingItem &item1, const SettingItem &item2);

class SettingKeys {
public:
    static constexpr char windowLength[] = "windowLength";
    static constexpr char filterNumber[] = "filterNumber";
    static constexpr char cepstramNumber[] = "cepstramNumber";
    static constexpr char maxFreq[] = "maxFreq";
    static constexpr char minFreq[] = "minFreq";
    static constexpr char fps[] = "fps";
    static constexpr char scale_multiple[] = "scale_multiple";
    static constexpr char default_scale[] = "default_scale";
    static constexpr char isAutoScale[] = "isAutoScale";
    static constexpr char movingAverageSize[] = "movingAverageSize";
};

class Settings : QObject{

public:
    static Settings &getInstance();
    void setValue(QString key, QVariant value);
    QList<QString> getKeys();
    SettingItem getSettingItem(QString key);

    void save();
    void load();

private:
    Settings();

    inline QString getSettingsPsth(){
        return QCoreApplication::applicationDirPath() + "/settings.ini";
    }

    QMap<QString, SettingItem> settings = {
        { "windowLength",       SettingItem("windowLength", QVariant(), windowLength, "int", u8"[MFCC] 窓幅（ms）") },
        { "filterNumber",       SettingItem("filterNumber", QVariant(), filterNumber, "int", u8"[MFCC] フィルタバンク数") },
        { "cepstramNumber",     SettingItem("cepstramNumber", QVariant(), cepstramNumber, "int", u8"[MFCC] 表示する次元数") },
        { "maxFreq",            SettingItem("maxFreq", QVariant(), maxFreq, "int", u8"[MFCC] 最大周波数（LPF）") },
        { "minFreq",            SettingItem("minFreq", QVariant(), minFreq, "int", u8"[MFCC] 最小周波数（HPF）") },
        { "fps",                SettingItem("fps", QVariant(), fps, "int", u8"[グラフ] 画面更新するFPS") },
        { "scale_multiple",     SettingItem("scale_multiple", QVariant(), scale_multiple, "double", u8"[グラフ] 最大拡大率") },
        { "default_scale",      SettingItem("default_scale", QVariant(), default_scale, "double", u8"[グラフ] デフォルト拡大率（0.0 - 1.0）") },
        { "isAutoScale",        SettingItem("isAutoScale", QVariant(), isAutoScale, "bool", u8"[グラフ] 自動的に拡大") },
        { "movingAverageSize",  SettingItem("movingAverageSize", QVariant(), movingAverageSize, "int", u8"[グラフ] 移動平均のサンプル数") },
    };

    // デフォルト値

    // MFCC
    static constexpr int windowLength = 25;
    static constexpr int filterNumber = 24;      // 圧縮するスペクト次元
    static constexpr int cepstramNumber = 24;    // 表示するスペクトル次元
    static constexpr int maxFreq = 6500;
    static constexpr int minFreq = 50;

    // 画面表示系
    static constexpr int fps = 30;
    static constexpr double scale_multiple = 500;
    static constexpr double default_scale = 0.15;
    static constexpr bool isAutoScale = true;
    static constexpr int movingAverageSize = fps / 5;
};
#endif // SETTINGS_H
