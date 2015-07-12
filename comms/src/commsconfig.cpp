#include "commsconfig.h"

#include <QCoreApplication>
#include <QFile>
#include <QSettings>

#include "utils/fileutils.h"

#define LOG_AREA "CommsConfig"
#include "log/log.h"

namespace GBerryComms {

// TODO: should we use ref instead of pointer to indicate we are not storing any reference?
CommsConfig::CommsConfig(CommsParameters* parameters)
{
    QString configFilePath;

    if (!parameters->isSet(CommsParameters::COMMS_CONFIG)) {
        // look next to binary
        configFilePath = GBerryLib::joinpath(QCoreApplication::applicationDirPath(), "comms.ini");
    } else {
        // config path defined externally
        // relative paths relative to current working dir
        configFilePath = parameters->value(CommsParameters::COMMS_CONFIG);
    }

    if (!QFile::exists(configFilePath)) {
        WARN("Can't find comms configuration file from" << configFilePath);
        _settings = new QSettings();
    } else {
        _settings = new QSettings(configFilePath, QSettings::IniFormat);
    }
}

CommsConfig::~CommsConfig()
{
    delete _settings;
}

bool CommsConfig::hasKey(const QString &key) const
{
    return _settings->contains(key);
}

QString CommsConfig::value(const QString &key) const
{
    return _settings->value(key).toString();
}

QString CommsConfig::value(const QString &key, const QString &defaultValue) const
{
    if (hasKey(key))
        return value(key);
    else
        return defaultValue;
}

} // eon
