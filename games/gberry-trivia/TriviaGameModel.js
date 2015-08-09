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
//var signals = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal moveToNextQuestion(); }', Qt.application, 'GameModelSignals');

var _pointsModel
var _questionsModel

function initialize(pointsModel, questionsModel) {
    _pointsModel = pointsModel
    _questionsModel = questionsModel
}

function setupGame() {
    // TODO: demo question
}

function devAnswerClicked(answerId) {
    var q = _questionsModel.currentQuestion()

    if (typeof(q[answerId]) !== "undefined" ) {
        // yes, we have such answer but is it correct
        if (q["answer"] === answerId) {
            // correct
            console.debug("Dev player answered correctly")
            _pointsModel.scorePoint(0)
            // TODO: now depends from game mode
            //    - move to next or wait other players
            //    - feedback also depends from that
            //    - NOW just proceeding to next question

            _questionsModel.moveToNextQuestion()

        } else {
            // not correct

            // TODO: normally feedback for player (appbox)
            console.debug("Dev player selected wrong answer")
        }
    } else {
        console.warn("Invalid dev answer: " + answerId)
    }
}

// TODO: win conditioon
