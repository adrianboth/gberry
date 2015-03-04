#ifndef MOCK_PLAYERSESSIONMANAGER_H
#define MOCK_PLAYERSESSIONMANAGER_H

#include "server/playersessionmanager.h"
#include "server/playersession.h"
#include <gmock/gmock.h>


class MockPlayerSessionManager : public PlayerSessionManager
{
public:

    MOCK_METHOD1(insertSession, void(PlayerSession&));
    MOCK_METHOD1(sessionByToken, PlayerSession(QString));
};


#endif // MOCK_PLAYERSESSIONMANAGER_H

