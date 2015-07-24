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
 
 #ifndef SERVERCONNECTIONMODEL_H
#define SERVERCONNECTIONMODEL_H

#include <QObject>

// TODO: rename to ConnectionModel
class ServerConnectionModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint serverPingOKCounter READ serverPingOKCounter NOTIFY serverCounterChanged)
    Q_PROPERTY(uint serverPingFailureCounter READ serverPingFailureCounter NOTIFY serverCounterChanged)
    Q_PROPERTY(bool serverConnectionOK READ serverConnectionOK NOTIFY serverStatusChanged)

    Q_PROPERTY(uint consolePingOKCounter READ consolePingOKCounter NOTIFY consoleCounterChanged)
    Q_PROPERTY(uint consolePingFailureCounter READ consolePingFailureCounter NOTIFY consoleCounterChanged)
    Q_PROPERTY(bool consoleConnectionOK READ consoleConnectionOK NOTIFY consoleStatusChanged)

public:
    explicit ServerConnectionModel(QObject *parent = 0);
    ~ServerConnectionModel();

    // property functions
    uint serverPingOKCounter() const { return _serverPingOKCounter; }
    uint serverPingFailureCounter() const { return _serverPingFailureCounter; }
    bool serverConnectionOK() const { return _serverConnectionOK; }
    uint consolePingOKCounter() const { return _consolePingOKCounter; }
    uint consolePingFailureCounter() const { return _consolePingFailureCounter; }
    bool consoleConnectionOK() const { return _consoleConnectionOK; }

signals:
    void serverCounterChanged();
    void consoleCounterChanged();
    void serverStatusChanged();
    void consoleStatusChanged();

public slots:
    void serverPingOK();
    void serverPingFailure();
    void serverConnected();
    void serverDisconnected();

    void consolePingOK();
    void consolePingFailure();
    void consoleConnected();
    void consoleDisconnected();

private:
    bool _serverConnectionOK;
    bool _serverPingReceived;
    uint _serverPingOKCounter;
    uint _serverPingFailureCounter;

    bool _consoleConnectionOK;
    bool _consolePingReceived;
    uint _consolePingOKCounter;
    uint _consolePingFailureCounter;

};

#endif // SERVERCONNECTIONMODEL_H
