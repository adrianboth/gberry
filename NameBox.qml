import QtQuick 2.0

Rectangle {
    id: self
    property string name: "undefined"
    property color bgcolor: "white" // just default, each box should get own color
    width: nameBoxText.implicitWidth + gdisplay.smallSize * gdisplay.ppmText
    height: nameBoxText.implicitHeight + gdisplay.smallSize * gdisplay.ppmText

    Text {
        id: nameBoxText
        text: self.name
        anchors.centerIn: parent
        font.pixelSize: gdisplay.smallSize * gdisplay.ppmText
    }
}

