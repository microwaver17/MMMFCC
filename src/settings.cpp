#include "settings.h"

#include <QSettings>
#include <QFileInfo>
#include <stdexcept>
#include <QMessageBox>
#include <QtDebug>

int SettingItem::itemCount = 0;

SettingItem::SettingItem(QString key, QVariant value, QVariant defaultValue, QString type, QString displayName) :
   key(key)
  , value(value)
  , defaultValue(defaultValue)
  , type(type)
  , displayName(displayName)
  , order(itemCount++)
{
    if (!value.isValid()){
        value = defaultValue;
    }
}

SettingItem::SettingItem(const SettingItem &item)
{
    key = item.key;
    value = item.value;
    defaultValue = item.defaultValue;
    type = item.type;
    displayName = item.displayName;
    order = item.order;

    if (!value.isValid()){
        value = defaultValue;
    }
}

bool operator<(const SettingItem &item1, const SettingItem &item2)
{
    return item1.order < item2.order;
}
bool operator<=(const SettingItem &item1, const SettingItem &item2)
{
    return item1.order <= item2.order;
}
bool operator>(const SettingItem &item1, const SettingItem &item2)
{
    return item1.order > item2.order;
}
bool operator>=(const SettingItem &item1, const SettingItem &item2)
{
    return item1.order >= item2.order;
}

Settings::Settings()
{
    settings = {
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
    auto s = settings;
    load();
    s = settings;
    save();
    s = settings;
}

Settings &Settings::getInstance()
{
    static Settings settings;

    return settings;
}

void Settings::load()
{
    QString path = getSettingsPsth();
    if (QFileInfo::exists(path) == false){
        return;
    }
    QSettings ini(path, QSettings::IniFormat);

    auto keys = settings.keys();
    for (auto key = keys.begin(); key != keys.end(); key++ ){
        QVariant value = ini.value(*key);
        if (value.isValid()){
            setValue(*key, value);
        }
    }
}

void Settings::save()
{
    QString path = getSettingsPsth();
    QSettings ini(path, QSettings::IniFormat);

    auto keys = settings.keys();
    for (auto key = keys.begin(); key != keys.end(); key++ ){
        ini.setValue(*key, getSettingItem(*key).value);
    }
    ini.sync();
}

void error(QString msg){
    QMessageBox::warning(nullptr, u8"設定エラー", msg);
    qDebug() << "settings error: " << msg;
}

QList<QString> Settings::getKeys()
{
    return settings.keys();
}

SettingItem Settings::getSettingItem(QString key)
{
    if (!settings.contains(key)){
        error(QString(u8"存在しない項目（値）を取得しようとしました [%1]").arg(key));
        return SettingItem();
    }

    return settings[key];
}

void Settings::setValue(QString key, QVariant value)
{
    if (!settings.contains(key)){
        error(QString(u8"存在しない項目に設定しようとしました [%1]").arg(key));
        return;
    }

    QString type = settings[key].type;
    QVariant newValue;
    if (type == "int"){
        newValue = value.toInt();
    }else if (type == "double"){
        newValue = value.toDouble();
    }else if (type == "bool"){
        newValue = value.toBool();
    }else{
        error(QString(u8"未対応の型を使用しようとしました %1 [%2]").arg(type, key));
        return;
    }

    settings[key].value = newValue;
}
