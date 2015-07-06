#include "invocationfactoryimpl.h"

#include <QNetworkAccessManager>
#include <QVariant>

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
    QString baseUrl = this->property("url_prefix").toString();
    return QUrl(baseUrl + invocationPath);
}


RESTInvocation* InvocationFactoryImpl::newRESTInvocation()
{
    return new RESTInvocationImpl(this);
}

DownloadStreamInvocation *InvocationFactoryImpl::newDownloadStreamInvocation()
{
    return new DownloadStreamInvocationImpl(this);
}
