#include "headserverconnection.h"

#include <QQueue>

#include "request.h"
#include "restinvocationfactory.h"
#include "restinvocation.h"
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
                                RESTInvocationFactory* invocationFactory_) :
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
    RESTInvocationFactory* invocationFactory;
    QQueue<Request*> requestQueue;
    int lastPingTimestamp{0};
    QtSignalProxy pingIntervalProxy;

    /* */
    void ping()
    {
        RESTInvocation* invocation = invocationFactory->newInvocation();
        // invocation gets deleted once we get response

        QObject::connect(invocation, &RESTInvocation::finishedOK,
                [=] () { this->onPingReady(invocation); });

        QObject::connect(invocation, &RESTInvocation::finishedError,
                [=] () { this->onPingError(invocation); });

        invocation->get("/ping");
        pingState = SENT_AND_WAITING;
    }

    /* When ping passes succesfully. We don't really care of about response data. */
    void onPingReady(RESTInvocation* invocation)
    {
        ConnectionState oldConnectionState = connectionState;
        connectionState = CONNECTED;
        pingState = IDLE_WAITING;
        invocation->deleteLater();

        if (oldConnectionState == NOT_CONNECTED) {
            INFO("Connection to head server established");
            emit q->connected();
        }

        doMainStateLoop();
    }

    /* When ping REST call fails*/
    void onPingError(RESTInvocation* invocation)
    {
        ConnectionState oldConnectionState = connectionState;
        connectionState = NOT_CONNECTED;
        pingState = IDLE_WAITING;
        invocation->deleteLater();

        // all requests waiting to be served will fail because there is no connection
        if (!requestQueue.isEmpty()) {
            // if there for some reason comes new request while handling current ones
            // do not fail those new ones.
            QQueue<Request*> currentlyWaitingRequests = requestQueue;
            requestQueue.empty();

            foreach(auto req, currentlyWaitingRequests) {
                req->finishedError(invocation); // TODO: should we mark somehow that there was connection error in ping?
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
        // if ping ongoing then wait that to finish
        if (pingState == SENT_AND_WAITING) {
            return;
        }

        if (connectionState == CONNECTED && !requestQueue.isEmpty()) {
            Request* req = requestQueue.dequeue();
            executeRequest(req);

        } else {

            // nothing else -> schedule new ping after some wait time
            int pingInterval = q->property(HeadServerConnection::PING_INTERVAL_INT_PROP).toInt();
            // only old style connection available
            SystemServices::instance()->singleshotTimer(pingInterval, &(pingIntervalProxy), SLOT(proxyNoParameters()));

        }

        // TODO: what about if request ongoing

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
        RESTInvocation* invocation = invocationFactory->newInvocation();
        // invocation gets deleted once we get response

        QObject::connect(invocation, &RESTInvocation::finishedOK,
                [=] () { this->requestReady(request, invocation); });

        QObject::connect(invocation, &RESTInvocation::finishedError,
                [=] () {
             this->requestFailed(request, invocation);
        });

        request->prepare(invocation);
        pingState = SENT_AND_WAITING; // any REST call is treated as PING
    }

    /* */
    void requestReady(Request* request, RESTInvocation* invocation)
    {
        // TODO: how streaming goes ...
        request->finishedOk(invocation);
        this->onPingReady(invocation); // will deleteLater() 'invocation'
    }

    /* */
    void requestFailed(Request* request, RESTInvocation* invocation)
    {
        request->finishedError(invocation);
        this->onPingError(invocation); // will deleteLater() 'invocation'
    }
};

HeadServerConnection::HeadServerConnection(RESTInvocationFactory* invFactory, QObject *parent) :
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
