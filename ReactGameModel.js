.import "ApplicationSettingsJS.js" as ApplicationSettingsJS
.import GBerry 1.0 as GBerry
.import GBerryConsole 1.0 as ConsoleLib

GBerry.Log.initLog("ReactGameModel", ApplicationSettingsJS.logLevel)

// logic of react game

// ?? how game actually starts
//    - first timer

//var _boxes = []
var _numbers = []
var _numberLength = 4
var _gameState = "UNINITIALIZED"

// call back for winning
var callbacks = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal playerWon(var pid); signal playerCorrectNumber(var pid); signal playerInvalidNumber(var pid); }', Qt.application, 'ModelCallbacks');

/*
function init(box1, box2, box3, box4) {
    _boxes = [box1, box2, box3, box4]
    _gameState = "INITIALIZED"
    GBerry.Log.debug("Model initialized")
}
*/

var _messageBoard;
function debugInit(messageBoard) {
    _messageBoard = messageBoard
}

function gameRunning() {
    return _gameState === "STARTED"
}

var _playersIDs = []
var _playerIndexes = {}
var _playersNumberQueue = []

function initializePlayers() {
    // current players, no more accepted during game play
    _playersIDs = playersManager.playerIds()

    // empty data structures for players
    _playersNumberQueue = []
    var appboxMsg = {action: "DisableControls"}
    var disableControlsMsg = ConsoleLib.Messages.createCustomAppBoxMsg(appboxMsg)
    var showAppBoxMsg = ConsoleLib.Messages.createShowAppBoxMsg()

    for (var p = 0; p < _playersIDs.length; p++) {
        _playersNumberQueue.push([])
        _playerIndexes[_playersIDs[p]] = p

        // send players game setup. 'playersManager' is in global context
        // make sure controls are disabled until we show (after timer they are enabled)
        playersManager.sendPlayerMessage(_playersIDs[p], disableControlsMsg)
        playersManager.sendPlayerMessage(_playersIDs[p], showAppBoxMsg)
    }

    // populate number boxes
    _numbers = []
    for (var i = 0; i < _numberLength; i++) {
        _numbers.push(_randomNumber())
        //_boxes[i].number = _numbers[i]
    }

    // (view shows timer, count down then calls start())
    _gameState = "COUNTDOWN"
}

function playerList() {
    return _playersIDs
}

function numbers() {
    return _numbers
}

// get random number between 0-9
function _randomNumber() {
    return Math.round(Math.random()*9)
}

function start() {
    _gameState = "STARTED"
    // send activation message to all players
    var msg = {action: "EnableControls"}
    for (var i = 0; i < _playersIDs.length; i++) {
        _sendMessageToPlayer(_playersIDs[i], msg)
    }
}

// returns what number box needs to be filled next
function playerPosition(pid) {
    var playerIndex = _playerIndexes[pid]
    var playerNumberQueue = _playersNumberQueue[playerIndex]
    return playerNumberQueue.length
}

function _sendMessageToPlayer(pid, msgJson) {
    var customMsg = ConsoleLib.Messages.createCustomAppBoxMsg(msgJson)
    playersManager.sendPlayerMessage(pid, customMsg)
}

// appbox custom message
function playerMessageReceived(pid, js) {
    GBerry.Log.debug("Player message: id = " + pid)

    if (_gameState != "STARTED") {
        GBerry.Log.debug("Discarded message as game is not running")
    }

    if (js.hasOwnProperty('number')) {
        // TODO: how to catch parsing errors?
        var n = parseInt(js["number"])

        GBerry.Log.debug("Player " + pid.toString() + " pressed " + n.toString())
        var playerIndex = _playerIndexes[pid]
        GBerry.Log.debug("Player " + pid.toString() + ": playerIndex=" + playerIndex)
        var playerNumberQueue = _playersNumberQueue[playerIndex]
        var nextIndexOfPlayerNumber = playerNumberQueue.length
        GBerry.Log.debug("Player " + pid.toString() + ": nextIndexOfPlayerNumber=" + nextIndexOfPlayerNumber)
        var referenceNumber = _numbers[nextIndexOfPlayerNumber]
        GBerry.Log.debug("_numbers=" + _numbers.toString())
        GBerry.Log.debug("Player " + pid.toString() + ": " + n.toString() + " vs " + referenceNumber.toString())

        if (n === referenceNumber) {
            // correct number pushed
            playerNumberQueue.push(n)

            // feedback
            var correctMsg = {action: "CorrectNumberFeedback"}
            _sendMessageToPlayer(pid, correctMsg)
            callbacks.playerCorrectNumber(pid)

            // victory condition?
            if (playerNumberQueue.length === _numbers.length) {
                callbacks.playerWon(pid)
                _gameState = "ENDED"
            }

        } else {
            // incorrect numbers
            // feed back
            var invalidMsg = {action: "InvalidNumberFeedback"}
            _sendMessageToPlayer(pid, invalidMsg)
            callbacks.playerInvalidNumber(pid)

        }
    }
}

// TODO: get messages from players
// TODO: some kind of mechanism to show who where
