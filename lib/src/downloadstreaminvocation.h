#ifndef DOWNLOADSTREAMINVOCATION_H
#define DOWNLOADSTREAMINVOCATION_H

#include <QUrl>
#include <QScopedPointer>

#include "invocation.h"
#include "invocationfactoryimpl.h"
#include "httpinvocationdefinition.h"

namespace GBerry {

/**
 * This is pretty much as typical RESTInvocation but instead storing response
 * data to memory it is streamed a file.
 */
class DownloadStreamInvocation : public Invocation
{
    Q_OBJECT

public:
    explicit DownloadStreamInvocation(QObject* parent = 0);
    virtual ~DownloadStreamInvocation();

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) = 0;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) = 0;

    // TODO: later support for stream (possible to continue writing)
    virtual void setOutputFilePath(QString filePath) = 0;

    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const = 0;

    virtual int progressPercentage() const = 0;

signals:
    void downloadStarted(DownloadStreamInvocation* inv);
    //void downloadFinished(DownloadStreamInvocation* inv);
    //void downloadError(DownloadStreamInvocation* inv);
    void downloadProgress(DownloadStreamInvocation* inv);

};

} // eon

#endif // DOWNLOADSTREAMINVOCATION_H
