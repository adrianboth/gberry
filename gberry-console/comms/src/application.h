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
 
 #ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QSharedPointer>

// TODO: we could have lib in path "console"
#include "server/application/baseapplication.h"

using namespace GBerry::Console::Server;

// TODO: now just wrapper, lets see if we need to have this at all
class Application : public BaseApplication
{

public:
    // standard constructor
    explicit Application(QSharedPointer<ApplicationMeta> meta);
    virtual ~Application();

    ApplicationMeta& metaRef();

    static QSharedPointer<Application> copy(QSharedPointer<IApplication> iapp);
};

#endif // APPLICATION_H
