import QtQuick 2.0

Rectangle {
    height: 100
    border.width: 1
    anchors.bottomMargin: -1 // hide bottom border on screen
    color: "yellow"
    opacity: 0.5

    property string messageText: ""

    function insertMessage(msg) {
        messageText = msg + "\n" + messageText
        // TODO: cut lines at some point
    }

    function insertPlayerMessage(pid, msg) {
        var pname = playersManager.playerName(pid)
        insertMessage("[" + pname +"] " + msg)
    }

    Text {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 5
        id: textArea
        text: messageText
    }
}

