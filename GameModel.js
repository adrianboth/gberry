.pragma library

.import "Player.js" as Player // actually just handling objects not creating

.import "ApplicationSettingsJS.js" as ApplicationSettingsJS
.import GBerry 1.0 as GBerry
.import GBerryConsole 1.0 as ConsoleLib

GBerry.Log.initLog("GameModel", ApplicationSettingsJS.logLevel)

// ?? how game actually starts
//    - first timer

var _columns = []
var _player1ID, _player2ID, _currentPlayer
var _player1, _player2 // TODO: these will replace above *ID

var _markedCells // helper variable to know draw condition
var _gameEnded
var _enableTurns

// call back for winning
var callbacks = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal playerWon(var pid); signal gameEndedDraw(); signal gameEnded(var playerNumber, var x1, var y1, var x2, var y2); signal cellMarked(var type, var x, var y) }', Qt.application, 'ModelCallbacks');

var callbackTurnSwitched = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal turnSwitched(var playerNumber) }', Qt.application, 'ModelCallbacks');


var _playersManager

function initialize(playersManager) {
    _playersManager = playersManager
    _player1 = new Player.UndefinedPlayer()
    _player2 = new Player.UndefinedPlayer()
}

function initGame(joinedPlayers) {
    _player1 = joinedPlayers.player1
    _player2 = joinedPlayers.player2

    _player1ID = _player1.id // "x"
    _player2ID = _player2.id // "o"
    _currentPlayer = _player1ID
    _markedCells = 0
    _gameEnded = false
    _enableTurns = false

    _columns = []
    for (var x = 0; x < 3; x++) {
        var currentList = []
        _columns.push(currentList)
        for (var y = 0; y < 3; y++) {
            currentList.push("_") // means not selected
        }
    }
    GBerry.Log.debug("Model initialized")
}

function player1() {
    return _player1
}

function player2() {
    return _player2
}

function isPlaying(playerID) {
    if (_player1.isValid() && _player1.id === playerID)
        return true
    if (_player2.isValid() && _player2.id === playerID)
        return true

    return false
}

function enableTurns() {
    _enableTurns = true
}

function markCell(x, y) {
    if (_gameEnded)
        return false

    // should know which player is in turn
    var cell = _columns[x][y]
    if (cell !== "_") {
        console.error("Tried to mark already marked cell:" + x.toString() + "," + y.toString())
        return false
    }

    if (_currentPlayer === _player1ID) {
        _columns[x][y] = "x"
    } else {
        _columns[x][y] = "o"
    }

    callbacks.cellMarked(_columns[x][y], x, y)

    // yes, marked -> check marking info for both players
    var msg = {action: "MarkCell", x: x, y: y}
    _sendMessageToPlayer(_player1ID, msg)
    _sendMessageToPlayer(_player2ID, msg)

    _markedCells++
    debugBoard()

    if (!checkGameEndCondition(x, y))
    {
        // game not ended yet
        switchPlayerTurn()
    }

    return true
}

function debugBoard() {
    console.debug("Board: marked=" + _markedCells.toString())
    for (var y = 0; y < 3; y++) {
        var row = ""
        for (var x = 0; x < 3; x++) {
            row = row + _columns[x][y]
        }
        console.debug(row)
    }
}

function isCrossItem(x, y) {
    return _columns[x][y] === "x"
}


function checkGameEndCondition(x, y) {
    console.debug("END GAME CHECK")

    if (y === 0 && checkRow(0, 0, 2, 0)) return true
    if (y === 1 && checkRow(0, 1, 2, 1)) return true
    if (y === 2 && checkRow(0, 2, 2, 2)) return true

    if (x === 0 && checkColumn(0, 0, 0, 2)) return true
    if (x === 1 && checkColumn(1, 0, 1, 2)) return true
    if (x === 2 && checkColumn(2, 0, 2, 2)) return true

    if (x === y && checkDiagonal(0, 0, 2, 2)) return true

    if (checkDiagonal(0, 2, 2, 0)) return true

    // checking draw needs to be last as the last selection
    // might be winning entry
    if (_markedCells === 9) {
        console.debug("Draw game!")
        _endGameProcedures()
        callbacks.gameEndedDraw()
        return true
    }

    return false
}

