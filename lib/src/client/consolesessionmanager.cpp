#include "consolesessionmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "invocationfactoryimpl.h"
#include "restinvocation.h"
#include "client/websocketclient.h"
#include "resultmessageformatter.h"


#define LOG_AREA "ConsoleSessionManager"
#include "log/log.h"

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

    RESTInvocation* inv = _restInvocationFactory->newRESTInvocation();
    connect(inv, &RESTInvocation::finishedOK,
            this, &ConsoleSessionManager::onOpenConsoleSessionFinished);

    connect(inv, &RESTInvocation::finishedError,
            this, &ConsoleSessionManager::onOpenConsoleSessionError);

    inv->definePostOperation("/session", jsondoc);
    inv->execute();
}

void ConsoleSessionManager::onOpenConsoleSessionFinished(Invocation* invocation)
{
    RESTInvocation* inv = qobject_cast<RESTInvocation*>(invocation);
    if (!inv) {
        ERROR("Failed to cast to RESTInvocation");
        invocation->deleteLater();
        return;
    }

    // console might have responded an http error
    if (inv->responseHttpStatusCode() != HTTPInvocationDefinition::OK_200)
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

    inv->deleteLater();
}

void ConsoleSessionManager::onOpenConsoleSessionError(Invocation* invocation)
{
    RESTInvocation* inv = qobject_cast<RESTInvocation*>(invocation);
    if (!inv) {
        // TODO: this is like DeveloperError
        ERROR("Failed to cast to RESTInvocation");
        invocation->deleteLater();
        return;
    }

    // TODO: there can be different reasons for error
    // TODO: how to localize them all for all (well maybe not all are needed?)
    QString err;
    if (inv->responseString().length() > 0) {
        // TODO: this would more line error response from the console
        err = inv->responseString();
        WARN("Console session open received error response:" << err);
    } else {
        // this is more like failed connection (e.g. connection problem)
        ResultMessageFormatter msgCreator(inv->result());
        WARN("Console session error:" << msgCreator.createDeveloperMessage());
        err = msgCreator.createEndUserMessage(); // TODO: localization
    }

    emit consoleSessionOpenFailed(err);
    inv->deleteLater();
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
    _restInvocationFactory = new InvocationFactoryImpl();
    QString urlPrefix("http://%1:8050/console/v1");
    _restInvocationFactory->setProperty("url_prefix", urlPrefix.arg(console.host()));
}
