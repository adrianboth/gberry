import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import GBerry 1.0
import GBerryConsole 1.0


Window {
    id: root
    visible: true
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    /*
    MainView {
        id: mainView
        anchors.fill: parent
        enabled: false
    }*/

    // Loader is required if
    //  a) Loading takes time
    //  b) or it would trigger sending message (e.g. to comms) -> need to wait connection ok

    Loader {
        id: mainViewLoader
        active: false
        source: "MainView.qml"
        visible: false
        anchors.fill: parent

        onLoaded: {
            item.initExistingPlayers()
            visible = true
            connectionWaitingDialog.visible = false
        }
    }

    Rectangle {
        id: connectionWaitingDialog
        visible: true
        color: "lightgreen"
        width: 0.75 * parent.width
        height: 0.75 * parent.height
        anchors.centerIn: parent

        Text {
            text: qsTr("Please wait ...")
            anchors.centerIn: parent
            font.pixelSize: gdisplay.mediumSizeText
        }

    }

    function onConnectionChanged() {
        // TODO: how to wait until connection ok
        console.debug("### onConnectionChanged()")
        if (Connection.isConnected) {
            console.debug("IS CONNECTED")
            mainViewLoader.active = true
        } else {
            connectionWaitingDialog.visible = true

            // TODO: how to disable
        }
    }

    function onActivatedChanged() {
        // currently this doesn't have any affect to mainui

        // as we don't keep any special list of player or states, this doesn't matter
        // but in other case we should discard our player list, reopens will come
    }

    function onPlayerIn(pid, pname)
    {
        console.log("New player in: id = " + pid + ", name = " + pname)
        if (mainViewLoader.status == Loader.Ready)
            mainViewLoader.sourceComponent.onPlayerIn(id, pname)
    }

    function onPlayerOut(pid, pname)
    {
        console.log("Player left: id = " + pid + ", name = " + pname)
        if (mainViewLoader.status == Loader.Ready)
            mainViewLoader.sourceComponent.onPlayerOut(id, pname)

    }

    function onPlayerMessageReceived(pid, data)
    {
        // TODO: some common framework to push move actions

        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)

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

    // TODO: how to pass function to view loaded by loader

    Component.onCompleted: {
        // make sure these are in place when connection opens
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        Connection.isConnectedChanged.connect(onConnectionChanged)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        // check current connection
        onConnectionChanged()
    }
}
