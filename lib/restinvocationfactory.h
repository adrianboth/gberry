#ifndef RESTINVOCATIONFACTORY_H
#define RESTINVOCATIONFACTORY_H

#include <QObject>

class RESTInvocationFactory : public QObject
{
    Q_OBJECT
public:
    explicit RESTInvocationFactory(configuration, QObject *parent = 0);
    ~RESTInvocationFactory();

    RESTInvocation newInvocation();

signals:

public slots:
};

#endif // RESTINVOCATIONFACTORY_H
