#ifndef RESTINVOCATIONIMPL_H
#define RESTINVOCATIONIMPL_H

#include <QUrl>
#include "restinvocation.h"
#include "restinvocationfactoryimpl.h"


namespace {
    static const uint DEFAULT_TIMEOUT_MS = 1000*30;
}

/**
 * @brief The RESTInvocationImpl class
 *
 * Properties:
 *   timeout
 *
 */
class RESTInvocationImpl : public RESTInvocation
{
public:
    RESTInvocationImpl(RESTInvocationFactoryImpl* factory, QObject* parent = 0);
    ~RESTInvocationImpl();

    virtual void get(QString invocationPath);
    virtual void get(QUrl url);

    virtual void post(QString invocationPath, QJsonDocument jsondoc);
    virtual void post(QUrl url, QJsonDocument jsondoc);

    virtual RESTInvocation::InvocationStatus statusCode() const;
    virtual RESTInvocation::HttpStatus responseHttpStatusCode() const;
    virtual bool responseAvailable() const;
    virtual QByteArray responseByteData() const;
    virtual QString responseString() const;
    virtual QString errorString() const;

protected slots:
    void httpFinished();

private:
    RESTInvocationFactoryImpl* _factory;
    RESTInvocation::InvocationStatus _invocationStatus;
    RESTInvocation::HttpStatus _httpStatus;
    QString _data;
    // TODO: encoding,  content-type
    QNetworkReply* _reply;
    QUrl _url;
};

#endif // RESTINVOCATIONIMPL_H
