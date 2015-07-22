#ifndef RESTINVOCATIONFACTORY_H
#define RESTINVOCATIONFACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

class RESTInvocation;

namespace GBerry {
    class DownloadStreamInvocation;
}
using namespace GBerry;

// TODO: namespace
// TODO: I prefix (then *.cpp file could be removed

class InvocationFactory : public QObject
{
    Q_OBJECT
public:
    explicit InvocationFactory(QObject *parent = 0);
    virtual ~InvocationFactory();

    virtual RESTInvocation* newRESTInvocation() = 0;
    virtual DownloadStreamInvocation* newDownloadStreamInvocation() = 0;
    virtual QString defaultHostName() const = 0;

signals:

public slots:
};

#endif // RESTINVOCATIONFACTORY_H
