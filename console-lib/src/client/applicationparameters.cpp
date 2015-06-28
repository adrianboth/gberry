#include "applicationparameters.h"

namespace GBerryApplication {

const QString ApplicationParameters::APPLICATION_CODE("application-code");

ApplicationParameters::ApplicationParameters(IEnvironmentVariables& env) :
    GBerryConsoleCommon::RuntimeParameters(env)
{
    // TODO: info for these should be read from appcfg.json
    //_parser.setApplicationDescription("Communication manager of GBerry game platform");
    //_parser.addVersionOption()

    addOption(APPLICATION_CODE, new QCommandLineOption(APPLICATION_CODE, "Defines uses application code identification", "application_code"));
    addEnvironmentVariable(APPLICATION_CODE, "GBERRY_APPLICATION_CODE");
}

ApplicationParameters::~ApplicationParameters()
{
}

} // eon
