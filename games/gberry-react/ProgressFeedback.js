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
 
 var _color = ["red", "blue", "green", "yellow", "purple", "pink", "gray"] // TODO: not really unlimited number
var _pidToIndexMap = {}
var _boxes = []
var _gameModel;
var _topBottomY;
var _initialX;
var _stepX;

function init(gameModel, topBottomY, initialX, stepX) {
    _gameModel = gameModel
    _topBottomY = topBottomY
    _initialX = initialX
    _stepX = stepX
}

function setup() {
    // if game has been in end state -> delete previous setup
    if (!_gameModel.gameRunning())
        teardown()

    // player list defines indexes for players
    var playerList = _gameModel.playerList()

    // used to calculate Y position for nameboxes
    var currentY = _topBottomY

    for (var i = 0; i < playerList.length; i++) {

        var pid = playerList[i]
        _pidToIndexMap[pid] = i

        var playerName = playersManager.playerName(pid)
        var color = _color[i] // index defines color

        // create box
        var component = Qt.createComponent("NameBox.qml")
        var box = component.createObject(mainarea, {name: playerName, bgColor: color})

        if (box == null) {
            // Error Handling
            console.log("Error creating object");
            continue
        }

        _boxes.push(box)

        // TODO: can we get right away the size?
        var margin = box.height / 2
        var y = currentY - box.height - margin
        var x = _initialX

        box.y = y
        box.x = x

        currentY = y // starting point for next namebox
    }
}

function teardown() {
    for (var i = 0; i < _boxes.length; i++) {
        _boxes[i].destroy()
    }

    _boxes = []
    _pidToIndexMap = {}
}

function updatePlayer(pid) {
    // TODO: actually in first version this func is called only when moving to next
    //var pos = gameModel.playerPosition(pid)
    var box = _boxes[_pidToIndexMap[pid]]

    // TODO: animation of moving box

    // in first version y is always kept same
    box.x = box.x + _stepX
}

function invalidInput(pid) {
    var box = _boxes[_pidToIndexMap[pid]]
    box.shake()
}

