#include "commsparameters.h"

#define LOG_AREA "CommsParameters"
#include "log/log.h"

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

namespace Comms
{

const QString CommsParameters::ROOT_PATH = QString("root-path");
const QString CommsParameters::DISABLE_WAITAPP = QString("disable-wait-ui");
const QString CommsParameters::DISABLE_MAINUI = QString("disable-main-ui");
const QString CommsParameters::COMMS_CONFIG = QString("comms-config");

CommsParameters::CommsParameters(IEnvironmentVariables& env) :
    GBerryConsoleCommon::RuntimeParameters(env)
{
    parser().setApplicationDescription("Communication manager of GBerry game platform");
    parser().addVersionOption();

    addBooleanOption(DISABLE_WAITAPP, new QCommandLineOption(DISABLE_WAITAPP, "Disables showing waiting application"));
    addBooleanOption(DISABLE_MAINUI,  new QCommandLineOption(DISABLE_MAINUI, "Disables showing main application"));

    addValueOption(ROOT_PATH, new QCommandLineOption(ROOT_PATH, "Defines used root path", "rootpath"));
    addBuildTimeDefault(ROOT_PATH, xstr(GBERRY_ROOT_PATH));
    addEnvironmentVariable(ROOT_PATH, "GBERRY_ROOT_PATH");

    // TODO: should value options add*() have different signature? Do we need value name to anything?
    addValueOption(COMMS_CONFIG, new QCommandLineOption(COMMS_CONFIG, "Defines used configuration file", "commsconfig"));
    addEnvironmentVariable(COMMS_CONFIG, "GBERRY_COMMS_CONFIG");
}

CommsParameters::~CommsParameters()
{
}

} // eon
