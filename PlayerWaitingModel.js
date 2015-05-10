.pragma library

.import "Player.js" as Player
.import "ApplicationSettingsJS.js" as ApplicationSettingsJS
.import GBerry 1.0 as GBerry
.import GBerryConsole 1.0 as ConsoleLib // TODO: needed?

GBerry.Log.initLog("GameModel", ApplicationSettingsJS.logLevel)

/**
 * PlayerWaitingModel handle logic of waiting two players to join
 * the game. Once two players are joined then game starts.
 *
 * Note that just connecting to game is not "join". Player is
 * shown join call where they need to respond.
 */

// call back for winning
var callbacks = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal player1Joined(var name); signal player2Joined(var name) }', Qt.application, 'PlayerWaitingModelCallbacks');

var _player1 = new Player.UndefinedPlayer()
var _player2 = new Player.UndefinedPlayer()
var _playersManager

// ".pragma library" hides visiblity to global context
function initialize(playersManager) {
    _playersManager = playersManager
}

function reset() {
    _player1 = new Player.UndefinedPlayer()
    _player2 = new Player.UndefinedPlayer()
}

function numberOfJoinedPlayers() {
    if (!_player1.isValid()) {
        return 0
    } else if (!_player2.isValid()) {
        return 1
    } else {
        return 2
    }
}

function joinPlayer(playerId, playerName) {
    if (!_player1.isValid()) {
        _player1 = new Player.Player(playerId, playerName)
        callbacks.player1Joined(playerName) // signal

    } else if (!_player2.isValid()) {
        _player2 = new Player.Player(playerId, playerName)
        callbacks.player2Joined(playerName) // signal
    }
}

function joinedPlayers() {
    return new Player.Players(_player1, _player2)
}


function _sendMessageToPlayer(pid, msgJson) {
    var customMsg = ConsoleLib.Messages.createCustomAppBoxMsg(msgJson)
    _playersManager.sendPlayerMessage(pid, customMsg)
}

// appbox custom message
function playerMessageReceived(pid, js)  {
    if (!js.hasOwnProperty('action')) {
        console.debug("ERROR: invalid message:" + js.toString())
        return false
    }

    if (js["action"] === "JoinGame") {
        joinPlayer(pid, _playersManager.playerName(pid))
        var msgToSend

        if (numberOfJoinedPlayers() === 1) {
            msgToSend = {action: "MoveToState", state: "JOINED_WAITING"}
             _sendMessageToPlayer(pid, msgToSend)
        } else if (numberOfJoinedPlayers() === 2) {
            msgToSend = {action: "MoveToState", state: "JOINED_WAITING"}
            _sendMessageToPlayer(pid, msgToSend)

            // showing game area triggers showing game screen
        }

        return true

    }
    /*
    else if (js["action"] === "CancelJoin") {
        // TODO: problem if first player cancels ... -> back to original situation
        console.debug("CancelJoin unimplemented!")
        return true
    }
    */

    return false
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

//  -> feedback
// whe
