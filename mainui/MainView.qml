import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import GBerry 1.0
import GBerryConsole 1.0

Item {
    id: self

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
        anchors.fill: parent
        border.color: "slategray"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }
    }

    StackView {
        id: mainarea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: infobar.bottom
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

            menuItems: [
                MainMenuItem {
                    text: qsTr("Games on console")
                    onSelected: {
                        mainarea.push({item: gamesOnConsoleView, immediate: true})
                        //onGamesOnConsoleSelected()
                    }
                },
                MainMenuItem {
                    text: qsTr("Games on webstore")
                    onSelected: {
                        if (Connection.isHeadServerConnected) {
                            mainarea.push({item: gamesOnConsoleView, immediate: true})
                        } else {
                            noWebstoreConnectionDialog.visible = true
                        }
                    }
                },
                MainMenuItem {
                    text: qsTr("Settings")
                    onSelected: notImplementedDialog.visible = true
                },
                MainMenuItem {
                    text: qsTr("Exit")
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
                mainarea.pop({immediate: true})
            }

            // TODO: download
            /*
            onLaunchRequested: {
                console.debug("LAUNCH: " + gameId)
                ApplicationManager.launchApplication(gameId)
            }
            */

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

        // TODO: clear up box command ?
    }

    function onPlayerOut(pid, pname)
    {
        // nothing at the moment
    }

    function onPlayerMessageReceived(pid, data)
    {
        // TODO: some common framework to push move actions

        console.log("Player message: id = " + pid)
        //messageBoard.insertPlayerMessage(pid, data)

        var js  = JSON.parse(data)
        if (js["action"] === "SelectBasicControlAction")
        {
            mainarea.currentItem.processControlAction(js["id"])

        } else if (js["action"] === "ConfirmationQuestionResponse") {
            // TODO: case when multiple possible confirmations
            if (exitConfirmationDialog.visible) {
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
        questionText: qsTr("Exit and stop using console? ")
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

    GErrorDialog {
        id: notImplementedDialog
        visible: false // initial state
        errorMessage: qsTr("This feature haven't been implemented yet.")

        onAcknowledged: {
            visible = false
        }

    }

    GErrorDialog {
        id: noWebstoreConnectionDialog
        visible: false // initial state
        errorMessage: qsTr("Webstore can't be accessed as there is no working network connection to the store.")

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

}
