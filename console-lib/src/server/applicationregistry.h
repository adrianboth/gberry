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
