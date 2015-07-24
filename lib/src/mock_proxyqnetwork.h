/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
    MOCK_METHOD1(get, ProxyQNetworkReply*(const QNetworkRequest& request));
};


class StubQNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    StubQNetworkReply(QObject* parent=NULL) : QNetworkReply(parent) {} // accesses protected constructor

    // implementing pure virtual
    virtual qint64 readData(char* data, qint64 maxSize) { Q_UNUSED(data); Q_UNUSED(maxSize); return 0; }
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

