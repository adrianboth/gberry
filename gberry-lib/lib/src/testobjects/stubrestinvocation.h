#ifndef STUBRESTINVOCATION_H
#define STUBRESTINVOCATION_H

#include <QMap>

#include "restinvocation.h"
using namespace GBerry;

class StubRestInvocation : public RESTInvocation
{
public:
    StubRestInvocation();
    ~StubRestInvocation();

    // -- Invocation
    int executeCallCount{0};
    virtual void execute() override { executeCallCount++; }

    int abortCallCount{0};
    virtual void abort() override { abortCallCount++; }

    Invocation::InvocationStatus statusCodeToReturn{Invocation::NOT_STARTED};
    virtual Invocation::InvocationStatus statusCode() const { return statusCodeToReturn; }

    Result resultToReturn;
    virtual Result result() const override { return resultToReturn; }

    // -- RESTInvocation

    QString definedOperation;
    QString paramInvocationPath;
    virtual void defineGetOperation(const QString& invocationPath) override { definedOperation = "GET"; paramInvocationPath = invocationPath; }

    QJsonDocument paramJsonDoc;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override { definedOperation = "GET"; paramInvocationPath = invocationPath; paramJsonDoc = jsondoc; }

    QMap<QString, QString> parameters;
    virtual void defineParameter(const QString& paramName, const QString& paramValue) override { parameters[paramName] = paramValue; }

    HTTPInvocationDefinition::Status responseHttpStatusCodeToReturn{HTTPInvocationDefinition::UNDEFINED};
    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const { return responseHttpStatusCodeToReturn; }

    QString responseToReturn;
    virtual bool responseAvailable() const { return responseToReturn.size() > 0; }
    virtual QByteArray responseByteData() const { return responseToReturn.toLatin1(); }
    virtual QString responseString() const { return responseToReturn; }

    // --
    void emitFinishedOk() { RESTInvocation* inv = this; emit inv->finishedOK(inv); }
    void emitFinishedError() { RESTInvocation* inv = this; emit inv->finishedError(inv); }

};

#endif // STUBRESTINVOCATION_H
