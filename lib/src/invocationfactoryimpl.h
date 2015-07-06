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
    virtual RESTInvocation* newRESTInvocation();
    virtual DownloadStreamInvocation* newDownloadStreamInvocation();

private:
    QNetworkAccessManager* _qnam;

};

#endif // RESTINVOCATIONFACTORYIMPL_H
