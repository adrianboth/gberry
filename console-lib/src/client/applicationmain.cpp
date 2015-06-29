#include "applicationmain.h"

#include <QGuiApplication>

#include <client/consoleapplication.h>
#include <systemservices.h>

#include "applicationparameters.h"

#define LOG_AREA "ApplicationMain"
#include <log/log.h>
#include <log/logcontrol.h>
#include <log/stdoutlogmsghandler.h>

namespace GBerryApplication {

ApplicationMain::ApplicationMain(QGuiApplication* app) :
    _app(app)
{
    _stdoutHandler = new StdoutLogMsgHandler(Log::TRACE);
    _logControl = new LogControl;
    _logControl->registerMsgHandler(_stdoutHandler);

    Log::singleton().use(_logControl);

    _env = new EnvironmentVariables (QProcessEnvironment::systemEnvironment());
    _params = new ApplicationParameters(*_env);
    _params->parse(_app->arguments());

}

ApplicationMain::~ApplicationMain()
{
    delete _env;
    delete _params;
    delete _logControl;
    delete _stdoutHandler;
}

bool ApplicationMain::hasApplicationCode() const
{
    return _params->isSet(ApplicationParameters::APPLICATION_CODE);
}

QString ApplicationMain::applicationCode() const
{
    return _params->value(ApplicationParameters::APPLICATION_CODE);
}

int ApplicationMain::exec()
{
    // enter to event loop
    return _app->exec();
}

}
