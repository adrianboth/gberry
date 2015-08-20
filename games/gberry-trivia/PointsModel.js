.pragma library

var _playerPoints = []
var _playersManager

var signals = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal scoresChanged(); }', Qt.application, 'PointsModelSignals');


function initialize(playersManager) {
    _playersManager = playersManager
}


function setupGame() {
    // TODO: we need to get ref to playersManager
    console.debug("START GAME: pids = " + _playersManager.playerIds().toString())
    _playerPoints = []

    // current players, more can join later
    _playersManager.playerIds().forEach(function (pid) {
        _playerPoints.push({id: pid,
                            name: _playersManager.playerName(pid),
                            points: 0
                           })
    })

    if (_playerPoints.length === 0) {
        // must be demo play
        _playerPoints = [{"id": 5000, "name": "Tero", "points": 5},
                         {"id": 5001, "name": "Joel", "points": 5}]
    }

    console.debug("START GAME: players: " + _playerPoints.toString())
    signals.scoresChanged()
}

function currentScores() {
    var sortedPlayerPoinst = _playerPoints
    // highest points first
    sortedPlayerPoinst.sort(function (a, b) { return b["points"] - a["points"]})

    return sortedPlayerPoinst
}

function scorePoint(playerPid) {
    if (playerPid === 0) {
        // this is dev player, score always last
        var playerData = _playerPoints[_playerPoints.length - 1]
        playerData.points++

        signals.scoresChanged()
    }
    else {
        for (var i = 0; i < _playerPoints.length; i++) {
            if (_playerPoints[i].id === playerPid) {
                _playerPoints[i].points++
                signals.scoresChanged()
                break
            }
        }
    }
}

// returns list: they can be multiple if there is draw condition
// returns list of names
function highestScores() {
    var highest = []

    for (var i = 0; i < _playerPoints.length; i++) {
        if (highest.length === 0) {
            highest.push(_playerPoints[i])
        } else if (_playerPoints[i]["points"] === highest[0]["points"]) {
            highest.push(_playerPoints[i])
        } else if (_playerPoints[i]["points"] > highest[0]["points"]) {
            highest = [playerPoints[i]]
        }
    }

    return highest
}

function currentScore(pid) {
    for (var i = 0; i < _playerPoints.length; i++) {
        if (_playerPoints[i].id === pid) {
            return _playerPoints[i].points
        }
    }

    console.warn("PointModel.currentScore(): Didn't find any player by pid: " + pid.toString())
    return -1
}

function playerJoined(pid, playerName) {
    _playerPoints.push({"id": pid, "name": playerName, "points": 0})
    signals.scoresChanged()
}

function playerLeft(pid, playerName) {
    for (var i = 0; i < _playerPoints.length; i++) {
        if (_playerPoints[i].id === pid) {
            _playerPoints.splice(i, 1)
            signals.scoresChanged()
            return
        }
    }
}
