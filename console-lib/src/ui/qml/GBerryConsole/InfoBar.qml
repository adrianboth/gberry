import QtQuick 2.0

Rectangle {
    id: infobar

    property string numberOfPlayers: playersManager.numberOfPlayers
    property string playersText: "Players: " + playersManager.numberOfPlayers
    property string commsStatusText: Connection.isActivated ? qsTr("Connected") : "--"
    property string headServerStatusText: Connection.isActivated ? qsTr("Connected") : "--"
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
        id: headServerStatus
        text: qsTr("HeadServer: ") + headServerStatusText
        anchors.left: commsStatus.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 50
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
             // TODO: should be property instead
             //console.debug("### UPDATING " + Connection.isActivated)
             time.text = new Date().toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
             infobar.commsStatusText = Connection.isActivated ? qsTr("Connected") : "--"
             infobar.headServerStatusText = Connection.isHeadServerConnected ? qsTr("Connected") : "--"
             //infobar.commsStatusText = comms.isOpen() ? qsTr("Connected") : "--"
         }
     }
}

