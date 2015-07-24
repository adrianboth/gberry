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
 
 #include "applicationparameters.h"

namespace GBerryApplication {

const QString ApplicationParameters::APPLICATION_CODE("application-code");

ApplicationParameters::ApplicationParameters(IEnvironmentVariables& env) :
    GBerryConsoleCommon::RuntimeParameters(env)
{
    // TODO: info for these should be read from appcfg.json
    //_parser.setApplicationDescription("Communication manager of GBerry game platform");
    //_parser.addVersionOption()

    addValueOption(APPLICATION_CODE, new QCommandLineOption(APPLICATION_CODE, "Defines uses application code identification", "application_code"));
    addEnvironmentVariable(APPLICATION_CODE, "GBERRY_APPLICATION_CODE");
}

ApplicationParameters::~ApplicationParameters()
{
}

} // eon
