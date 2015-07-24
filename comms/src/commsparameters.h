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
 
 #ifndef COMMSPARAMETERS_H
#define COMMSPARAMETERS_H

#include "systemservices.h"
#include "common/runtimeparameters.h"

namespace Comms
{

/**
 * @brief Parameter handler
 *
 * Handles parameters for 'comms' application from various sources:
 *  - command line
 *  - environment variables
 *  - built time
 *
 * Contains a generic approach but also option specific handling.
 *
 * Typical order of predescence (top most effective if defined):
 *  - command line
 *  - environment variable
 *  - build time default
 */
class CommsParameters : public GBerryConsoleCommon::RuntimeParameters
{
public:
    explicit CommsParameters(IEnvironmentVariables& env);
    virtual ~CommsParameters();

    static const QString ROOT_PATH;
    static const QString DISABLE_WAITAPP;
    static const QString DISABLE_MAINUI;
    static const QString COMMS_CONFIG;
};

} // eon

#endif // COMMSPARAMETERS_H
