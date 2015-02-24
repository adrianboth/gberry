#include "playersmanager.h"

PlayersManager::PlayersManager(QObject *parent) : QObject(parent)
{

}

PlayersManager::~PlayersManager()
{

}

int PlayersManager::numberOfPlayers()
{
    return -1;
}

void PlayersManager::newPlayer(int channelId)
{

}

void PlayersManager::playerExit(int channelId)
{

}
