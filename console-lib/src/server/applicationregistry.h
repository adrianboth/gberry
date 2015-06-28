#ifndef APPLICATIONREGISTRY_H
#define APPLICATIONREGISTRY_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QMap>

namespace GBerry {
namespace Console {
namespace Server {

class ApplicationRegistryPrivate;

class ApplicationRegistry
{
public:
    explicit ApplicationRegistry();
    ~ApplicationRegistry();

    void insertFixedIdentificationCode(const QString& appId, const QString& code);

    // creates one-time code
    QString createIdentificationCode(const QString& appId);

    // returns appId, "" if no match
    QString matchCodeToApplication(const QString& code);

    void createLink(int connectionId, const QString&  applicationId);
    void removeLink(int connectionId);

    // returns -1 if no such link
    int connectionIdByApplicationId(const QString& applicationId) const;
    // returns "" if no such link
    QString applicationIdByConnectionId(int connectionId) const;

private:
    const QScopedPointer<ApplicationRegistryPrivate> _d;
};

}}} // eon

#endif // APPLICATIONREGISTRY_H
