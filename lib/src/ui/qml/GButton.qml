/**
 * Button for dialogs and menu.
 */
import QtQuick 2.0

import "qrc:/ui/gberry-lib/js/DeveloperLog.js" as Log

// TODO: how to adjust text size according to screen size
// TODO: what about different text size in different languages

Rectangle {
    id: button
    color: "#2db6e1"
    width: buttonLabel.width + (25*2)
    height: buttonLabel.height + (12*2)
    //width: 107
    //height: 55
    antialiasing: true
    property string label

    signal buttonClicked()

    Text {
        id: buttonLabel
        smooth: true
        text: label
        anchors.centerIn: parent
        font.bold: true
        font.pixelSize: dimensions.defaultTextPixelSize * dimensions.textScale
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        onClicked: { buttonClicked();
            Log.debug("button w: " + (button.width - buttonLabel.width))
            Log.debug("button h: " + (button.height - buttonLabel.height))
        }
    }

    Component.onCompleted: {
        Log.initLog("Button/" + buttonLabel.text, _CONSTANTS.logLevel)
        Log.debug("width: " + width + ", height: " + height)
    }
}
