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
 
 #ifndef MOCK_RESTINVOCATIONFACTORY_H
#define MOCK_RESTINVOCATIONFACTORY_H

#include "invocationfactory.h"

#include <gmock/gmock.h>


class MockRESTInvocationFactory : public InvocationFactory
{
public:

    MOCK_METHOD0(newRESTInvocation, RESTInvocation*());
    MOCK_METHOD0(newDownloadStreamInvocation, DownloadStreamInvocation*());
    MOCK_CONST_METHOD0(defaultHostName, QString());
};

#endif // MOCK_RESTINVOCATIONFACTORY_H

