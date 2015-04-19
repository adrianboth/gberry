import QtQuick 2.0

Rectangle {
    // what number is shown in button
    property int number: 0

    // TODO: scaling on correct proportions
    width: gdisplay.largeSize * gdisplay.ppmX
    height: gdisplay.largeSize * gdisplay.ppmY

    color: "gray"
    border.width: 2
    border.color: "lightgray"
    radius: 5

    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightgray" }
        GradientStop { position: 1.0; color: "darkgray" }
    }

    Text {
        anchors.centerIn: parent
        text: number.toString()
        font.pixelSize: gdisplay.largeSize * gdisplay.ppmText
    }
}

