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
