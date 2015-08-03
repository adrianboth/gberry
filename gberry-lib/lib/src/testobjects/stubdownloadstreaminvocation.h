#ifndef STUBDOWNLOADSTREAMINVOCATION_H
#define STUBDOWNLOADSTREAMINVOCATION_H

#include <QMap>
#include <QJsonDocument>

#include "downloadstreaminvocation.h"

class StubDownloadStreamInvocation : public DownloadStreamInvocation
{
public:
    StubDownloadStreamInvocation();
    ~StubDownloadStreamInvocation();

    // -- Invocation
    int executeCallCount{0};
    virtual void execute() override { executeCallCount++; }

    int abortCallCount{0};
    virtual void abort() override { abortCallCount++; }

    Invocation::InvocationStatus statusCodeToReturn{Invocation::NOT_STARTED};
    virtual Invocation::InvocationStatus statusCode() const { return statusCodeToReturn; }

    Result resultToReturn;
    virtual Result result() const override { return resultToReturn; }

    QString definedOperation;
    QString paramInvocationPath;
    virtual void defineGetOperation(const QString& invocationPath) override { definedOperation = "GET"; paramInvocationPath = invocationPath; }

    QJsonDocument paramJsonDoc;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override { definedOperation = "GET"; paramInvocationPath = invocationPath; paramJsonDoc = jsondoc; }

    QMap<QString, QString> parameters;
    virtual void defineParameter(const QString& paramName, const QString& paramValue) override { parameters[paramName] = paramValue; }

    HTTPInvocationDefinition::Status responseHttpStatusCodeToReturn{HTTPInvocationDefinition::UNDEFINED};
    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const { return responseHttpStatusCodeToReturn; }

    // TODO: later support for stream (possible to continue writing)
    QString paramOutputFilePath;
    virtual void setOutputFilePath(const QString& filePath) { paramOutputFilePath = filePath; }

    int progressPercentageToReturn{0};
    virtual int progressPercentage() const { return progressPercentageToReturn; }
};

#endif // STUBDOWNLOADSTREAMINVOCATION_H
