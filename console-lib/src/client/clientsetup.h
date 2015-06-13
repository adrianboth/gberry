#ifndef CLIENTSETUP_H
#define CLIENTSETUP_H

#include <QObject>
#include "commtcpclient.h"
#include "client/4qml/playersmanager.h"
#include "client/clientchannelmanager.h"
#include "client/clientsidecontrolchannel.h"


class ClientSetup : public QObject
{
    Q_OBJECT

public:
    ClientSetup(QObject* parent = 0);
    ~ClientSetup();

    void start();

    CommTcpClient tcpClient;
    ClientChannelManager channelManager;
    ClientSideControlChannel controlChannel;
    PlayersManager playersManager;

public slots:
    void connectionEstablished();
    void connectionBroken();

};

#endif // CLIENTSETUP_H
