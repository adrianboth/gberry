#ifndef RESTINVOCATION_H
#define RESTINVOCATION_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>


class RESTInvocation : public QObject
{
    Q_OBJECT
public:
    explicit RESTInvocation(QObject *parent = 0);
    ~RESTInvocation();

    enum InvocationStatus {
        NOT_STARTED,
        ONGOING,
        CONNECTION_FAILED,
        TIMEOUT_OCCURRED,
        RESPONSE_RECEIVED
    };

    enum HttpStatus {
        UNDEFINED = 0,
        OK_200 = 200
    };

    virtual void get(QString invocationPath) = 0;
    virtual void post(QString invocationPath, QJsonDocument jsondoc) = 0;

    virtual InvocationStatus statusCode() const = 0;
    virtual HttpStatus responseHttpStatusCode() const = 0;
    virtual bool responseAvailable() const = 0;
    virtual QByteArray responseByteData() const = 0;
    virtual QString responseString() const = 0;
    virtual QString errorString() const = 0;
    // TODO: json data

signals:
    void finishedOK(RESTInvocation* invocation);
    void finishedError(RESTInvocation* invocation);

public slots:
};

#endif // RESTINVOCATION_H
