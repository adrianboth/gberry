import QtQuick 2.0
import GBerry 1.0

Item {


    GButton {
        anchors.top: parent.top
        anchors.left: parent.left

        height: preferredHeight
        width: preferredWidth

        buttonText: qsTr("Back")

    }

    Rectangle {
        anchors.centerIn: parent
        color: red
        width: 100
        height: 100
    }
}

