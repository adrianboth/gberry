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
 
 #ifndef BASEAPPLICATION_H
#define BASEAPPLICATION_H

#include <QSharedPointer>

#include "server/application/iapplication.h"
#include "server/application/applicationmeta.h"

namespace GBerry {
    namespace Console {
        namespace Server {


/**
 * @brief Very basic implementation of IApplication
 *
 * This class is usefull for tests, but also as intermediate class for
 * advanced implementations.
 */
class BaseApplication : public IApplication
{

public:
    BaseApplication(QSharedPointer<ApplicationMeta> meta);
    virtual ~BaseApplication();

    // IApplication
    virtual State state() const override;
    virtual QString id() const override;
    virtual QSharedPointer<IApplicationMeta> meta() const override;

    // --
    void markState(State newState);

private:
    QSharedPointer<IApplicationMeta> _meta;
    State _state;
};

}}} // eon

#endif // BASEAPPLICATION_H
