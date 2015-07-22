#ifndef DOWNLOADSTREAMINVOCATIONIMPL_H
#define DOWNLOADSTREAMINVOCATIONIMPL_H

#include <QUrl>
#include <QScopedPointer>

#include "downloadstreaminvocation.h"
// TODO: change name of factory
#include "invocationfactoryimpl.h"

namespace GBerry {

class DownloadStreamInvocationImplPrivate;

/**
 * This is pretty much as typical RESTInvocation but instead storing response
 * data to memory it is streamed a file.
 */
class DownloadStreamInvocationImpl : public DownloadStreamInvocation
{
    Q_OBJECT

public:
    DownloadStreamInvocationImpl(InvocationFactoryImpl* factory, QObject* parent = 0);
    virtual ~DownloadStreamInvocationImpl();

    // -- Invocation
    virtual void execute() override;
    virtual void abort() override;
    virtual Result result() const override;
    virtual Invocation::InvocationStatus statusCode() const override;

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) override;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override;

    // TODO: later support for stream (possible to continue writing)
    virtual void setOutputFilePath(QString filePath);

    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const override;

    virtual int progressPercentage() const override;

private:
    const QScopedPointer<DownloadStreamInvocationImplPrivate> _d;

};

} // eon

#endif // DOWNLOADSTREAMINVOCATIONIMPL_H
