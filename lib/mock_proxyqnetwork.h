#ifndef MOCK_PROXYQNETWORK_H
#define MOCK_PROXYQNETWORK_H

#include "proxyqnetwork.h"

#include <gmock/gmock.h>


class MockProxyQNetworkReply : public ProxyQNetworkReply
{
public:
    MockProxyQNetworkReply() : ProxyQNetworkReply(Q_NULLPTR) {}

    MOCK_CONST_METHOD0(isFinished, bool());
};


class MockProxyQNetworkAccessManager : public ProxyQNetworkAccessManager
{
public:
    MockProxyQNetworkAccessManager() : ProxyQNetworkAccessManager(Q_NULLPTR) {}

    // QNetworkReply *get(const QNetworkRequest &request);
    MOCK_METHOD1(get, QNetworkReply*(const QNetworkRequest& request));
    MOCK_METHOD1(get_proxytest, ProxyQNetworkReply*(const QNetworkRequest& request));
};


class StubQNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    StubQNetworkReply(QObject* parent=NULL) : QNetworkReply(parent) {} // accesses protected constructor

    // implementing pure virtual
    virtual qint64 readData(char* data, qint64 maxSize) {}
    //virtual qint64 QIODevice::​writeData(const char * data, qint64 maxSize) {}

public Q_SLOTS:
    virtual void abort() {}
};


// for using templating in dependency injection technique
class MockNetworkAccessManager : public QObject
{
    Q_OBJECT

 public:
     // QNetworkReply *get(const QNetworkRequest &request);
     MOCK_METHOD1(get, QNetworkReply*(const QNetworkRequest& request));

};
#endif // MOCK_PROXYQNETWORK_H

