#include "settings.h"

#include <QSettings>
#include <QFileInfo>
#include <stdexcept>
#include <QMessageBox>

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
    load();
    save();
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
    QMessageBox::warning(nullptr, "設定エラー", msg);
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
