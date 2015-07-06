#ifndef RESTINVOCATION_H
#define RESTINVOCATION_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>

#include "invocation.h"
#include "restinvocationdefinition.h"
using namespace GBerry;

/**
 * RESTInvocation is HTTP operation that has some restrictions that guide
 * it use to REST style operation, not just any HTTP request (you can use
 * QNetworkAccessManager directly to those).
 *
 * RESTInvocation is abstract class to separate interface and implementation.
 * The main benefit is to support unit testing.
 */
class RESTInvocation : public Invocation
{
    Q_OBJECT
public:
    explicit RESTInvocation(QObject *parent = 0);
    virtual ~RESTInvocation();

    // these are used to define operation
    virtual void defineGetOperation(const QString& invocationPath) = 0;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) = 0;

    // TODO: functions to define url parameters

    // TODO: could some of these moved to impl class?
    virtual RESTInvocationDefinition::HttpStatus responseHttpStatusCode() const = 0;
    virtual bool responseAvailable() const = 0;
    virtual QByteArray responseByteData() const = 0;
    virtual QString responseString() const = 0;
    virtual QString errorString() const = 0;

    // TODO: json data response

};

#endif // RESTINVOCATION_H
