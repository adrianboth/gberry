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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0

import "TriviaGameModel.js" as GameModel
import "PointsModel.js" as PointsModel
import "QuestionsModel.js" as QuestionsModel
import "QuestionsLoader.js" as QuestionsLoader

Window {
    id: root
    visible: true
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    property bool debugSkipCountDown: false

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay; debug: false }
    Theme { id: theme }

    // testing
    //GBerry.GButton { label: "Test2"; anchors.centerIn: parent; z: 1000 }

    // TODO: column layout

    /*
    InfoBar {
        id: infobar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        z: 100

        gradient: Gradient {
            GradientStop { position: 0.0; color: "snow" }
            GradientStop { position: 1.0; color: "gray" }
        }
    }
    */

    Rectangle {
        id: mainarea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        Rectangle {
            id: menuview
            anchors.fill: parent

            border.color: "slategray"
            color: "lightsteelblue"
            /* Gradient doesn't work well on Raspberry Pi
            gradient: Gradient {
                GradientStop { position: 0.0; color: "lightsteelblue" }
                GradientStop { position: 1.0; color: "slategray" }
            }
            */

            Item {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: mainmenu.top
                //color: "red"
                //opacity: 0

                Text {
                    id: triviaTextLabel
                    text: "Trivia"
                    font.pixelSize: Math.min(55, gdisplay.text_mm(25))
                    anchors.centerIn: parent
                }

                Image {
                    anchors.right: triviaTextLabel.left
                    anchors.margins: gdisplay.touchCellWidth()
                    anchors.verticalCenter: triviaTextLabel.verticalCenter
                    source: "images/question_mark.svg"
                    width: 100; height: 100
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }
            }

            MainMenu {
                id: mainmenu
                anchors.centerIn: parent

                items: [
                    MainMenuItem {
                        text: qsTr("Play Game")
                        onSelected: playGameSelected()
                    },
                    MainMenuItem {
                        text: qsTr("Game Mode")
                        onSelected: gameModeSelected()
                    },
                    MainMenuItem {
                        text: qsTr("Exit")
                        onSelected: exitGameSelected()
                    }
                ]

                // TODO:could this be embedded to MainMenu?
                // commands from mobile clients
                function processControlAction(action) {
                    if (action === "Up")
                        mainmenu.moveFocusToPrevious()
                    else if (action === "Down")
                        mainmenu.moveFocusToNext()
                    else if (action === "OK")
                        mainmenu.selectCurrent()
                }
            }

            // TODO: just skeleton
            Text {
                id: currentgamemode
                text: qsTr("Game mode: 4 boxes")
                font.pixelSize: gdisplay.smallSize * gdisplay.ppmText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: mainmenu.bottom
            }

            Text {
                id: playersJoinedLabel
                text: playersManager.numberOfPlayers.toString() + qsTr(" players connected to play")
                font.pixelSize: gdisplay.mediumSizeText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: currentgamemode.bottom
                anchors.topMargin: gdisplay.touchCellHeight()
            }

            GameModeSelectionView {
                id: gamemodeview
                visible: false
                anchors.centerIn: parent

                function processControlAction(action) {
                    /*
                    if (action === "Up")
                        // TODO

                    else if (action === "Down")
                        // TODO
                    else
                    */
                    if (action === "OK") {
                        // close popup
                        mainarea.state = "MENU"
                    }
                }
            }
        }

        // --- alternative view

        GameView {
            id: gameView
            anchors.fill: parent

            onNewGameRequested: initializeNewGame()
            onMenuRequested: mainarea.state = "MENU"

            // game time (seconds) on top
            /*
            GameTime {
                id: gametimelabel
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            */
            /*
            CountDown {
                id: startcountdown
                visible: false
                anchors.centerIn: parent

                onFinished: {
                    ReactGameModel.start()
                    gametimelabel.start()
                    gameView.start()
                }
            }
            */
        }

        state: "MENU"
        // TODO: temporary for testing
        //state: "GAME"

        states: [
                State {
                    name: "MENU"
                    PropertyChanges { target: gameView; visible: false }
                    PropertyChanges { target: menuview; visible: true }
                    PropertyChanges { target: gamemodeview; visible: false }
                    StateChangeScript {
                            name: "myScript1"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO MENU")
                                var js = {action: "ShowBasicControls",
                                          enable: ["Up", "Down", "OK"]}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))
                            }
                        }
                },
                State {
                    name: "GAMEMODESELECTION"
                    // show on top of menu
                    PropertyChanges { target: gamemodeview; visible: true }
                    PropertyChanges { target: gameView; visible: false }
                    PropertyChanges { target: menuview; visible: true }
                },

                State {
                    name: "GAME"
                    PropertyChanges { target: gameView; visible: true}
                    PropertyChanges { target: menuview; visible: false}

                    StateChangeScript {
                            name: "myScript"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO GAME")
                            }
                        }
                },
                State {
                    // game ended state
                    name: "END"
                    // keep still game area visible
                }
            ]
    }

    ExitConfirmationDialog {
        id: exitConfirmationDialog
    }

    // -------------------------------------------------------------------------

    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "GameMenu", actionName: "Abort Game"}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (mainarea.state === "MENU") {
            // TODO: we should ask from view what basic controls should be enabled
            js = {action: "ShowBasicControls",
                  enable: ["Up", "Down", "OK"]}
            playersManager.sendPlayerMessage(pid, JSON.stringify(js))
        } else if (mainarea.state === "GAME") {
            js = {action: "DefineAppBox",
                      data: AppBoxMaster.dataStr()}

            playersManager.sendAllPlayersMessage(JSON.stringify(js))

            js = {action: "ShowAppBox"}
            playersManager.sendAllPlayersMessage(JSON.stringify(js))
        }

        // TODO: add to points model
    }

    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)

        if (playersManager.numberOfPlayers === 0) {
            // last player left

            // if in game -> return to menu
            if (mainarea.state === "GAME") {
                mainarea.state = "MENU"
            }
        }

        // TODO: remove from PointsModel
    }

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        var js  = JSON.parse(data)
        if (js["action"] === "SelectBasicControlAction")
        {
            if (mainarea.state === "MENU") {
                mainmenu.processControlAction(js["id"])
            } else if (mainarea.state === "GAMEMODESELECTION") {
                gamemodeview.processControlAction(js["id"])
            }

        } else if (js["action"] === "ConfirmationQuestionResponse") {
            // TODO: could it be possible to have more object oriented questions
            //       (i.e. if visible it has been registered as listener)

            if (js["questionId"] === exitConfirmationDialog.questionId) {
                exitConfirmationDialog.processResponse(js["ref"])

            } else if (js["questionId"] === "playanother") {
                playedwonDialog.selectOption(js["ref"])
            }

        } else if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION")
                mainarea.state = "MENU"
            }
        } else if (js["action"] === "AppBoxMessage") {
            console.debug("### message received: " + js["data"])
            var responseJs = {"action": "AppBoxMessage",
                              "data": {"action": "Ping", "msg": "ping " + playersManager.playerName(pid) + "!"}}

            playersManager.sendPlayerMessage(pid, JSON.stringify(responseJs))

            responseJs = {"action": "AppBoxMessage",
                          "data": {"action": "Ping", "msg": "ping all!"}}

            playersManager.sendAllPlayersMessage(JSON.stringify(responseJs))

            //ReactGameModel.playerMessageReceived(pid, js["data"])
        }
    }

    function playGameSelected() {
        console.debug("Play selected")
        // TODO: appbox version handshaking at some point

        var js = {action: "DefineAppBox",
                  data: AppBoxMaster.dataStr()}

        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        js = {action: "ShowAppBox"}
        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        // TODO: waiting response from player (every one ready)

        // TODO: create timer to enable buttons
        // TODO: wait button presses -> !inform whose came first
        // TODO:   - ?? how to move to next

        mainarea.state = "GAME"

        PointsModel.setupGame()
        QuestionsModel.setupGame(10)
        GameModel.setupGame()
        gameView.startGame()

    }

    function gameModeSelected() {
        Log.debug("Game mode selected")
        // TODO: show game mode list to select
        //   a) number lenght
        //   b) random buttons

        mainarea.state = "GAMEMODESELECTION"
    }

    function exitGameSelected() {
        console.debug("Exit selected")
        exitConfirmationDialog.show()
    }

    function initializeNewGame() {
        playGameSelected()
    }

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        PointsModel.initialize()

        QuestionsLoader.loadQuestions()
        //console.debug("### READ QUESTIONS: " + QuestionsLoader.data())
        QuestionsModel.initialize(QuestionsLoader.dataJson()["questions"])

        GameModel.initialize(PointsModel, QuestionsModel)

        gameView.initialize(GameModel, PointsModel, QuestionsModel)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        // TODO: for dev
        playGameSelected()
    }

    AppBoxDebugWindow {

    }
}
