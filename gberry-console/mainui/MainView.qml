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
import QtQuick.Controls 1.2

import GBerry 1.0
import GBerryConsole 1.0

Item {
    id: self

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
        anchors.fill: parent
        border.color: "slategray"
        color: "lightsteelblue"
        /*
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }*/
    }

    StackView {
        id: mainarea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        initialItem: mainMenuView

        onCurrentItemChanged: sendControlActions()

        function sendControlActions() {
            if (currentItem !== null) {
                var js = {action: "ShowBasicControls",
                          enable: currentItem.enabledControlActions}
                playersManager.sendAllPlayersMessage(JSON.stringify(js))
            }
        }

        MainMenuView {
            id: mainMenuView
            enabled: visible

            // switching between connection info and menu
            onEnabledControlActionsChanged: {
                mainarea.sendControlActions()
            }

            menuItems: [
                MainMenuItem {
                    text: qsTr("Games on console")
                    imageUrl: "images/video-game5.svg"
                    onSelected: {
                        mainarea.push({item: gamesOnConsoleView, immediate: true})
                        //onGamesOnConsoleSelected()
                    }
                },
                MainMenuItem {
                    text: qsTr("Games on webstore")
                    imageUrl: "images/online-store.svg"

                    onSelected: {
                        console.debug("pid: " + pid)
                        if (Connection.isHeadServerConnected) {
                            // can be -1 if no specific user is allocated
                            ActivePlayerModel.activatePlayer(pid)
                            mainarea.push({item: downloadableGamesView, immediate: true})
                        } else {
                            noWebstoreConnectionDialog.visible = true

                            var js = {action: "ConfirmationQuestion",
                                      questionId: "noWebstoreConnectionDialog",
                                      title: qsTr("Error"),
                                      text: qsTr("No connection"),
                                      options: [{id:   "ok",
                                                 text: qsTr("OK")} ]
                                     }

                            playersManager.sendAllPlayersMessage(JSON.stringify(js))
                        }

                    }
                },
                MainMenuItem {
                    text: qsTr("Settings")
                    imageUrl: "images/settings48.svg"
                    //onSelected: notImplementedDialog.visible = true
                    onSelected: {
                        // TODO: better to have connection info outside
                        mainMenuView.showConnectionInfo()
                    }
                },
                MainMenuItem {
                    text: qsTr("Exit")
                    imageUrl: "images/direction133.svg"
                    onSelected: exitGameSelected()
                }
            ]
        }

        GamesOnConsoleView {
            id: gamesOnConsoleView
            visible: false

            onVisibleChanged: {
                if (visible)
                    forceActiveFocus()
            }

            onBackSelected: {
                mainarea.pop({immediate: true})
            }

            onLaunchRequested: {
                console.debug("LAUNCH: " + gameId)
                ApplicationManager.launchApplication(gameId)
            }

            onEnabledControlActionsChanged: mainarea.sendControlActions()
        }

        DownloadableGamesView {
            id: downloadableGamesView
            visible: false

            onVisibleChanged: {
                if (visible)
                    forceActiveFocus()
            }

            onBackSelected: {
                ActivePlayerModel.deactivatePlayer()
                mainarea.pop({immediate: true})
            }

            onDownloadRequested: {
                console.debug("DOWNLOAD: " + gameId)
                downloadFeedbackBar.startDownload(gameId)
            }

            onLaunchRequested: {
                console.debug("LAUNCH: " + gameId)
                ApplicationManager.launchApplication(gameId)
            }

            // TODO: now first just for this view, but could be somehow generic
            onErrorOccurred: {
                feedbackDialog.show(errorMsg)
            }

            onEnabledControlActionsChanged: mainarea.sendControlActions()
        }

        state: "MENU"
        states: [
                State {
                    name: "MENU"
                    //PropertyChanges { target: appboxui; visible: false }
                    PropertyChanges { target: mainMenuView; visible: true }
                    StateChangeScript {
                            name: "myScript1"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO MENU")
                                var js = {action: "ShowBasicControls"}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))
                            }
                        }
                },
                State {
                    name: "GAME"
                    PropertyChanges { target: appboxui; visible: true}
                    PropertyChanges { target: mainmenu; visible: false}

                    StateChangeScript {
                            name: "myScript"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO GAME")
                                var js = {action: "ShowAppBox"}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))
                            }
                        }
                }
            ]

        Component.onCompleted: {
            // for dev time
            //mainarea.push({item: gamesOnConsoleView, immediate: true})
        }
    }

    DownloadFeedbackBar {
        id: downloadFeedbackBar
        //visible: true // TODO: dev

        height: preferredHeight
        textPixelSize: gdisplay.mediumSize * gdisplay.ppmText
        initialOpacity: 0.6
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 100
        anchors.rightMargin: 100
    }

    // this fades after timeout automatically away -> could be common for all views
    GFeedbackDialog {
        id: feedbackDialog
        visible: false
        //feedbackMessage: "This is a test message, quite long This is a test message, quite long This is a test message, quite long"
        showingTime: 3000 // ms
        height: preferredHeight
        textPixelSize: gdisplay.mediumSize * gdisplay.ppmText
        initialOpacity: 0.6
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    // --- FUNCTIONS

    function initExistingPlayers()
    {
        playersManager.playerIds()
            .map(function(pid) { onPlayerIn(pid, playersManager.playerName(pid)) })
    }

    function onPlayerIn(pid, pname)
    {
        var js = {action: "DefineGeneralActions",
                  actions: []} // {actionId: "GameMenu", actionName: "Menu"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "ShowBasicControls",
              enable: mainarea.currentItem.enabledControlActions}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (noWebstoreConnectionDialog.visible) {
            // TODO: to single place
            js = {action: "ConfirmationQuestion",
                      questionId: "noWebstoreConnectionDialog",
                      title: qsTr("Error"),
                      text: qsTr("No connection"),
                      options: [{id:   "ok",
                                 text: qsTr("OK")} ]
                     }

            playersManager.sendPlayerMessage(pid, JSON.stringify(js))
        }

        else if (exitConfirmationDialog.visible) {
            // TODO: defining this message should be in one place
            js = {action: "ConfirmationQuestion",
                      questionId: exitConfirmationDialog.questionId,
                      text: exitConfirmationDialog.questionText,
                      options: [{id:   exitConfirmationDialog.option1Id,
                                 text: exitConfirmationDialog.option1Text},
                                {id:   exitConfirmationDialog.option2Id,
                                 text: exitConfirmationDialog.option2Text}]
                     }

            playersManager.sendPlayerMessage(pid, JSON.stringify(js))
        }
    }

    function onPlayerOut(pid, pname)
    {
        if (ActivePlayerModel.hasActivePlayer && pid === ActivePlayerModel.activePlayerId) {
            ActivePlayerModel.deactivatePlayer()
            var msg = qsTr("Active player " + pname + "left")
            msgToSend = {action: "FeedbackMessage",
                         message: msg}
            playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
            feedbackDialog.show(msg)
        }

    }

    function onPlayerMessageReceived(pid, data)
    {
        // TODO: some common framework to push move actions

        console.log("Player message: id = " + pid)
        //messageBoard.insertPlayerMessage(pid, data)

        var js  = JSON.parse(data)
        if (js["action"] === "SelectBasicControlAction")
        {
            mainarea.currentItem.processControlAction(js["id"], pid)

        } else if (js["action"] === "ConfirmationQuestionResponse") {
            // TODO: case when multiple possible confirmations -> should use id
            if (js["questionId"] === "noWebstoreConnectionDialog") {
                if (noWebstoreConnectionDialog.visible) {
                    noWebstoreConnectionDialog.visible = false
                    // just to make sure everyones dialogs are closed
                    playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
                }
            }

            else if (exitConfirmationDialog.visible) {
                exitConfirmationDialog.selectOption(js["ref"])

                // just to make sure everyones dialogs are closed
                playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
            }

        }
    }

    // background fading as in other cases -> common component?
    GConfirmationDialog {
        id: exitConfirmationDialog
        visible: false // initial state
        questionId: "exitconfirmation"
        questionText: qsTr("Exit and stop using console? ")
        option1Text: qsTr("Yes")
        option2Text: qsTr("No")
        border.color: "gray"
        border.width: 1

        onOption1Selected: {
            // Yes
            console.debug("Exit selected")

            // if comms started us we want to quit comms too and that will
            // individually started mainui
            if (Connection.isActivated)
                ApplicationManager.exitConsole()
            else
                Qt.quit()

            //Qt.quit()
        }

        onOption2Selected: {
            exitConfirmationDialog.visible = false
            mainMenuView.focus = true
        }

        Keys.onPressed: {
            console.debug("######## KEY PRESSED (exitConfirmationDialog)")
            if (event.key === Qt.Key_Return)
                selectOption(option1Id)
            if (event.key === Qt.Key_Escape)
                selectOption(option2Id)
        }
    }

    GErrorDialog {
        id: notImplementedDialog
        visible: false // initial state
        errorMessage: qsTr("This feature haven't been implemented yet.")

        border.color: "gray"
        border.width: 1

        onAcknowledged: {
            visible = false
        }

    }

    GErrorDialog {
        id: noWebstoreConnectionDialog
        visible: false // initial state
        errorMessage: qsTr("Webstore can't be accessed as there is no working network connection to the store.")

        border.color: "gray"
        border.width: 1

        onAcknowledged: {
            visible = false
        }

    }

// ----------------------------------------------------------------------------

    function onGamesOnConsoleSelected() {
        console.debug("Games on console selected")
        // TODO: appbox version handshaking at some point
        gamesOnConsoleView.visible = true
        // TODO: stack view??

        /*
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
        */

    }

    function exitGameSelected() {
        console.debug("Exit selected")

        // as demo send confirmation to clients

        // TODO: how localization of these texts would go?

        var js = {action: "ConfirmationQuestion",
                  questionId: exitConfirmationDialog.questionId,
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
        exitConfirmationDialog.focus = true

        // TODO: disable everything else -> record state
    }

}
