/**
 * Button for dialogs and menu.
 */
import QtQuick 2.0

import "js/DeveloperLog.js" as Log

// TODO: how to adjust text size according to screen size
// TODO: what about different text size in different languages

Rectangle {
    id: button
    color: "#2db6e1"
    width: buttonLabel.width + (25*2)
    height: buttonLabel.height + (12*2)
    radius: 20
    antialiasing: true
    property string label

    signal buttonClicked()

    Text {
        id: buttonLabel
        smooth: true
        text: label
        anchors.centerIn: parent
        font.bold: true
        font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText
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
        Log.initLog("Button/" + buttonLabel.text, gsettings.logLevel)
        Log.debug("width: " + width + ", height: " + height)
    }
}
