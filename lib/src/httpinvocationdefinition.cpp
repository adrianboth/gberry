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
 
 #include "httpinvocationdefinition.h"

namespace GBerry {

HTTPInvocationDefinition::HTTPInvocationDefinition()
{

}

HTTPInvocationDefinition::~HTTPInvocationDefinition()
{

}

HTTPInvocationDefinition::Status HTTPInvocationDefinition::resolveHttpStatus(int code)
{
    switch (code)
    {
    case 200:
        return OK_200;
    case 403:
        return FORBIDDEN_403;
    default:
        return UNDEFINED;
    }
}

HTTPInvocationDefinition::Operation HTTPInvocationDefinition::httpOperation() const
{
    return _httpOperation;
}

void HTTPInvocationDefinition::setHttpOperation(HTTPInvocationDefinition::Operation op)
{
    _httpOperation = op;
}

QString HTTPInvocationDefinition::invocationPath() const
{
    return _invocationPath;
}

void HTTPInvocationDefinition::setInvocationPath(const QString &invocationPath)
{
    _invocationPath = invocationPath;
}

} // eon
