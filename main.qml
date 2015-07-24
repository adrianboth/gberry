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

import "ReactGameModel.js" as ReactGameModel
import "ProgressFeedback.js" as ProgressFeedback
import "HighScore.js" as HighScore

Window {
    id: root
    visible: true
    width: 800
    height: 600

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    // testing
    //GBerry.GButton { label: "Test2"; anchors.centerIn: parent; z: 1000 }

    // TODO: column layout
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

    Rectangle {
        id: mainarea
        anchors.top: infobar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        Rectangle {
            id: menuview // TODO: menuview
            anchors.fill: parent

            border.color: "slategray"
            gradient: Gradient {
                GradientStop { position: 0.0; color: "lightsteelblue" }
                GradientStop { position: 1.0; color: "slategray" }
            }

            Item {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: mainmenu.top
                //color: "red"
                //opacity: 0

                Text {
                    text: "React!"
                    font.pixelSize: gdisplay.text_mm(25)
                    anchors.centerIn: parent
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
                //anchors.topMargin: 50 // TODO: somehow changing size
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

            // TODO: width should adapt to available size
            Text {
                id: todaysbestlabel
                text: "undefined"
                font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText
                z: 100
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                function update() {
                    if (HighScore.hasTodaysFastest()) {
                        Log.debug("HighScore exists!")
                        todaysbestlabel.visible = true
                        var hs = HighScore.todaysFastest()
                        todaysbestlabel.text = qsTr("Todays best:") + " " + hs.time + " seconds by " + hs.name
                    } else {
                        Log.debug("No HighScore yet!")
                        todaysbestlabel.visible = false
                    }
                }
            }


        }

        // --- alternative view

        GameView {
            id: appboxui
            anchors.fill: parent

            onVisibleChanged: {
                if (appboxui.visible) {
                    // TODO: now just fixed, can we even get coords before gameview visible
                    var topY = appboxui.yForBoxLine()
                    var initialX = appboxui.xForLeftMostBox()
                    var stepX = appboxui.stepSize()
                    Log.debug("appboxui visible: topY=" + topY + ", initialX=" + initialX + ", stepX=" + stepX)
                    ProgressFeedback.init(ReactGameModel, topY, initialX, stepX)
                } else {
                    ProgressFeedback.teardown()
                }
            }

            // game time (seconds) on top
            GameTime {
                id: gametimelabel
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }

            CountDown {
                id: startcountdown
                visible: false
                anchors.centerIn: parent

                onFinished: {
                    ReactGameModel.start()
                    gametimelabel.start()
                    appboxui.start()
                }
            }
        }

        state: "MENU"
        // TODO: temporary
        //state: "GAME"

        states: [
                State {
                    name: "MENU"
                    PropertyChanges { target: appboxui; visible: false }
                    PropertyChanges { target: menuview; visible: true }
                    PropertyChanges { target: gamemodeview; visible: false }
                    StateChangeScript {
                            name: "myScript1"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO MENU")
                                var js = {action: "ShowBasicControls",
                                          enable: ["Up", "Down", "OK"]}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))

                                // TODO: could this be embedded to setup() but what is UI effect
                                ProgressFeedback.teardown()
                                todaysbestlabel.update()
                            }
                        }
                },
                State {
                    name: "GAMEMODESELECTION"
                    // show on top of menu
                    PropertyChanges { target: gamemodeview; visible: true }
                    PropertyChanges { target: appboxui; visible: false }
                    PropertyChanges { target: menuview; visible: true }
                },

                State {
                    name: "GAME"
                    PropertyChanges { target: appboxui; visible: true}
                    PropertyChanges { target: menuview; visible: false}

                    StateChangeScript {
                            name: "myScript"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO GAME")
                                //var js = {action: "ShowAppBox"}
                                //playersManager.sendAllPlayersMessage(JSON.stringify(js))
                                ReactGameModel.initializePlayers()
                                ProgressFeedback.setup()
                                appboxui.setNumbers(ReactGameModel.numbers())
                                gametimelabel.reset()
                                startcountdown.start()
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


    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "GameMenu", actionName: "Menu"}]}
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
    }

    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "Player left")

        if (playersManager.numberOfPlayers === 0) {
            // last player left

            // if in game -> return to menu
            if (mainarea.state === "GAME") {
                messageBoard.insertMessage("Last player left -> back to menu")
                mainarea.state = "MENU"
            }
        }
    }

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)

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

            if (js["questionId"] === "exitgame") {
                if (exitConfirmationDialog.visible) {
                    exitConfirmationDialog.selectOption(js["ref"])

                    // just to make sure everyones dialogs are closed
                    playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
                }
            } else if (js["questionId"] === "playanother") {
                playedwonDialog.selectOption(js["ref"])
            }

        } else if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION")
                mainarea.state = "MENU"
            }
        } else if (js["action"] === "AppBoxMessage") {
            ReactGameModel.playerMessageReceived(pid, js["data"])
        }
    }

    MessageBoard {
        id: messageBoard
        opacity: 0.5
        visible: gsettings.developmentMode

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    GConfirmationDialog {
        id: exitConfirmationDialog
        visible: false // initial state
        //text: "Are you sure to exit this game?"
        questionText: qsTr("Are you sure to exit this game?")
        option1Text: qsTr("Yes")
        option2Text: qsTr("No")

        onOption1Selected: {
            // Yes
            Qt.quit()
        }

        onOption2Selected: {
            exitConfirmationDialog.visible = false
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

        // as demo send confirmation to clients

        // TODO: how localization of these texts would go?

        var js = {action: "ConfirmationQuestion",
                  questionId: "exitgame",
                  title: exitConfirmationDialog.titleText,
                  text: exitConfirmationDialog.questionText,
                  options: [{id:   exitConfirmationDialog.option1Id,
                             text: exitConfirmationDialog.option1Text},
                            {id:   exitConfirmationDialog.option2Id,
                             text: exitConfirmationDialog.option2Text}]
                 }

        // TODO: should be send only for controlling player but that is not yet implemented)
        //    * Now for first player

        // TODO: gets really strange player id list (not list at all??)
        //playersManager.sendPlayerMessage(playersManager.playerIds(), JSON.stringify(js))
        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        // show also on big screen the question
        exitConfirmationDialog.visible = true

        // TODO: disable everything else -> record state
    }


    GConfirmationDialog {
        id: playedwonDialog
        visible: false
        questionText: "xxx"
        option1Id: "Yes"
        option1Text: qsTr("Yes")
        option2Id: "No"
        option2Text: qsTr("No")

        // pid:  player ID
        // time: how much time it was used to complete game
        function declarePlayerWon(pid, time) {
            var name = playersManager.playerName(pid)
            // TODO: localization of this is screwed!!!FIX
            playedwonDialog.questionText = qsTr("Player " + name + " won!\nCompleted in +" + time.toString() + " seconds.\n\nPlay another?")
            playedwonDialog.visible = true

            // note that we are not showing exactly same info in question that
            // on client side
            var clientQuestionText = qsTr("Play another?")
            // TODO: GConfirmationDialog could have a helper ...
            var js = {action: "ConfirmationQuestion",
                      questionId: "playanother", // TODO: client needs to return this
                      title: playedwonDialog.titleText,
                      text: clientQuestionText,
                      options: [{id:   playedwonDialog.option1Id,
                                 text: playedwonDialog.option1Text},
                                {id:   playedwonDialog.option2Id,
                                 text: playedwonDialog.option2Text}]
                     }

            playersManager.sendAllPlayersMessage(JSON.stringify(js))
        }

        onOption1Selected: {
            // yes
            playedwonDialog.visible = false
            mainarea.state = "GAME"
        }

        onOption2Selected: {
            // no
            mainarea.state = "MENU"
            playedwonDialog.visible = false
        }
    }

    function onPlayerWon(pid) {
        mainarea.state = "END"
        gametimelabel.stop()
        appboxui.stop()
        HighScore.recordWinningResult(playersManager.playerName(pid), gametimelabel.finalTime())
        playedwonDialog.declarePlayerWon(pid, gametimelabel.finalTime())
    }
    function onPlayerCorrectNumber(pid) {
        messageBoard.insertPlayerMessage(pid, "Correct number!")
        ProgressFeedback.updatePlayer(pid)
        var msg = {action: "CorrectNumberFeedback"}
        playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(msg))
    }
    function onPlayerInvalidNumber(pid) {
        messageBoard.insertPlayerMessage(pid, "Invalid number!")
        ProgressFeedback.invalidInput(pid)
        var msg = {action: "InvalidNumberFeedback"}
        playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(msg))
    }

    // TODO: sound when game starts
    // TODO: sound when timer ticks
    // TODO: sound when game ends

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        ReactGameModel.callbacks.playerWon.connect(onPlayerWon)
        ReactGameModel.callbacks.playerCorrectNumber.connect(onPlayerCorrectNumber)
        ReactGameModel.callbacks.playerInvalidNumber.connect(onPlayerInvalidNumber)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        todaysbestlabel.update()

        // testing dialog
        //playedwonDialog.declarePlayerWon(0, 0)
    }
}

// TODO: credits main page
// TODO: why timer text not center

// TODO: UI candy
//   - timer number size changing, going smaller
//   - at least zero fading
//
// some indicator who has type what

// TODO: confirmation dialog doesn't work; why ? I thought ready; check mainui

// TODO: get fancy background image (1920x1080)

// TODO: record running time: stop clock when winner

// copyright: Developed by GBerry Project
