import QtQuick 2.3

// TODO: there was way to separate Design from logic ...
Rectangle {
    id: rectangle2
    property alias mouseArea: mouseArea
    property alias connectButton: connectButton
    property alias connectButton1: connectButton1

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
    }

    Text {
        id: serverText
        anchors.centerIn: parent
        text: serverTextStr
    }

    Text {
        id: consoleText
        text: consoleTextStr
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: serverText.bottom
    }

    Rectangle {
        id: buttonFrame
        x: 132
        width: 74
        height: 50
        color: "#f81e1e"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: consoleText.bottom
        anchors.topMargin: 2

        Text {
            id: connectButtonText
            x: 25
            y: 18
            width: 59
            height: 0
            text: qsTr("Connect")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 16
        }

        MouseArea {
            id: connectButton
            anchors.fill: parent
            enabled: true
            opacity: 0.5
        }
    }

    ConnectButton {
        id: connectButton1
        x: 130
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: buttonFrame.bottom
        anchors.topMargin: 6
    }
}
