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
 
 #include "common/runtimeparameters.h"

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

#define LOG_AREA "RuntimeParameters"
#include "log/log.h"

namespace GBerryConsoleCommon {

RuntimeParameters::RuntimeParameters(IEnvironmentVariables& env) :
    _env(env)
{
    _parser.addHelpOption();
}

RuntimeParameters::~RuntimeParameters()
{
    foreach (auto option, _options) {
        delete option;
    }
}

RuntimeParameters& RuntimeParameters::parse(const QStringList& args)
{
    _parser.process(args);
    return *this;
}

bool RuntimeParameters::isSet(const QString &optionName) const
{
    if (_options.contains(optionName) && _parser.isSet(*_options[optionName]))
        return true;

    if (_envVarNames.contains(optionName) && _env.contains(_envVarNames[optionName]))
        return true;

    if (_buildTimeDefaults.contains(optionName))
        return true;

    return false;
}

QString RuntimeParameters::value(const QString &optionName) const
{
    if (!_options.contains(optionName)) {
        WARN("Queried unknown option" << optionName);
        return "";
    }

    if (_parser.isSet(optionName)) {
        return _parser.value(optionName);
    }
    if (_envVarNames.contains(optionName)) {
        if (_env.contains(_envVarNames[optionName])) {
            return _env.value(_envVarNames[optionName]);
        }
    }
    if (_buildTimeDefaults.contains(optionName)) {
        return _buildTimeDefaults[optionName];
    }
    if (!_options[optionName]->defaultValues().isEmpty()) {
        return _options[optionName]->defaultValues()[0];
    }

    return _parser.value(optionName);
}

QCommandLineParser& RuntimeParameters::parser()
{
    return _parser;
}

void RuntimeParameters::addBooleanOption(const QString& optionName, QCommandLineOption* option)
{
    _parser.addOption(*option);
    _options[optionName] = option; // this is need for possible default values
    if (!option->valueName().isEmpty()) {
        ERROR("Option" << optionName << "marked bool option but value name was defined.");
    }
}

void RuntimeParameters::addValueOption(const QString& optionName, QCommandLineOption* option)
{
    _parser.addOption(*option);
    _options[optionName] = option; // this is need for possible default values
    if (option->valueName().isEmpty()) {
        ERROR("Option" << optionName << "marked value option but no value name defined.");
    }
}

void RuntimeParameters::addBuildTimeDefault(const QString& optionName, const QString& defaultValue)
{
    if (!_buildTimeDefaults.contains(optionName)) {
        _buildTimeDefaults[optionName] = defaultValue;
    } else {
        WARN("Build time default for option" << optionName << "defined second time. Discarding the second definition");
    }
}

void RuntimeParameters::addEnvironmentVariable(const QString& optionName, const QString& envVarName)
{
    if (!_envVarNames.contains(optionName)) {
        _envVarNames[optionName] = envVarName;
    } else {
        WARN("Environment variable for pption" << optionName << "defined second time. Discarding the second definition");
    }
}

} // eon
