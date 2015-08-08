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
 
.import "ApplicationSettingsJS.js" as ApplicationSettingsJS
.import GBerry 1.0 as GBerry
.import GBerryConsole 1.0 as ConsoleLib

GBerry.Log.initLog("GameModel", ApplicationSettingsJS.logLevel)
//GBerry.Log.debug("Model initialized")

// logic of game


// call back for winning
//var callbacks = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal playerWon(var pid); signal playerCorrectNumber(var pid); signal playerInvalidNumber(var pid); }', Qt.application, 'ModelCallbacks');

var _pointsModel
var _questionsModel
var _currentQuestion

function initialize(pointsModel, questionsModel) {
    _pointsModel = pointsModel
    _questionsModel = questionsModel
}

function setupGame() {
    // TODO: demo question
    _currentQuestion = {"question": "Foo question", "a": "aa", "b": b}
}

function currentQuestion() {

}

// TODO: win conditioon
