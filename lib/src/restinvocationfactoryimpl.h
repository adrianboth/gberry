#ifndef RESTINVOCATIONFACTORYIMPL_H
#define RESTINVOCATIONFACTORYIMPL_H

#include <QUrl>

#include "restinvocationfactory.h"

class QNetworkAccessManager;

/**
 * @brief The RESTInvocationFactoryImpl class
 *
 * Properties:
 *   url_prefix             http://<host>:<port>/<url>
 *
 */
class RESTInvocationFactoryImpl : public RESTInvocationFactory
{
    Q_DISABLE_COPY(RESTInvocationFactoryImpl);

public:
    RESTInvocationFactoryImpl(QNetworkAccessManager* qnam = NULL);
    virtual ~RESTInvocationFactoryImpl();

    QNetworkAccessManager* getQNetworkAccessManager() const { return _qnam; }
    QUrl buildUrl(QString invocationPath) const;

    // RESTInvocationFactory
    virtual RESTInvocation* newRESTInvocation();
    virtual DownloadStreamInvocation* newDownloadStreamInvocation();

private:
    QNetworkAccessManager* _qnam;

};

#endif // RESTINVOCATIONFACTORYIMPL_H
