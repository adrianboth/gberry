#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include <QObject>

//TODO: move to server/ folder

class PlayersManager : public QObject
{
    Q_OBJECT

public:
    explicit PlayersManager(QObject *parent = 0);
    ~PlayersManager();

    int numberOfPlayers();

signals:

public slots:
    void newPlayer(int channelId);
    void playerExit(int channelId);

private:
    // TODO: this is just temp
    int _playerCount;
};

// TODO: handle metadata

#endif // PLAYERSMANAGER_H
