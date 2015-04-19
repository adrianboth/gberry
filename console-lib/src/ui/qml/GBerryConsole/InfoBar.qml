import QtQuick 2.0

Rectangle {
    id: infobar

    property string numberOfPlayers: playersManager.numberOfPlayers
    property string playersText: "Players: " + playersManager.numberOfPlayers
    property string commsStatusText: comms.isOpen() ? "OK" : "NOK"
    property string timeText: "-"

    height: 30

    opacity: 0.5
    color: "grey"

    Text {
        id: commsStatus
        text: qsTr("Comms: ") + commsStatusText
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
        font.pixelSize: 12
    }

    Text {
        id: players
        text: playersText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: time
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        text: timeText
    }

    Timer {
         interval: 500; running: true; repeat: true
         onTriggered: {
             time.text = new Date().toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
             infobar.commsStatusText = comms.isOpen() ? "OK" : "NOK"
         }
     }
}

