#ifndef PLAYERCONNECTIONMANAGER_H
#define PLAYERCONNECTIONMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QMap>

#include <server/playersession.h>

// TODO: this could be moved to folder called 'northside' i.e. towards mobapps

class WebsocketServer;
class ServerChannelManager;
class ServerSidePlayerChannel;
class PlayerConnectionManagerPrivate;
class PlayerChannelNorthPartnerImpl;

/**
 * @brief The PlayerConnectionManager class
 *
 * On north side of server communication:
 *
 * ConsoleRESTServer listens HTTP rest calls. Mobapp clients register and get
 * a code that can be used to open websocket connection. That connection
 * forms a player channel by PlayerConnectionManager.
 *
 * Once connection is established, PlayerConnectionManager is responsible
 * for managing it on north side: messages and disconnects.
 *
 * Note that south side never disconnects, instead player connections are
 * kept open and reopened on the south side when that is approriate.
 *
 */
class PlayerConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerConnectionManager(WebsocketServer* websocketServer,
                                     ServerChannelManager* channelManager,
                                     QObject *parent = 0);
    ~PlayerConnectionManager();

public slots:
    void onNewPlayerConnection(PlayerSession session);
    void onPlayerConnectionClosed(int playerId);
    void onPlayerMessageReceivedFromNorth(int playerId, QString message);

protected:
    friend class PlayerChannelNorthPartnerImpl;
    void playerMessageFromSouth(int channelId, const QByteArray& msg);

private:
    const QScopedPointer<PlayerConnectionManagerPrivate> _d;

};

#endif // PLAYERCONNECTIONMANAGER_H
