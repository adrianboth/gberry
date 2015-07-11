#include "downloadableapplicationcache.h"

namespace GBerryComms {

DownloadableApplicationCache::DownloadableApplicationCache()
{
}

DownloadableApplicationCache::~DownloadableApplicationCache()
{
}

QSharedPointer<IApplication> DownloadableApplicationCache::application(const QString &applicationFullId) const
{
    if (hasApplication(applicationFullId))
        return _cachedApps[applicationFullId];
    else
        return QSharedPointer<IApplication>(nullptr);
}

void DownloadableApplicationCache::cacheApplication(QSharedPointer<IApplication> application)
{
    _cachedApps[application->id()] = application;
}

bool DownloadableApplicationCache::hasApplication(const QString &applicationFullId) const
{
    return _cachedApps.contains(applicationFullId);
}

} // eon

// TODO: some kind of time stamp
