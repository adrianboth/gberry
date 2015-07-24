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
 
 #ifndef HTTPINVOCATIONDEFINITION_H
#define HTTPINVOCATIONDEFINITION_H

#include <QString>

namespace GBerry {

class HTTPInvocationDefinition
{
public:
    explicit HTTPInvocationDefinition();
    ~HTTPInvocationDefinition();

    enum Operation { NOT_DEFINED, GET, POST };

    enum Status {
        UNDEFINED = 0,
        OK_200 = 200,
        FORBIDDEN_403 = 403
    };

    static Status resolveHttpStatus(int code);

    Operation httpOperation() const;
    void setHttpOperation(Operation op);

    QString invocationPath() const;
    void setInvocationPath(const QString& invocationPath);

private:
    Operation _httpOperation{NOT_DEFINED};
    QString _invocationPath;

};

} // eon

#endif // HTTPINVOCATIONDEFINITION_H
