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

#ifndef IAPPLICATIONEXECUTIONSETUP_H
#define IAPPLICATIONEXECUTIONSETUP_H

#include <QScopedPointer>
#include <QProcess>

#include "server/application/iapplication.h"
using namespace GBerry::Console::Server;

#include "result.h"
using namespace GBerryLib;

namespace GBerryComms {

class IApplicationExecutionSetup
{
public:
    IApplicationExecutionSetup() {}
    virtual ~IApplicationExecutionSetup() {}

    virtual bool prepare(QProcess& process, const IApplication& app, Result& res) = 0;

    virtual void setAdditionalArguments(const QStringList& args) = 0;
    virtual void setEnvironmentVariable(const QString& varName, const QString& varValue) = 0;
};

} // eon

#endif // IAPPLICATIONEXECUTIONSETUP_H

