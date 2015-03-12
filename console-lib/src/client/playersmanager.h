#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include <QObject>
#include <QMap>
#include <QJsonObject>

class PlayerChannel;


// TODO: make this qml compatible
class PlayerMetadata
{
public:
    PlayerMetadata() : playerId(-1), playerName("UNKNOWN") {}
    PlayerMetadata(int pid, QString name) :
        playerId(pid), playerName(name) {}

    PlayerMetadata(QJsonObject metaJson)
    {
        this->playerId = metaJson["id"].toInt();
        this->playerName = metaJson["name"].toString();
    }

    PlayerMetadata(const PlayerMetadata& other)
    {
        this->playerId = other.playerId;
        this->playerName = other.playerName;
    }

    PlayerMetadata& operator=(const PlayerMetadata& other)
    {
        this->playerId = other.playerId;
        this->playerName = other.playerName;
        return *this;
    }

    QJsonObject json()
    {
        QJsonObject json;
        json["id"] = playerId;
        json["name"] = playerName;
        return json;
    }

    int playerId;
    QString playerName;
};

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

    Q_INVOKABLE void sendPlayerMessage(int playerId, QByteArray msg);
    Q_INVOKABLE void sendAllPlayersMessage(QByteArray msg);


signals:
    void playerIn(int playerId);
    void playerOut(int playerId);
    void numberOfPlayersChanged();
    void playerMessageReceived(int playerId, QByteArray msg); // TODO: might need to be string

public slots:
    void newPlayer(PlayerChannel* channel, PlayerMetadata metadata);

    // handle incoming player message
    void playerMessage(int channelId, QByteArray msg);
    void playerExit(int channelId);

private:
    // TODO: this is just temp
    int _playerCount;
    QMap<int, PlayerChannel*> _channelsByPlayerId;
    QMap<int, PlayerMetadata> _meta;
    QMap<int, int> _playerIdsByChannelId;
    QMap<int, int> _channelIdsByPlayerIds;
};

// TODO: handle metadata

#endif // PLAYERSMANAGER_H
