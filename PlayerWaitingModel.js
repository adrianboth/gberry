.import "ApplicationSettingsJS.js" as ApplicationSettingsJS
.import GBerry 1.0 as GBerry
.import GBerryConsole 1.0 as ConsoleLib

GBerry.Log.initLog("GameModel", ApplicationSettingsJS.logLevel)

/**
 * PlayerWaitingModel handle logic of waiting two players to join
 * the game. Once two players are joined then game starts.
 *
 * Note that just connecting to game is not "join". Player is
 * shown join call where they need to respond.
 */

// call back for winning
var callbacks = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal playerWon(var pid); signal playerCorrectNumber(var pid); signal playerInvalidNumber(var pid); }', Qt.application, 'PlayerWaitingModelCallbacks');


function func() {

}

// handle player joined message
//  -> when player connect to game
//      -> if waiting players push him join button
//      -> if game on going push him "game ongoing, please wait to join"
//

// handle appbox message: join
//  -> 1st: Push player a message "joined"
//  -> trigger some kind animation on TV screen
//
//  -> 2nd: same as above but after 3s start game (enough for possible animation)
//     TODO: who handles timer?
//

// when game ends
//  - clear states
//  - push for players joining message


// how about to general action for stopping game, where is the logic?

/
//  -> feedback
// whe
