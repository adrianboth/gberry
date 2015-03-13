import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import "AppBoxMaster.js" as AppBoxMaster
import "js/Messages.js" as MessagesJS

Window {
    id: root
    visible: true
    width: 800
    height: 600

    InfoBar {
        id: infobar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        z: 100
    }

    MessageBoard {
        id: messageBoard

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    Rectangle {
        id: mainarea
        anchors.top: infobar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: messageBoard.top

        Menu {
            id: menu
            focus: true // TODO: needed?
            anchors.centerIn: parent
        }

        AppBoxUI {
            id: appboxui
            color: "orange"
            anchors.fill: parent
        }

        state: "MENU"
        states: [
                State {
                    name: "MENU"
                    PropertyChanges { target: appboxui; visible: false }
                    PropertyChanges { target: menu; visible: true }
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
                    PropertyChanges { target: menu; visible: false}

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
    }

    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "GameMenu", actionName: "Menu"}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (mainarea.state === "MENU") {
            js = {action: "ShowBasicControls"}
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
    }
    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)

        var js  = JSON.parse(data)
        if (js["action"] === "SelectBasicControlAction")
        {
            if (js["id"] === "Up")
                menu.moveFocusToNext()
            else if (js["id"] === "Down")
                menu.moveFocusToPrevious()
            else if (js["id"] === "OK")
                menu.selectCurrent()

        } else if (js["action"] === "ConfirmationQuestionResponse") {
            // TODO: case when multiple possible confirmations
            if (exitConfirmationDialog.visible) {
                if (js["ref"] === "Yes") {
                    // this is not in official docs
                    exitConfirmationDialog.click(StandardButton.Yes)
                }
                if (js["ref"] === "No") {
                    exitConfirmationDialog.click(StandardButton.No)
                }
                // just to make sure everyones dialogs are closed
                playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
            }

        } else if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION     ")
                mainarea.state = "MENU"
            }
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
    function exitGameSelected() {
        console.debug("Exit selected")

        // as demo send confirmation to clients

        // TODO: how localization of these texts would go?

        var js = {action: "ConfirmationQuestion",
                  title: "Confirmation",
                  text: "Are you sure to exit this game?",
                  options: [{id: "Yes", text: "Yes"}, {id: "No", text: "No"}]
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

    MessageDialog {
        id: exitConfirmationDialog
        visible: false // initial state
        title: "Exit Confirmation"
        text: "Are you sure to exit this game?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onNo: exitConfirmationDialog.close()
        onYes: Qt.quit()
    }

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        menu.playGameSelected.connect(root.playGameSelected)
        menu.exitGameSelected.connect(root.exitGameSelected)

        AppBoxMaster.loadAppBoxResources()
    }
}
