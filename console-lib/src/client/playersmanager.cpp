#include "playersmanager.h"

PlayersManager::PlayersManager(QObject *parent) :
    QObject(parent), _playerCount(0)
{

}

PlayersManager::~PlayersManager()
{

}

int PlayersManager::numberOfPlayers()
{
    return _playerCount;
}

void PlayersManager::newPlayer(int channelId)
{
    _playerCount++;
}

void PlayersManager::playerExit(int channelId)
{
    _playerCount--;
}
