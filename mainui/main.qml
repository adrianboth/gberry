import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600

    MainForm {
        id: ui
        anchors.fill: parent

        property string numberOfPlayers: playersManager.numberOfPlayers
        property string playersText: "Players: " + playersManager.numberOfPlayers
        property string commsStatusText: comms.isOpen() ? "OK" : "NOK"

        mouseArea.onClicked: {
            Qt.quit();
        }

    }

    MessageBoard {
        id: messageBoard

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    Timer {
         interval: 500; running: true; repeat: true
         onTriggered: {
             time.text = Date().toString()
             ui.commsStatusText = comms.isOpen() ? "OK" : "NOK"
         }
     }

     Text {
         id: time
         anchors.top: parent.top
         anchors.right: parent.right
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
    }

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)
    }
}
