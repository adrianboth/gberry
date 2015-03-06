#ifndef RESTINVOCATIONFACTORY_H
#define RESTINVOCATIONFACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

class RESTInvocation;

// TODO: namespace
// TODO: I prefix (then *.cpp file could be removed

class RESTInvocationFactory : public QObject
{
    Q_OBJECT
public:
    explicit RESTInvocationFactory(QObject *parent = 0);
    virtual ~RESTInvocationFactory();

    virtual RESTInvocation* newInvocation() = 0;

signals:

public slots:
};

#endif // RESTINVOCATIONFACTORY_H
