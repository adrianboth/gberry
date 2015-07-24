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
