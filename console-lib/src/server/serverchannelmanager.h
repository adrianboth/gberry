#ifndef SERVERCHANNELMANAGER_H
#define SERVERCHANNELMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "playermeta.h"

namespace GBerry {
namespace Console {
namespace Server {
    class ChannelFactory;
}}}

using namespace GBerry::Console::Server;

class ServerSidePlayerChannel;
class ServerSideControlChannel;
class ServerChannelManagerPrivate;
class PlayerChannelSouthPartner;

class ServerChannelManager : public QObject
{
    Q_OBJECT

public:
    ServerChannelManager(ChannelFactory* factory, QObject* parent = 0);
    ~ServerChannelManager();

    int nextFreeChannelId();
    ServerSidePlayerChannel* openPlayerChannel(PlayerMeta metadata);
    ServerSideControlChannel* openControlChannel(int connectionId);

    // If channel is open then it is closed before unattaching
    // ChannelHandler from channel.
    void closePlayerChannel(int channelId);
    void closeControlChannel(int connectionId);

    QList<int> allPlayerChannelIds();

    void activateConnection(int connectionId);
    bool hasActiveConnection();

public slots:
    void applicationClosed(int connectionId); // connection towards client closed
    void processMessageFromSouth(int connectionId, int channelId, const QByteArray& msg);

signals:
    void outgoingMessageToSouth(int connectionId, int channelId, const QByteArray msg);

private:
    friend class PlayerChannelSouthPartnerImpl;
    const QScopedPointer<ServerChannelManagerPrivate> _d;
};

#endif // SERVERCHANNELMANAGER_H
