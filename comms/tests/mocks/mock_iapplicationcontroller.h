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
 
 #ifndef MOCK_IAPPLICATIONCONTROLLER_H
#define MOCK_IAPPLICATIONCONTROLLER_H


#include "interfaces/iapplicationcontroller.h"
#include "interfaces/ilaunchcontroller.h"

#include <gmock/gmock.h>

//#define MOCK_SIGNAL(func) void emit_##func() {  ##func() ); }

class MockIApplicationController : public IApplicationController
{
    Q_OBJECT
public:
    MOCK_METHOD0(launch, void());
    MOCK_METHOD0(pause, void());
    MOCK_METHOD0(resume, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(fullApplicationId, QString());

    void emitLaunched() { emit launched(); }
};

class MockILaunchController : public ILaunchController
{
    Q_OBJECT
public:
    MOCK_METHOD0(launch, void());
    MOCK_METHOD0(pause, void());
    MOCK_METHOD0(resume, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(fullApplicationId, QString());
    MOCK_METHOD1(useApplication, bool(const QString& s));

    void emitLaunched() { emit launched(); }
    void emitLaunchFailed() { emit launchFailed(); }
    void emitDied() { emit died(); }
};

#endif // MOCK_IAPPLICATIONCONTROLLER_H

