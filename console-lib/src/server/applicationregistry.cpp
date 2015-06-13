#include "applicationregistry.h"

#include <QTime>

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

}}} // eon
