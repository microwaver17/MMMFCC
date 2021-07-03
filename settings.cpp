#include "settings.h"

#include <QSettings>
#include <QFileInfo>
#include <stdexcept>
#include <QMessageBox>

SettingItem::SettingItem()
{

}

SettingItem::SettingItem(QString key, QVariant value, QString type, QString displayName)
{
    this->key = key;
    this->value = value;
    this->type = type;
    this->displayName = displayName;
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
        ini.setValue(*key, getValue(*key));
    }
    ini.sync();
}

void error(QString msg){
    QMessageBox::warning(nullptr, "設定エラー", msg);
    throw std::runtime_error(msg.toStdString());
}

QVariant Settings::getValue(QString key)
{
    if (!settings.contains(key)){
        error(QString(u8"存在しない項目を取得しようとしました [%1]").arg(key));
        return QVariant();
    }

    return settings[key].value;
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
        error(QString(u8"未対応の型の値を設定しようとしました %1 [%2]").arg(type, key));
        return;
    }

    settings[key].value = newValue;
}
