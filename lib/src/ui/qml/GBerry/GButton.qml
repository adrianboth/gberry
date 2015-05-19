/**
 * Button for dialogs and menu.
 */
import QtQuick 2.0

import "js/DeveloperLog.js" as Log

// TODO: how to adjust text size according to screen size
// TODO: what about different text size in different languages

Rectangle {
    id: button

    property color buttonBgColor: "#2db6e1"
    property color buttonFgColor: "black"

    color: buttonBgColor

    // We don't want to define width&height directly as this button
    // could be used also in layouts where you can define width&height.
    // So we have utilility properties
    property int buttonWidth: buttonLabel.implicitWidth + gdisplay.touchCellWidth() // + margins
    property int buttonHeight: buttonLabel.implicitHeight + gdisplay.touchCellHeight()

    radius: 20
    antialiasing: true
    property string label
    opacity: enabled ? 1 : 0.75

    signal buttonClicked()

    Text {
        id: buttonLabel
        smooth: true
        text: label
        anchors.centerIn: parent
        font.bold: true
        font.pixelSize: gdisplay.touchCellHeight()
        color: buttonFgColor
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        property bool pressStarted: false

        onPressed: {
            pressStarted = true
            button.color = Qt.darker(buttonBgColor)
            buttonLabel.color = Qt.lighter(buttonFgColor)
        }

        onExited: {
            console.debug("Exited button area")
            pressStarted = false
        }

        onReleased: {
            if (pressStarted) {
                buttonClicked()
            }

            pressFeedbackTimer.restart()

            //Log.debug("button w: " + (button.width - buttonLabel.width))
            //Log.debug("button h: " + (button.height - buttonLabel.height))
        }
    }

    Timer {
        id: pressFeedbackTimer
        running: false; repeat: false
        interval: 150 // ms
        onTriggered: {
            button.color = buttonBgColor
            buttonLabel.color = buttonFgColor
        }
    }


    Component.onCompleted: {
        Log.initLog("Button/" + buttonLabel.text, gsettings.logLevel)
        Log.debug("width: " + width + ", height: " + height)
    }
}
