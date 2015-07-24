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
 
 #include "applicationregistry.h"

#include <QTime>

#define LOG_AREA "ApplicationRegistry"
#include "log/log.h"

namespace GBerry {
namespace Console {
namespace Server {

class ApplicationRegistryPrivate
{
public:
    QMap<QString, QString> appIdByFixedCode;
    QMap<QString, QString> appIdByCode;
    QMap<int, QString> appIdByConnectionId;
};

ApplicationRegistry::ApplicationRegistry() :
    _d(new ApplicationRegistryPrivate)
{

}

ApplicationRegistry::~ApplicationRegistry()
{

}

void ApplicationRegistry::insertFixedIdentificationCode(const QString& appId, const QString& code)
{
    _d->appIdByFixedCode[code] = appId;
}

QString ApplicationRegistry::createIdentificationCode(const QString &appId)
{
    QTime time(QTime::currentTime());
    QString code(QString::number(time.elapsed()));
    // TODO: verification that unique code

    _d->appIdByCode[code] = appId;
    return code;
}

QString ApplicationRegistry::matchCodeToApplication(const QString &code)
{
    if (_d->appIdByCode.contains(code)) {
        QString appId = _d->appIdByCode[code];
        _d->appIdByCode.remove(code);
        return appId;
    } else if (_d->appIdByFixedCode.contains(code)) {
        return _d->appIdByFixedCode[code];
    }

    return "";
}

void ApplicationRegistry::createLink(int connectionId, const QString&  applicationId)
{
    if (_d->appIdByConnectionId.contains(connectionId)) {
        WARN("ConnectionId already used on other link: connectionId =" << connectionId << ", old applicationId =" << _d->appIdByConnectionId[connectionId] << ", new applicationId =" << applicationId);
    }
    _d->appIdByConnectionId[connectionId] = applicationId;
}

void ApplicationRegistry::removeLink(int connectionId)
{
    if (_d->appIdByConnectionId.contains(connectionId)) {
        _d->appIdByConnectionId.remove(connectionId);
    } else {
        WARN("Unknown link removal requested: connectionId =" << connectionId);
    }
}

int ApplicationRegistry::connectionIdByApplicationId(const QString& applicationId) const
{
    foreach (int connectionId, _d->appIdByConnectionId.keys()) {
        if (_d->appIdByConnectionId[connectionId] == applicationId)
            return connectionId;
    }

    return -1;
}

QString ApplicationRegistry::applicationIdByConnectionId(int connectionId) const
{
    if (_d->appIdByConnectionId.contains(connectionId))
        return _d->appIdByConnectionId[connectionId];

    return "";
}

}}} // eon
