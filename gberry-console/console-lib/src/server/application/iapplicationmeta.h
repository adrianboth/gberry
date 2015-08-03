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
 
 #ifndef IAPPLICATIONMETA_H
#define IAPPLICATIONMETA_H

#include <QString>

namespace GBerry {
    namespace Console {
        namespace Server {

typedef QString ApplicationVersion;

class IApplicationMeta
{
public:
    IApplicationMeta() {}
    virtual ~IApplicationMeta() { } //qDebug() << "IApplicationMeta destructor"; }

    // combination of applicationId + version -> unique id
    virtual QString id() const = 0;

    virtual QString applicationId() const = 0;
    virtual ApplicationVersion version() const = 0;
    virtual bool isSystemApp() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString applicationDirPath() const = 0;
    virtual QString applicationExecutablePath() const = 0;
    virtual QString catalogImageFilePath() const = 0;
    virtual bool isFree() const = 0;
};

}}} // eon

#endif // IAPPLICATIONMETA_H

