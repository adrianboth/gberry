import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import "AppBoxMaster.js" as AppBoxMaster


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

    MainForm {
        id: ui
        anchors.fill: parent
    }

    MessageBoard {
        id: messageBoard

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    Menu {
        id: menu
        focus: true
        anchors.centerIn: ui
    }


    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")
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
        }
        if (js["action"] === "ConfirmationQuestionResponse")
        {
            // TODO: case when multiple possible confirmations
            if (exitConfirmationDialog.visible) {
                if (js["ref"] === "Yes") {
                    // this is not in official docs
                    exitConfirmationDialog.click(StandardButton.Yes)
                }
                if (js["ref"] === "No") {
                    exitConfirmationDialog.click(StandardButton.No)
                }
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
        playersManager.sendPlayerMessage(playersManager.playerIds(), JSON.stringify(js))

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

        //menu.forceActiveFocus()

        AppBoxMaster.loadAppBoxResources()
    }
}
