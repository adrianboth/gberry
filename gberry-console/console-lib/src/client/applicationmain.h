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
 
 #ifndef APPLICATIONMAIN_H
#define APPLICATIONMAIN_H

#include <QString>

class QGuiApplication;
class IEnvironmentVariables;
class LogControl;
class StdoutLogMsgHandler;

namespace GBerryApplication {

class ApplicationParameters;

class ApplicationMain
{
public:
    ApplicationMain(QGuiApplication* app);
    ~ApplicationMain();

    bool hasApplicationCode() const;
    QString applicationCode() const;

    // enters into event loop
    int exec();

private:
    const QGuiApplication* _app;
    IEnvironmentVariables* _env;
    ApplicationParameters* _params;
    LogControl*          _logControl;
    StdoutLogMsgHandler* _stdoutHandler;

};

} // eon

#endif // APPLICATIONMAIN_H
