#ifndef DOWNLOADABLEAPPLICATIONCACHE_H
#define DOWNLOADABLEAPPLICATIONCACHE_H

#include <QMap>
#include <QSharedPointer>

#include "server/application/iapplication.h"
using namespace GBerry::Console::Server;

namespace GBerryComms {

class DownloadableApplicationCache
{
public:
    DownloadableApplicationCache();
    ~DownloadableApplicationCache();

    QSharedPointer<IApplication> application(const QString& applicationFullId) const;
    void cacheApplication(QSharedPointer<IApplication> application);
    bool hasApplication(const QString& applicationFullId) const;

private:
    QMap<QString, QSharedPointer<IApplication>> _cachedApps;
};

} // eon

#endif // DOWNLOADABLEAPPLICATIONCACHE_H
