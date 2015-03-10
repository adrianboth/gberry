import QtQuick 2.0

Rectangle {
    width: 100
    height: 62

    signal connectButtonClicked()

    Text {
        id: connectButtonText
        text: qsTr("Connect")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
    }

    MouseArea {
        id: connectButton
        anchors.fill: parent
        enabled: true
        opacity: 0.5
        onClicked: connectButtonClicked()
    }
}

