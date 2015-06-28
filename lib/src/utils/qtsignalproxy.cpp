#include "qtsignalproxy.h"

QtSignalProxy::QtSignalProxy(QObject *parent) : QObject(parent)
{

}

QtSignalProxy::~QtSignalProxy()
{

}

void QtSignalProxy::proxyInt(int i)
{
    _proxiedCount++;
    emit proxiedInt(i);
}

void QtSignalProxy::proxyNoParameters()
{
    _proxiedCount++;
    emit proxiedNoParameters();
}

int QtSignalProxy::proxiedCount() const
{
    return _proxiedCount;
}
