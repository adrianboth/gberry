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
