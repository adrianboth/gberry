#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QObject>
#include "commtcpserver.h"
#include "server/serversidechannelmanager.h"
#include "server/serversidecontrolchannel.h"


class ServerSetup : public QObject
{
    Q_OBJECT

public:
    ServerSetup(QObject* parent = 0);
    ~ServerSetup();

    void start();

    CommTcpServer tcpServer;
    ServerSideChannelManager channelManager;
    ServerSideControlChannel controlChannel;

};

#endif // SERVERSETUP_H
