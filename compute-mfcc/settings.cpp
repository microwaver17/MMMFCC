#include "settings.h"

Settings::Settings()
{

}

Settings &Settings::getInstance()
{
    static Settings settings;

    return settings;
}
