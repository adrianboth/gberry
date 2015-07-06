#include "restinvocationfactoryimpl.h"

#include <QNetworkAccessManager>
#include <QVariant>

#include "restinvocation.h"
#include "restinvocationimpl.h"
#include "downloadstreaminvocationimpl.h"


RESTInvocationFactoryImpl::RESTInvocationFactoryImpl(QNetworkAccessManager* qnam) :
    _qnam(qnam)
{
    if (!_qnam)
        _qnam = new QNetworkAccessManager(this);
}

RESTInvocationFactoryImpl::~RESTInvocationFactoryImpl()
{
}

QUrl RESTInvocationFactoryImpl::buildUrl(QString invocationPath) const
{
    QString baseUrl = this->property("url_prefix").toString();
    return QUrl(baseUrl + invocationPath);
}


RESTInvocation* RESTInvocationFactoryImpl::newRESTInvocation()
{
    return new RESTInvocationImpl(this);
}

DownloadStreamInvocation *RESTInvocationFactoryImpl::newDownloadStreamInvocation()
{
    return new DownloadStreamInvocationImpl(this);
}
