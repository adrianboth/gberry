#include "applicationmain.h"

#include <QGuiApplication>

#include <client/consoleapplication.h>
#include <systemservices.h>

#include "applicationparameters.h"

namespace GBerryApplication {

ApplicationMain::ApplicationMain(QGuiApplication* app) :
    _app(app)
{
    _env = new EnvironmentVariables (QProcessEnvironment::systemEnvironment());
    _params = new ApplicationParameters(*_env);
    _params->parse(_app->arguments());
}

ApplicationMain::~ApplicationMain()
{
    delete _env;
    delete _params;
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
