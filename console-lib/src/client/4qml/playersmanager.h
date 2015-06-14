#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "client/clientsideplayerchannel.h"
#include "playermeta.h"

class PlayersManagerPrivate;
class ClientSidePlayerChannelPartnerImpl;


class PlayersManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int numberOfPlayers READ numberOfPlayers NOTIFY numberOfPlayersChanged)



public:
    explicit PlayersManager(QObject *parent = 0);
    ~PlayersManager();

    int numberOfPlayers() const;
    Q_INVOKABLE QList<int> playerIds() const;
    Q_INVOKABLE QString playerName(int playerId) const;

    // TODO: for now player id matches to channel id, but later it should be different
    //       as player might disconnect for awhile (in phone, e.g.) but then come
    //       back and channel id will change -> game might have kept some information
    //       stored and player can continue if player ids match

    Q_INVOKABLE void sendPlayerMessage(int playerId, const QByteArray msg);
    Q_INVOKABLE void sendAllPlayersMessage(QByteArray msg);


signals:
    void playerIn(int playerId, QString playerName);
    void playerOut(int playerId, QString playerName);
    void numberOfPlayersChanged();
    void playerMessageReceived(int playerId, QByteArray msg); // TODO: might need to be string

public slots:
    void newPlayer(ClientSidePlayerChannel* channel, PlayerMeta metadata);

protected:
    friend class ClientSidePlayerChannelPartnerImpl;

    // TODO: Q.object to hide these
    // from ClientSidePlayerChannelPartner
    void playerMessage(int channelId, const QByteArray& msg);
    void playerExit(int channelId);

private:
    const QScopedPointer<PlayersManagerPrivate> _d;
};


#endif // PLAYERSMANAGER_H
