#ifndef APPLICATIONREGISTRY_H
#define APPLICATIONREGISTRY_H

#include <QObject>
#include <QString>
#include <QMap>

namespace GBerry {
namespace Console {
namespace Server {

class ApplicationRegistry
{
public:
    explicit ApplicationRegistry();
    ~ApplicationRegistry();

    QString createIdentificationCode(const QString& appId);

    // returns appId, "" if no match
    QString matchCodeToApplication(const QString& code);

    // TODO: is this need, who keeps record of active application
    void activateApplication(const QString& appId);

    // TODO: how and you wants ref to active app

private:
    QMap<QString, QString> _appIdByCode;
};

}}} // eon

#endif // APPLICATIONREGISTRY_H
