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

class RESTInvocationFactory : public QObject
{
    Q_OBJECT
public:
    explicit RESTInvocationFactory(QObject *parent = 0);
    virtual ~RESTInvocationFactory();

    virtual RESTInvocation* newRESTInvocation() = 0;
    virtual DownloadStreamInvocation* newDownloadStreamInvocation() = 0;

signals:

public slots:
};

#endif // RESTINVOCATIONFACTORY_H
