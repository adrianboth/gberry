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
 
 #include "invocationfactoryimpl.h"

#include <QNetworkAccessManager>
#include <QVariant>
#include <QUrlQuery>

#include "restinvocation.h"
#include "restinvocationimpl.h"
#include "downloadstreaminvocationimpl.h"


InvocationFactoryImpl::InvocationFactoryImpl(QNetworkAccessManager* qnam) :
    _qnam(qnam)
{
    if (!_qnam)
        _qnam = new QNetworkAccessManager(this);
}

InvocationFactoryImpl::~InvocationFactoryImpl()
{
}

QUrl InvocationFactoryImpl::buildUrl(QString invocationPath) const
{
    QString baseUrl = this->property(InvocationFactory::URL_PREFIX_PROP).toString();
    return QUrl(baseUrl + invocationPath);
}

QUrl InvocationFactoryImpl::buildUrl(QString invocationPath, const QMap<QString, QString>& params) const
{
    QString baseUrl = this->property(InvocationFactory::URL_PREFIX_PROP).toString();
    QUrl url(baseUrl + invocationPath);
    QUrlQuery query;
    foreach(QString key, params.keys())
        query.addQueryItem(key, params[key]);

    url.setQuery(query);
    // TODO: using QString instead of QUrl
    return url;
}


RESTInvocation* InvocationFactoryImpl::newRESTInvocation()
{
    return new RESTInvocationImpl(this);
}

DownloadStreamInvocation *InvocationFactoryImpl::newDownloadStreamInvocation()
{
    return new DownloadStreamInvocationImpl(this);
}

QString InvocationFactoryImpl::defaultHostName() const
{
    QString baseUrl = this->property(InvocationFactory::URL_PREFIX_PROP).toString();
    return QUrl(baseUrl).host();
}
