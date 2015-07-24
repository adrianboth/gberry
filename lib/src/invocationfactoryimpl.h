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
 
 #ifndef RESTINVOCATIONFACTORYIMPL_H
#define RESTINVOCATIONFACTORYIMPL_H

#include <QUrl>

#include "invocationfactory.h"

class QNetworkAccessManager;

/**
 * @brief The InvocationFactoryImpl class
 *
 * Properties:
 *   url_prefix             http://<host>:<port>/<url>
 *
 */
class InvocationFactoryImpl : public InvocationFactory
{
    Q_DISABLE_COPY(InvocationFactoryImpl);

public:
    InvocationFactoryImpl(QNetworkAccessManager* qnam = NULL);
    virtual ~InvocationFactoryImpl();

    QNetworkAccessManager* getQNetworkAccessManager() const { return _qnam; }
    QUrl buildUrl(QString invocationPath) const;

    // RESTInvocationFactory
    virtual RESTInvocation* newRESTInvocation() override;
    virtual DownloadStreamInvocation* newDownloadStreamInvocation() override;
    virtual QString defaultHostName() const;

private:
    QNetworkAccessManager* _qnam;

};

#endif // RESTINVOCATIONFACTORYIMPL_H
