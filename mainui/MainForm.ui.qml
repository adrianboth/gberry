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


    }
}
