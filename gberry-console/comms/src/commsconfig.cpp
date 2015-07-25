/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
    QFileInfo configFile(configFilePath);
    if (!configFile.exists()) {
        WARN("Can't find comms configuration file from" << configFilePath);
        _settings = new QSettings();

    } else if (!configFile.isFile()) {
        WARN("Given comms configuration path is not file:" << configFilePath);
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
