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
