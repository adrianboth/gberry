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
#include "serverconnectionmodel.h"

#include "invocationfactory.h"

#define LOG_AREA "ServerConnectionModel"
#include "log/log.h"


namespace GBerryClient {

class ServerConnectionModel::Private {
public:
    Private(InvocationFactory* invocationFactory_) :
        invocationFactory(invocationFactory_) {
        Q_ASSERT(invocationFactory);

        if (!invocationFactory->property(InvocationFactory::URL_PREFIX_PROP).isValid()) {
            // TODO: some valid default, but better would checking connection conditions before hand (like ping status)
            invocationFactory->setProperty(InvocationFactory::URL_PREFIX_PROP, "http://localhost");
        }
    }

    InvocationFactory* invocationFactory;
};

ServerConnectionModel::ServerConnectionModel(InvocationFactory *invocationFactory, QObject *parent) :
    QObject(parent),
    _d(new Private(invocationFactory))
{
}

ServerConnectionModel::~ServerConnectionModel()
{
}

void ServerConnectionModel::setServerHost(const QString &host)
{
    // make sure there is protocol defined
    if (host.startsWith("http")) {
        _d->invocationFactory->setProperty(InvocationFactory::URL_PREFIX_PROP, host);
    }
    else {
        // TODO: rest v1 part should be read from a ini file
        _d->invocationFactory->setProperty(InvocationFactory::URL_PREFIX_PROP, "http://" + host + "/gberryrest/v1");
    }

    DEBUG("New server host:" << _d->invocationFactory->property(InvocationFactory::URL_PREFIX_PROP).toString());
}

} // eon
