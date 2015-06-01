import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0


Window {
    id: root
    visible: true
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    GSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }


    Rectangle {
        id: mainarea
        color: "red"
        anchors.fill: parent

        border.color: "slategray"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("Starting ... please wait ...")
            font.pixelSize: gdisplay.mediumSizeText

            // not best and smoothiest animation ... but enough for testing
            SequentialAnimation on font.pixelSize {
                loops: Animation.Infinite

                PropertyAnimation { duration: 2000; to: gdisplay.mediumSizeText * 1.2 }
                PropertyAnimation { duration: 2000; to: gdisplay.mediumSizeText * 0.8}
            }
        }


    }

    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")

        var js = {action: "DefineGeneralActions",
                  actions: [{}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "DefineAppBox",
                  data: AppBoxMaster.dataStr()}

        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "ShowAppBox"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))
    }

    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "Player left")
    }
    function onPlayerMessageReceived(pid, data)
    {

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

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")
    }
}
