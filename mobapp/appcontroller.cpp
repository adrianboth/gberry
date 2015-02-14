#include "appcontroller.h"

#include <QDebug>
#include <QTimer>

// TODO: private impl

AppController::AppController() : QObject(NULL),
  _serverConnectionOK(false),
  _serverPingReceived(false),
  _serverPingOKCounter(0),
  _serverPingFailureCounter(0),
  _consoleConnectionOK(false),
  _consolePingReceived(false),
  _consolePingOKCounter(0),
  _consolePingFailureCounter(0)
{
    // TODO: some kind of timer to check state of query and notice that no connection
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    _timer->start(1000);

    connect(&_serverQuery, &RESTQuery::queryFinished,
            this,         &AppController::serverQueryReady);

    connect(&_consoleQuery, &RESTQuery::queryFinished,
            this,          &AppController::consoleQueryReady);
}

AppController::~AppController()
{
    // TODO: clean up resources for tests

    _timer->stop();
}

void AppController::timerUpdate()
{
    qDebug() << "TIMER";

    // check if ping has been received
    //  - if not change status to NOK
    if ( !_serverPingReceived )
    {
        bool prev = _serverConnectionOK;
        _serverConnectionOK = false;
        if (prev != _serverConnectionOK)
            emit serverStatusChanged();

        // TODO: some how cancel request
    }

    // schedule new ping (TODO: new object?
    _serverPingReceived = false;
    _serverQuery.startQuery(QUrl("http://localhost:9000/gberryrest/v1/ping/"));

    if ( !_consolePingReceived )
    {
        bool prev = _consoleConnectionOK;
        _consoleConnectionOK = false;
        if (prev != _consoleConnectionOK)
            emit consoleStatusChanged();

        // TODO: some how cancel request
    }

    _consolePingReceived = false;
    _consoleQuery.startQuery(QUrl("http://localhost:8050/console/v1/ping"));
    // TODO: connect
}

// TODO: handling error situation

void AppController::serverQueryReady(QString data)
{
    qDebug() << "SERVER QUERY RECEIVED";

    // TODO: read data

    // TODO: check data and status_code
    if (data.length() > 0)
    {
        _serverPingReceived = true;
        _serverConnectionOK = true;
        _serverPingOKCounter++;
    }
    else
    {
        // TODO: we know ping failed -> should we inform that some to timerUpdate() ... it tries to cancel query
        _serverPingFailureCounter++;
    }
    emit serverCounterChanged();
}


void AppController::consoleQueryReady(QString data)
{
    qDebug() << "CONSOLE QUERY RECEIVED";

    // TODO: read data

    // TODO: check data and status_code
    if (data.length() > 0)
    {
        _consolePingReceived = true;
        _consoleConnectionOK = true;
        _consolePingOKCounter++;
    }
    else
    {
        _consolePingFailureCounter++;
    }
    emit consoleCounterChanged();
}

