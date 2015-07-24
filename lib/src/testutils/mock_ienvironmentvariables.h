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
 
 #ifndef MOCK_IENVIRONMENTVARIABLES_H
#define MOCK_IENVIRONMENTVARIABLES_H

#include "systemservices.h"

#include <QMap>

#include <gmock/gmock.h>


class MockIEnvironmentVariables : public IEnvironmentVariables
{
public:
    MOCK_CONST_METHOD1(contains, bool(const QString&));
    MOCK_CONST_METHOD1(value, QString(const QString&));
    MOCK_CONST_METHOD2(value, QString(const QString&, const QString& defaultValue));
};

class StubIEnvironmentVariables : public IEnvironmentVariables
{
public:
    StubIEnvironmentVariables(QMap<QString, QString>& _values) : _map(_values) {}
    virtual ~StubIEnvironmentVariables() {}

    virtual bool contains(const QString &key) const override {
        return _map.contains(key);
    }

    virtual QString value(const QString &key, const QString &defaultValue) const {
        return _map.value(key, defaultValue);
    }
private:
    QMap<QString, QString>& _map;
};

#endif // MOCK_IENVIRONMENTVARIABLES_H

