import QtQuick 2.3

// TODO: there was way to separate Design from logic ...
Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Text {
        id: serverText
        anchors.centerIn: parent
        text: serverTextStr
        anchors.verticalCenterOffset: -22
        anchors.horizontalCenterOffset: -4
    }

    Text {
        id: consoleText
        anchors.centerIn: parent
        text: consoleTextStr
    }
}
