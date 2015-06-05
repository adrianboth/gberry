#include "cmdlineparams.h"

#define LOG_AREA "CmdLineParams"
#include "log/log.h"

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

namespace Comms
{

const QString CmdLineParams::ROOT_PATH = QString("root-path");
const QString CmdLineParams::DISABLE_WAITAPP = QString("disable-wait-ui");
const QString CmdLineParams::DISABLE_MAINUI = QString("disable-main-ui");

CmdLineParams::CmdLineParams(IEnvironmentVariables& env) :
    _env(env)
{
    _parser.setApplicationDescription("Communication manager of GBerry game platform");
    _parser.addHelpOption();
    _parser.addVersionOption();

    addOption(DISABLE_WAITAPP, new QCommandLineOption(DISABLE_WAITAPP, "Disables showing waiting application"));
    addOption(DISABLE_MAINUI,  new QCommandLineOption(DISABLE_MAINUI, "Disables showing main application"));

    addOption(ROOT_PATH, new QCommandLineOption(ROOT_PATH, "Defines used root path", "rootpath"));
    addBuildTimeDefault(ROOT_PATH, xstr(GBERRY_ROOT_PATH));
    addEnvironmentVariable(ROOT_PATH, "GBERRY_ROOT_PATH");
}

CmdLineParams::~CmdLineParams()
{
    foreach (auto option, _options) {
        delete option;
    }
}

CmdLineParams& CmdLineParams::parse(const QStringList& args)
{
    _parser.process(args);
    return *this;
}

bool CmdLineParams::isSet(const QString &optionName) const
{
    return _parser.isSet(optionName) || _envVarNames.contains(optionName) || _buildTimeDefaults.contains(optionName);
}

QString CmdLineParams::value(const QString &optionName) const
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

void CmdLineParams::addOption(const QString& optionName, QCommandLineOption* option)
{
    _parser.addOption(*option);
    _options[optionName] = option; // this is need for possible default values
}

void CmdLineParams::addBuildTimeDefault(const QString& optionName, const QString& defaultValue)
{
    if (!_buildTimeDefaults.contains(optionName)) {
        _buildTimeDefaults[optionName] = defaultValue;
    } else {
        WARN("Build time default for option" << optionName << "defined second time. Discarding the second definition");
    }
}

void CmdLineParams::addEnvironmentVariable(const QString& optionName, const QString& envVarName)
{
    if (!_envVarNames.contains(optionName)) {
        _envVarNames[optionName] = envVarName;
    } else {
        WARN("Environment variable for pption" << optionName << "defined second time. Discarding the second definition");
    }
}

} // eon
