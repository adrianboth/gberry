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
 
 #include "headserverconnection.h"

#include <QQueue>

#include "invocationfactory.h"
#include "restinvocation.h"
#include "request.h"
#include "systemservices.h"
#include "utils/qtsignalproxy.h"

#define LOG_AREA "HeadServerConnection"
#include "log/log.h"


namespace {
    enum ConnectionState { NOT_CONNECTED, CONNECTED };
    enum PingState { IDLE_WAITING, SENT_AND_WAITING };
}

namespace GBerry {

const char* HeadServerConnection::PING_INTERVAL_INT_PROP = "pingIntervalMs";
const char* HeadServerConnection::PING_TIMEOUT_INT_PROP = "pingTimeoutMs";


class HeadServerConnectionPrivate {

public:
    HeadServerConnectionPrivate(HeadServerConnection* q_,
                                InvocationFactory* invocationFactory_) :
        q(q_),
        invocationFactory(invocationFactory_)
        {
        QObject::connect(&pingIntervalProxy, &QtSignalProxy::proxiedNoParameters,
                         [this] () { this->pingIntervalCheck(); });
    }

    // -- members
    ConnectionState connectionState{NOT_CONNECTED};
    PingState pingState{IDLE_WAITING};

    HeadServerConnection* q;
    InvocationFactory* invocationFactory;
    QQueue<Request*> requestQueue;
    int lastPingTimestamp{0};
    QtSignalProxy pingIntervalProxy;

    /* */
    void ping()
    {
        RESTInvocation* invocation = invocationFactory->newRESTInvocation();

        if (!invocation) {
            ERROR("Request::prepareInvocation() returned NULL Invocation");
            return;
        }

        // invocation gets deleted once we get response

        QObject::connect(invocation, &RESTInvocation::finishedOK,
                [=] () { this->onPingReady(); invocation->deleteLater(); });

        QObject::connect(invocation, &RESTInvocation::finishedError,
                [=] () { this->onPingError(); invocation->deleteLater(); });

        //invocation->defineGetOperation("/user/ping?user_token=81555c66-25fe-4c75-a56a-37ff1ab72d90");
        invocation->defineGetOperation("/ping/");
        invocation->execute();
        pingState = SENT_AND_WAITING;

    }

    /* When ping passes succesfully. We don't really care of about response data. */
    void onPingReady()
    {
        ConnectionState oldConnectionState = connectionState;
        connectionState = CONNECTED;
        pingState = IDLE_WAITING;

        if (oldConnectionState == NOT_CONNECTED) {
            INFO("Connection to head server established");
            emit q->connected();
        }

        doMainStateLoop();
    }

    /* When ping REST call fails*/
    void onPingError()
    {
        ConnectionState oldConnectionState = connectionState;
        connectionState = NOT_CONNECTED;
        pingState = IDLE_WAITING;

        // all requests waiting to be served will fail because there is no connection
        if (!requestQueue.isEmpty()) {
            // if there for some reason comes new request while handling current ones
            // do not fail those new ones.
            QQueue<Request*> currentlyWaitingRequests = requestQueue;
            requestQueue.empty();

            foreach(auto req, currentlyWaitingRequests) {
                Result res(RequestErrors::NO_CONNECTION);
                res << Result::Meta("address", invocationFactory->defaultHostName());
                req->finishedError(res, nullptr);
            }
        }
        if (oldConnectionState == CONNECTED) {
            INFO("No connection to head server");
            emit q->disconnected();
        }

        doMainStateLoop();
    }

    /* */
    void doMainStateLoop()
    {
        //TRACE("doMainStateLoop()");

        // if ping ongoing then wait that to finish
        if (pingState == SENT_AND_WAITING) {
            //DEBUG("Ping wait state -> continue waiting");
            return;
        }

        if (connectionState == CONNECTED && !requestQueue.isEmpty()) {
            //DEBUG("Execute request");
            Request* req = requestQueue.dequeue();
            executeRequest(req);

        } else {
            //DEBUG("Schedule new ping");
            // nothing else -> schedule new ping after some wait time
            int pingInterval = q->property(HeadServerConnection::PING_INTERVAL_INT_PROP).toInt();
            // only old style connection available
            SystemServices::instance()->singleshotTimer(pingInterval, &(pingIntervalProxy), SLOT(proxyNoParameters()));
        }

        // there can be multiple requests ongoing
    }

    /* */
    void pingIntervalCheck()
    {
        // TODO: check time stamp; else doMainStateLoop()

        // any REST call is treated as ping -> could be ongoing
        if (pingState != SENT_AND_WAITING)
            ping();
    }

    /* */
    void executeRequest(Request* request)
    {

        Invocation* inv = request->prepareInvocation(invocationFactory);

        if (!inv) {
            ERROR("Request::prepareInvocation() returned NULL Invocation");
            return;
        }

        QObject::connect(inv, &Invocation::finishedOK,
                [=] () { this->requestReady(request, inv); });

        QObject::connect(inv, &Invocation::finishedError,
                [=] () { this->requestFailed(request, inv); });

        inv->execute();
    }

    /* */
    void requestReady(Request* request, Invocation* inv)
    {
        request->finishedOk(inv);
        this->onPingReady();
    }

    /* */
    void requestFailed(Request* request, Invocation* inv)
    {
        Result res(RequestErrors::INVOCATION_FAILED);
        if (inv)
            res << inv->result();

        request->finishedError(res, inv);

        switch (inv->statusCode()) {
            case Invocation::CONNECTION_FAILED:
            case Invocation::TIMEOUT_OCCURRED:
                // something failed -> redo ping
                ping();
                break;
            default:
                // nothing to do
                break;
        }
    }
};

HeadServerConnection::HeadServerConnection(InvocationFactory* invFactory, QObject *parent) :
    QObject(parent),
    _d(new HeadServerConnectionPrivate(this, invFactory))
{
    // defaults
    this->setProperty(PING_INTERVAL_INT_PROP, 60 * 1000); // 1 min
    this->setProperty(PING_TIMEOUT_INT_PROP, 2 * 60 * 1000); // 2 min
}

HeadServerConnection::~HeadServerConnection()
{
}

void HeadServerConnection::open()
{
    // ping opens connection
    _d->ping();
}

bool HeadServerConnection::isConnected() const
{
    return _d->connectionState == CONNECTED;
}

void HeadServerConnection::makeRequest(Request *request)
{
    _d->requestQueue.enqueue(request);

    // if there is no connection then try to ping first
    if (_d->connectionState == NOT_CONNECTED)
        _d->ping();
    else
        _d->doMainStateLoop();
}

} // eon
