#include "applicationregistry.h"

#include <QTime>

#define LOG_AREA "ApplicationRegistry"
#include "log/log.h"

namespace GBerry {
namespace Console {
namespace Server {

ApplicationRegistry::ApplicationRegistry()
{

}

ApplicationRegistry::~ApplicationRegistry()
{

}

QString ApplicationRegistry::createIdentificationCode(const QString &appId)
{
    QTime time(QTime::currentTime());
    QString code(QString::number(time.elapsed()));
    // TODO: verification that unique code

    _appIdByCode[code] = appId;
    return code;
}

QString ApplicationRegistry::matchCodeToApplication(const QString &code)
{
    if (_appIdByCode.contains(code)) {
        QString appId = _appIdByCode[code];
        _appIdByCode.remove(code);
        return appId;
    }

    return "";
}

void ApplicationRegistry::createLink(int connectionId, const QString&  applicationId)
{
    if (_appIdByConnectionId.contains(connectionId)) {
        WARN("ConnectionId already used on other link: connectionId =" << connectionId << ", old applicationId =" << _appIdByConnectionId[connectionId] << ", new applicationId =" << applicationId);
    }
    _appIdByConnectionId[connectionId] = applicationId;
}

void ApplicationRegistry::removeLink(int connectionId)
{
    if (_appIdByConnectionId.contains(connectionId)) {
        _appIdByConnectionId.remove(connectionId);
    } else {
        WARN("Unknown link removal requested: connectionId =" << connectionId);
    }
}

int ApplicationRegistry::connectionIdByApplicationId(const QString& applicationId) const
{
    foreach (int connectionId, _appIdByConnectionId.keys()) {
        if (_appIdByConnectionId[connectionId] == applicationId)
            return connectionId;
    }

    return -1;
}

QString ApplicationRegistry::applicationIdByConnectionId(int connectionId) const
{
    if (_appIdByConnectionId.contains(connectionId))
        return _appIdByConnectionId[connectionId];

    return "";
}

}}} // eon
