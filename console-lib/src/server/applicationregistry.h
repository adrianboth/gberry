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

    void createLink(int connectionId, const QString&  applicationId);
    void removeLink(int connectionId);

    // returns -1 if no such link
    int connectionIdByApplicationId(const QString& applicationId) const;
    // returns "" if no such link
    QString applicationIdByConnectionId(int connectionId) const;

private:
    QMap<QString, QString> _appIdByCode;
    QMap<int, QString> _appIdByConnectionId;
};

}}} // eon

#endif // APPLICATIONREGISTRY_H
