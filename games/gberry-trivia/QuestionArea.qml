import QtQuick 2.4

Item {
    id: self

    property string questionText: "This a question text"

    Text {
        id: questionTextField
        anchors.fill: parent
        anchors.margins: gdisplay.touchCellHeight()
        font.pixelSize: gdisplay.mediumSizeText
        text: questionText
        wrapMode: Text.WordWrap
        // without specific width word wrapping doesn't work
        width: self.width - 2*gdisplay.touchCellHeight()
    }
}

