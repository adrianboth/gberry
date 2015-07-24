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
 
 #ifndef RUNTIMEPARAMETERS_H
#define RUNTIMEPARAMETERS_H

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QMap>

#include "systemservices.h"

namespace GBerryConsoleCommon
{

/**
 * @brief Parameter handler
 *
 * Handles parameters for 'comms' application from various sources:
 *  - command line
 *  - environment variables
 *  - built time
 *
 * Contains a generic approach but also option specific handling.
 *
 * Typical order of predescence (top most effective if defined):
 *  - command line
 *  - environment variable
 *  - build time default
 *
 */
class RuntimeParameters
{
public:
    explicit RuntimeParameters(IEnvironmentVariables& env);
    virtual ~RuntimeParameters();

    RuntimeParameters& parse(const QStringList& args);

    bool isSet(const QString& optionName) const;
    QString value(const QString& optionName) const;

protected:
    QCommandLineParser& parser();
    void addBooleanOption(const QString& optionName, QCommandLineOption* option);
    void addValueOption(const QString& optionName, QCommandLineOption* option);
    void addBuildTimeDefault(const QString& optionName, const QString& defaultValue);
    void addEnvironmentVariable(const QString& optionName, const QString& envVarName);

private:
    IEnvironmentVariables& _env;
    QCommandLineParser _parser;
    QMap<QString, QCommandLineOption*> _options;
    QMap<QString, QString> _buildTimeDefaults;
    QMap<QString, QString> _envVarNames;

};

} // eon

#endif // RUNTIMEPARAMETERS_H
