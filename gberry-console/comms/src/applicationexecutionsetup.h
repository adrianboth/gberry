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

#ifndef APPLICATIONEXECUTIONSETUP_H
#define APPLICATIONEXECUTIONSETUP_H

#include <QScopedPointer>
#include <QProcess>
#include <QStringList>

#include "server/application/iapplication.h"
using namespace GBerry::Console::Server;

#include "iapplicationexecutionsetup.h"
#include "qtlibrariesmanager.h"
#include "server/applicationregistry.h"

namespace GBerryComms {

ERRORCLASS(ApplicationExecutionSetupError)

class ApplicationExecutionSetupErrors {
public:
    static const ApplicationExecutionSetupError EXECUTION_SETUP_FAILED;
    static const ApplicationExecutionSetupError LOGGING_SETUP_FAILED;
};

class ApplicationExecutionSetup : public IApplicationExecutionSetup
{
public:
    ApplicationExecutionSetup(QtLibrariesManager* qtlibsManager, ApplicationRegistry* registry, bool enableOutputLogging);
    ~ApplicationExecutionSetup();

    bool prepare(QProcess& process, const IApplication& app, Result& res) override;

    void setAdditionalArguments(const QStringList& args) override;
    void setEnvironmentVariable(const QString& varName, const QString& varValue) override;

private:
    class Private;
    const QScopedPointer<Private> _d;
};

} // eon

#endif // APPLICATIONEXECUTIONSETUP_H
