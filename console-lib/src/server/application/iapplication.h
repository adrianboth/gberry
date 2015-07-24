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
 
 #ifndef IAPPLICATION
#define IAPPLICATION

#include <QSharedPointer>

#include "iapplicationmeta.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class IApplication
{

public:
    IApplication();
    virtual ~IApplication();

    enum State { Valid, Invalid, Downloading }; // TODO: should we have more states for downloadable apps?
    virtual State state() const = 0;

    virtual QString stateString() const;
    virtual QString id() const;

    // TODO: if IApplication and meta are tightly coupled -> do we need Qsharedpointer?
    virtual QSharedPointer<IApplicationMeta> meta() const = 0;
};

}}} // eon

#endif // IAPPLICATION

