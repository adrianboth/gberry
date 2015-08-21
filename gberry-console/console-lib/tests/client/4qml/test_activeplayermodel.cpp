/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */

#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include "client/4qml/activeplayermodel.h"
using namespace GBerryApplication;

TEST(ActivePlayerModel, BasicCases)
{
    ActivePlayerModel model;

    int activePlayerIdChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::activePlayerIdChanged,
                     [&] () { activePlayerIdChangedCounter++; });

    int hasActivePlayerChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::hasActivePlayerChanged,
                     [&] () { hasActivePlayerChangedCounter++; });

    int activePlayerIsGuestChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::activePlayerIsGuestChanged,
                     [&] () { activePlayerIsGuestChangedCounter++; });

    // -- initial state
    ASSERT_FALSE(model.hasActivePlayer());
    ASSERT_EQ(0, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    ASSERT_FALSE(model.isActivePlayerDevelopmentUser());

    // -- guest activated
    model.activatePlayer(-10); // guest
    ASSERT_TRUE(model.hasActivePlayer());
    ASSERT_EQ(-10, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    ASSERT_FALSE(model.isActivePlayerDevelopmentUser());

    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 1);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 1);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 0); // no change in value

    model.deactivatePlayer();
    ASSERT_FALSE(model.hasActivePlayer());
    ASSERT_EQ(0, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 2);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 2);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 0); // no change in value


    // -- normal user activated
    activePlayerIdChangedCounter = 0;
    hasActivePlayerChangedCounter = 0;
    activePlayerIsGuestChangedCounter = 0;

    model.activatePlayer(5);
    ASSERT_TRUE(model.hasActivePlayer());
    ASSERT_EQ(5, model.activePlayerId());
    ASSERT_FALSE(model.activePlayerIsGuest());
    ASSERT_FALSE(model.isActivePlayerDevelopmentUser());

    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 1);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 1);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 1);

    model.deactivatePlayer();
    ASSERT_FALSE(model.hasActivePlayer());
    ASSERT_EQ(0, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 2);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 2);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 2);

    // -- development user activated
    activePlayerIdChangedCounter = 0;
    hasActivePlayerChangedCounter = 0;
    activePlayerIsGuestChangedCounter = 0;

    model.activatePlayer(-1);
    ASSERT_TRUE(model.hasActivePlayer());
    ASSERT_EQ(-1, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    ASSERT_TRUE(model.isActivePlayerDevelopmentUser());

    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 1);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 1);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 0);

    model.deactivatePlayer();
    ASSERT_FALSE(model.hasActivePlayer());
    ASSERT_EQ(0, model.activePlayerId());
    ASSERT_TRUE(model.activePlayerIsGuest());
    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 2);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 2);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 0);
}

TEST(ActivePlayerModel, SequenceFromGuestToUser)
{
    ActivePlayerModel model;

    int activePlayerIdChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::activePlayerIdChanged,
                     [&] () { activePlayerIdChangedCounter++; });

    int hasActivePlayerChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::hasActivePlayerChanged,
                     [&] () { hasActivePlayerChangedCounter++; });

    int activePlayerIsGuestChangedCounter = 0;
    QObject::connect(&model, &ActivePlayerModel::activePlayerIsGuestChanged,
                     [&] () { activePlayerIsGuestChangedCounter++; });

    // -- guest activated
    model.activatePlayer(-10); // guest
    model.activatePlayer(5);

    ASSERT_TRUE(model.hasActivePlayer());
    ASSERT_EQ(5, model.activePlayerId());
    ASSERT_FALSE(model.activePlayerIsGuest());

    WAIT_AND_ASSERT( activePlayerIdChangedCounter == 2);
    WAIT_AND_ASSERT( hasActivePlayerChangedCounter == 1);
    WAIT_AND_ASSERT( activePlayerIsGuestChangedCounter == 1);
}