function _endGameProcedures() {
    _gameEnded = true
    _enableTurns = false
    sendEndGameMessages()
    _player1 = new Player.UndefinedPlayer()
    _player2 = new Player.UndefinedPlayer()
}

function checkRow(x1, y1, x2, y2) {
    var type = playerMark()

    // if ends are not expected type ...
    if (_columns[x1][y1] !==  type)
        return false

    if (_columns[x2][y2] !==  type)
        return false

    // this is row - check middle cell
    if (y1 === y2 && _columns[x2/2][y1] === type) {
        // yes, row match
        _endGameProcedures()
        callbacks.gameEnded(playerNumber(), x1, y1, x2, y2)
        return true
    }
    return false
}

function checkColumn(x1, y1, x2, y2) {
    var type = playerMark()

    // if ends are not expected type ...
    if (_columns[x1][y1] !==  type)
        return false

    if (_columns[x2][y2] !==  type)
        return false

    // this is row - check middle cell
    if (x1 === x2 && _columns[x1][y2/2] === type) {
        // yes, row match
        _endGameProcedures()
        callbacks.gameEnded(playerNumber(), x1, y1, x2, y2)
        return true
    }
    return false
}

function checkDiagonal(x1, y1, x2, y2) {
    console.debug("CHECK DIAGONAL: " + x1 + "," + y1 + " - " + x2 + "," + y2)
    var type = playerMark()

    // if ends are not expected type ...
    if (_columns[x1][y1] !== type)
        return false

    if (_columns[x2][y2] !== type)
        return false

    // this is diagonal line - check middle cell
    if (_columns[x2/2][Math.max(y1, y2)/2] === type) {
        // yes, row match
        _endGameProcedures()
        callbacks.gameEnded(playerNumber(), x1, y1, x2, y2)
        return true
    }
    return false
}

function isGameEnded() {
    return _gameEnded
}

function playerNumber() {
    if (_currentPlayer === _player1ID)
        return 1
    else
        return 2
}

function playerMark() {
    if (_currentPlayer === _player1ID) {
        return "x"
    } else {
        return "o"
    }
}

function switchPlayerTurn() {
    console.debug("Switching turns! current=" + _currentPlayer)
    var msgTurn = {action: "MoveToState", state: "PLAY_GAME_MY_TURN"}
    var msgWait = {action: "MoveToState", state: "PLAY_GAME_WAIT_TURN"}

    if (_currentPlayer === _player1ID) {
        console.debug("Next player is " + _player2ID)
        _currentPlayer = _player2ID
        callbackTurnSwitched.turnSwitched(2)

        _sendMessageToPlayer(_player1ID, msgWait)
        _sendMessageToPlayer(_player2ID, msgTurn)

    } else {
        console.debug("Next player is " + _player1ID)
        _currentPlayer = _player1ID
        callbackTurnSwitched.turnSwitched(1)

        _sendMessageToPlayer(_player1ID, msgTurn)
        _sendMessageToPlayer(_player2ID, msgWait)
    }
}



function _sendMessageToPlayer(pid, msgJson) {
    var customMsg = ConsoleLib.Messages.createCustomAppBoxMsg(msgJson)
    _playersManager.sendPlayerMessage(pid, customMsg)
}

function sendEndGameMessages() {
    var msgEndGameWinner = {action: "MoveToState", state: "END_GAME_WINNER"}
    var msgEndGameLooser = {action: "MoveToState", state: "END_GAME_LOOSER"}
    var winner = playerNumber()
    if (winner === 1) {
        _sendMessageToPlayer(_player1ID, msgEndGameWinner)
        _sendMessageToPlayer(_player2ID, msgEndGameLooser)
    } else {
        _sendMessageToPlayer(_player1ID, msgEndGameLooser)
        _sendMessageToPlayer(_player2ID, msgEndGameWinner)
    }

}

// appbox custom message
function playerMessageReceived(pid, js) {
    GBerry.Log.debug("Player message: id = " + pid)

    if (!js.hasOwnProperty('action')) {
        GBerry.Log.error("Invalid message: " + js.toString())
        return false
    }

    if (js["action"] === "SelectCell" && !_enableTurns)
        return true // we don't handle yet turns

    if (js["action"] === "SelectCell") {
        // check turn
        if (pid === _currentPlayer) {
            markCell(js["x"], js["y"])

        } else {
            GBerry.Log.error("Not player turn!")

        }
        return true
    }

    return false
}
