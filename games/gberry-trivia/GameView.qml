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

import QtQuick 2.4
import QtQuick.Layouts 1.1

import GBerry 1.0

Rectangle {
    id: self

    property var gameModel: Object
    property var questionsModel: Object
    property var pointsModel: Object

    signal newGameRequested()
    signal menuRequested()

    function initialize(gameModel, pointsModel, questionsModel) {
        self.gameModel = gameModel
        self.questionsModel = questionsModel
        self.pointsModel = pointsModel


        self.pointsModel.signals.scoresChanged.connect(onScoresChanged)
        self.questionsModel.signals.gameEnded.connect(onGameEnded)
        self.questionsModel.signals.moveToNextQuestion.connect(onMoveToNextQuestion)

    }

    function startGame() {
        self.visible = true
        questionsFrame.showWait()
        currentQuestion.readQuestion()
        onScoresChanged()
        questionsFrame.start()
    }

    // TODO: how to define in common place gradient for all views
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    RowLayout {
        anchors.fill: parent

        PlayersFrame {
            id: playersFrame
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.33

        }

        // this way to pass information into sub ui components, they see
        // 'currentQuestion' in their scope
        Item {
            id: currentQuestion
            property string text: "a"
            property string answer1: "aa"
            property string answer2: "bb"
            property string answer3: "cc"
            property string answer4: "dd"
            property int currentQuestionIndex: 0
            property int maxQuestionsCount: 0

            property int showAnswer1: 0
            property int showAnswer2: 0
            property int showAnswer3: 0
            property int showAnswer4: 0

            property string correntAnswerId: ""

            function readQuestion() {
                console.debug("Reading question")
                var q = self.questionsModel.currentQuestion()
                text = q["question"]

                function get(value) {
                    if (typeof(value) !== "undefined") {
                        return value
                    } else {
                        return ""
                    }
                }
                answer1 = get(q["a"])
                answer2 = get(q["b"])
                answer3 = get(q["c"])
                answer4 = get(q["d"])
                correntAnswerId = get(q["answer"])

                showAnswer1 = 0
                showAnswer2 = 0
                showAnswer3 = 0
                showAnswer4 = 0

                currentQuestionIndex = self.questionsModel.properties.currentQuestionIndex
                maxQuestionsCount = self.questionsModel.properties.maxQuestionsCount
            }

            function showAnswer() {
                showAnswer1 = correntAnswerId === "a" ? 1 : -1
                showAnswer2 = correntAnswerId === "b" ? 1 : -1
                showAnswer3 = correntAnswerId === "c" ? 1 : -1
                showAnswer4 = correntAnswerId === "d" ? 1 : -1

            }
        }

        QuestionsFrame {
            id: questionsFrame
            Layout.fillWidth: true
            Layout.fillHeight: true

            onDevAnswerClicked: self.gameModel.devAnswerClicked(answerId)

            SequentialAnimation {
                id: changeQuestionAnimation
                running: false
                NumberAnimation { target: questionsFrame; property: "y"; to: root.height; duration: 500 }
                //PropertyAction { target: questionsFrame; property: "y"; value: 0 }
                ScriptAction { script: self.onOldQuestionHiddenAnimationFinished() }
            }

            SequentialAnimation {
                id: newQuestionAnimation
                running: false
                PropertyAction { target: questionsFrame; property: "y"; value: -root.height }
                NumberAnimation { target: questionsFrame; property: "y"; to: 0; duration: 500 }
                ScriptAction { script: self.onNewQuestionAnimationFinished() }
            }

            onReadyToAcceptAnswers: {
                self.onReadyToAcceptAnswers()
            }
        }

    }

    PlayerWonDialog {
        id: playerWonDialog

        onNewGameRequested: self.newGameRequested()
        onMenuRequested: self.menuRequested()
    }

    Timer {
        id: changeQuestionsTimer
        running: false; repeat: false
        interval: 2000
        onTriggered: {
            onChangeQuestionsTimerFinished()
        }
    }

    Timer {
        id: gameEndedTimer
        running: false; repeat: false
        interval: 2000
        onTriggered: {
            onGameEndedTimerFinished()
        }
    }

    // -------------------------------------------------------------------------

    // ANIMATIONS
    //  - first correct answer is show + player scoring emphasized
    //  - then hide old question
    //  - finally show new question (ready to accept answers)

    function onMoveToNextQuestion() {
        currentQuestion.showAnswer()
        changeQuestionsTimer.running = true
        //currentQuestion.readQuestion()
    }

    function onChangeQuestionsTimerFinished() {
        changeQuestionAnimation.running = true
    }

    function onOldQuestionHiddenAnimationFinished() {
        currentQuestion.readQuestion()
        newQuestionAnimation.running = true
    }

    function onNewQuestionAnimationFinished() {
        // ok to start accepting answers
    }

    function onReadyToAcceptAnswers() {
        console.debug("### ready to accept answers")
    }

    function onScoresChanged() {
        playersFrame.updateScores(pointsModel.currentScores())
    }

    function onGameEnded() {
        currentQuestion.showAnswer()
        gameEndedTimer.running = true
    }

    function onGameEndedTimerFinished() {
        // all questions answered
        var highestScores = self.pointsModel.highestScores()
        if (highestScores.length > 1) {
            // draw between players
            playerWonDialog.declareDraw(highestScores, currentQuestion.maxQuestionsCount)

        } else {
            // single player won
            playerWonDialog.declarePlayerWon(highestScores[0], currentQuestion.maxQuestionsCount, 0) // TODO: time
        }

        // TODO: for players appbox ui state change

        //gametimelabel.stop()
        //appboxui.stop()
        //HighScore.recordWinningResult(playersManager.playerName(pid), gametimelabel.finalTime())

    }
}

