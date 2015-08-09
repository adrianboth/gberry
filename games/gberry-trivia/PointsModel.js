.pragma library

var _playerPoints

var signals = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal scoresChanged(); }', Qt.application, 'PointsModelSignals');


function initialize() {

}


function setupGame() {
    _playerPoints = {}
    // TODO: we need to get ref to playersManager
    //playersManager.playerIds()

    // TODO: demo setup
    _playerPoints = [{"name": "tero", "points": 5},
                     {"name": "joel", "points": 5}]

}

function currentScores() {
    var sortedPlayerPoinst = _playerPoints
    sortedPlayerPoinst.sort(function (a, b) { return b["points"] - a["points"]})

    return sortedPlayerPoinst
}

function scorePoint(playerPid) {
    if (playerPid === 0) {
        // this is dev player, score always last
        var playerData = _playerPoints[_playerPoints.length - 1]
        playerData["points"] += 1

        signals.scoresChanged()
    }

    // TODO: real impl
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

// TODO score point

// TODO get ui model

