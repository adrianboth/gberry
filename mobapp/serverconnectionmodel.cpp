#include "serverconnectionmodel.h"

ServerConnectionModel::ServerConnectionModel(QObject *parent) : QObject(parent),
    _serverConnectionOK(false),
    _serverPingReceived(false),
    _serverPingOKCounter(0),
    _serverPingFailureCounter(0),
    _consoleConnectionOK(false),
    _consolePingReceived(false),
    _consolePingOKCounter(0),
    _consolePingFailureCounter(0)
{

}

ServerConnectionModel::~ServerConnectionModel()
{

}

void ServerConnectionModel::serverPingOK()
{
    _serverPingOKCounter++;
    emit serverCounterChanged();
}

void ServerConnectionModel::serverPingFailure()
{
    _serverPingFailureCounter++;
    emit serverCounterChanged();
}

void ServerConnectionModel::serverConnected()
{
    if (!_serverConnectionOK)
        emit serverStatusChanged();

    _serverConnectionOK = true;
}

void ServerConnectionModel::serverDisconnected()
{
    if (_serverConnectionOK)
        emit serverStatusChanged();

    _serverConnectionOK = false;
}

void ServerConnectionModel::consolePingOK()
{
    _consolePingOKCounter++;
    emit consoleCounterChanged();
}

void ServerConnectionModel::consolePingFailure()
{
    _consolePingFailureCounter++;
    emit consoleCounterChanged();
}

void ServerConnectionModel::consoleConnected()
{
    if (!_consoleConnectionOK)
        emit consoleStatusChanged();

    _consoleConnectionOK = true;
}

void ServerConnectionModel::consoleDisconnected()
{
    if (_consoleConnectionOK)
        emit consoleStatusChanged();

    _consoleConnectionOK = false;
}
