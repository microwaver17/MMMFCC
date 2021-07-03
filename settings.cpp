#include "settings.h"

#include <QSettings>
#include <QFileInfo>
#include <stdexcept>
#include <QMessageBox>

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
            settings[*key] = QVariant(value);
        }
    }
}

void Settings::save()
{
    QString path = getSettingsPsth();
    QSettings ini(path, QSettings::IniFormat);

    auto keys = settings.keys();
    for (auto key = keys.begin(); key != keys.end(); key++ ){
        ini.setValue(*key, settings[*key]);
    }
    ini.sync();
}

void error(QString msg){
    QMessageBox::warning(nullptr, "Settings", msg);
    throw std::runtime_error(msg.toStdString());
}

QVariant Settings::getValue(QString key)
{
    if (!settings.contains(key)){
        error("invalid setting key [" + key + "]");
    }

    return settings[key];
}

void Settings::setValue(QString key, QVariant value)
{
    if (!settings.contains(key)){
        error("[setting] invalid key [" + key + "]");
    }
    if (settings_type[key] != value.typeName()){
        error("[setting] wrong type [" + QString(value.typeName()) + "]");
    }
    settings[key] = value;
}

