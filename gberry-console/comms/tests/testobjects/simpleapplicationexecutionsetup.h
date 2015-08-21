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

#ifndef SIMPLEAPPLICATIONEXECUTIONSETUP_H
#define SIMPLEAPPLICATIONEXECUTIONSETUP_H

#include "iapplicationexecutionsetup.h"
using namespace GBerryComms;

class SimpleApplicationExecutionSetup : public IApplicationExecutionSetup {
public:
    virtual bool prepare(QProcess& process, const IApplication& app, Result& res) override {
        Q_UNUSED(res);
        process.setProgram(app.meta()->applicationExecutablePath());
        return true;
    }


    virtual void setAdditionalArguments(const QStringList& args) override { Q_UNUSED(args) }
    virtual void setEnvironmentVariable(const QString& varName, const QString& varValue) {
        Q_UNUSED(varName);
        Q_UNUSED(varValue);
    }
};

#endif // SIMPLEAPPLICATIONEXECUTIONSETUP_H
