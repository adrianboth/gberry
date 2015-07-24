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
 
 #include "realsystemservices.h"

#include <QTimer>

RealSystemServices::RealSystemServices()
{

}

RealSystemServices::~RealSystemServices()
{

}

void RealSystemServices::singleshotTimer(int msec, QObject *receiver, const char *member)
{
    QTimer::singleShot(msec, receiver, member);
}

void RealSystemServices::registerItself()
{
    QCoreApplication* app = QCoreApplication::instance();
    QObject *obj = this;
    QVariant var = QVariant::fromValue(obj);

    // note that we are not freeing previous service anyway
    app->setProperty("systemservices", var);
}
