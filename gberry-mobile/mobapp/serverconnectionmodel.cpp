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
