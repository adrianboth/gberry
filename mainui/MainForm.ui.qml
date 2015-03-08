import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 800
    height: 600

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        Column {
            id: column
            width: 200
            height: 400
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottom: parent.bottom
        }

        Text {
            id: message
            x: 356
            y: 585
            text: messageText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            font.pixelSize: 12
        }

        Row {
            id: row1
            height: 400
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            Text {
                id: players
                text: playersText
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: commsStatus
                text: qsTr("Comms: ") + commsStatusText
                anchors.left: parent.left
                anchors.leftMargin: 0
                font.pixelSize: 12
            }
        }
    }
}
