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

void RuntimeParameters::addOption(const QString& optionName, QCommandLineOption* option)
{
    _parser.addOption(*option);
    _options[optionName] = option; // this is need for possible default values
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
