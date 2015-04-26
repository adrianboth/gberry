#include "consolesessionmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "restinvocationfactoryimpl.h"
#include "restinvocation.h"
#include "client/websocketclient.h"


using namespace mobile;

ConsoleSessionManager::ConsoleSessionManager(QObject *parent) : QObject(parent), _restInvocationFactory(NULL)
{
    connect(&_websocket, &WebsocketClient::connected,
            this, &ConsoleSessionManager::onWebsocketConnected);
}

ConsoleSessionManager::~ConsoleSessionManager()
{
}

void ConsoleSessionManager::open(ConsoleDevice console, QString playerName)
{
    initRESTInvocationFactory(console);
    _hostAddress = console.host();

    // first get token

    QJsonObject json;
    json["action"] = "open_console_session";
    json["id"] = "guest";
    json["name"] = playerName;

    QJsonDocument jsondoc(json);

    RESTInvocation* inv = _restInvocationFactory->newInvocation();
    connect(inv, &RESTInvocation::finishedOK,
            this, &ConsoleSessionManager::onOpenConsoleSessionFinished);

    connect(inv, &RESTInvocation::finishedError,
            this, &ConsoleSessionManager::onOpenConsoleSessionError);

    inv->post("/session", jsondoc);
}

void ConsoleSessionManager::onOpenConsoleSessionFinished(RESTInvocation* inv)
{
    // console might have responded an http error
    if (inv->responseHttpStatusCode() != RESTInvocation::OK_200)
    {
        emit consoleSessionOpenFailed(QString("HTTP Status Code:").append(inv->responseHttpStatusCode()));
        return;
    }

    // or error data
    QJsonDocument jsondoc(QJsonDocument::fromJson(inv->responseString().toLatin1()));
    QJsonObject json = jsondoc.object();

    if (json.contains("response_type") && json["response_type"] == "error")
    {
        QString errMsg = json["error_message"].toString();
        emit consoleSessionOpenFailed(QString("Console responded:") + errMsg);
    }

    if (json.contains("response_type") && json["response_type"] == "ok")
    {
        QString token = json["token"].toString(); // TODO: validation of this
        openWebsocket(token);
    }
}

void ConsoleSessionManager::onOpenConsoleSessionError(RESTInvocation* inv)
{
    emit consoleSessionOpenFailed(QString(inv->responseString().toLatin1()));
}

void ConsoleSessionManager::openWebsocket(QString token)
{
    QUrl url(QString("ws://%1:8888/open?token=%2").arg(_hostAddress).arg(token));
    _websocket.open(url);
}


void ConsoleSessionManager::onWebsocketConnected()
{
    // session now on
    emit consoleSessionOpened();
}


void ConsoleSessionManager::close()
{
    _websocket.close();
}

void ConsoleSessionManager::initRESTInvocationFactory(ConsoleDevice console)
{
    if (_restInvocationFactory)
    {
        delete _restInvocationFactory;
    }
    _restInvocationFactory = new RESTInvocationFactoryImpl();
    QString urlPrefix("http://%1:8050/console/v1");
    _restInvocationFactory->setProperty("url_prefix", urlPrefix.arg(console.host()));
}
