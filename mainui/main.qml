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
        //property string messageText: "..."
        property string commsStatusText: comms.isOpen() ? "OK" : "NOK"

        mouseArea.onClicked: {
            Qt.quit();
        }

    }

    MessageBoard {
        id: messageBoard

    }

    Timer {
         interval: 500; running: true; repeat: true
         onTriggered: time.text = Date().toString()
     }

     Text { id: time
     anchors.bottom: parent
     }

    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        var pname = playersManager.playerName(pid)
        ui.messageText = "New player in: " + pname + "(" + pid + ")"
    }
    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)
        ui.messageText = "Player left: id = " + pid
    }
    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        ui.messageText = "" + pid + ": " + data
    }

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)
    }
}
