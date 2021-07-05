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
    SettingItem(const SettingItem &item);

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

    QMap<QString, SettingItem> settings;

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
