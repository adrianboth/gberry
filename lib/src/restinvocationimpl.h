#ifndef RESTINVOCATIONIMPL_H
#define RESTINVOCATIONIMPL_H

#include <QUrl>
#include <QScopedPointer>

#include "invocation.h"
#include "restinvocation.h"
#include "invocationfactoryimpl.h"
#include "httpinvocationdefinition.h"

using namespace GBerry;

namespace {
    static const uint DEFAULT_TIMEOUT_MS = 1000*30;
}

class RESTInvocationImplPrivate;

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
    RESTInvocationImpl(InvocationFactoryImpl* factory, QObject* parent = 0);
    virtual ~RESTInvocationImpl();

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) override;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override;

    virtual void execute() override;
    virtual void abort() override;

    virtual Invocation::InvocationStatus statusCode() const;
    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const;
    virtual bool responseAvailable() const;
    virtual QByteArray responseByteData() const;
    virtual QString responseString() const;
    virtual QString errorString() const;

private:
    const QScopedPointer<RESTInvocationImplPrivate> _d;
};

#endif // RESTINVOCATIONIMPL_H
