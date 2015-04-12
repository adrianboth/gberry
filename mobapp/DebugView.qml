import QtQuick 2.4

Rectangle {
    anchors.fill: parent

    property string serverTextStr: "Server (INIT)"
    property string consoleTextStr: "Console (INIT)"

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

}
