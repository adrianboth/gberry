/**
 * ConfirmationDialog
 */
import QtQuick 2.4
import QtQuick.Layouts 1.1

import "js/DeveloperLog.js" as Log

Rectangle {
    id: dialog

    // public
    property string questionText: "<undefined>"
    property string titleText: "<undefined>"
    property bool showTitle: false
    property string option1Text: "<answer1>"
    property string option2Text: "<answer2>"
    property string option1Id: "option1"
    property string option2Id: "option2"

    // TODO: size option

    signal option1Selected()
    signal option2Selected()

    function selectOption(optionId) {
        if (optionId === option1Id) {
            option1Selected()
        } else if (optionId === option2Id) {
            option2Selected()
        } else {
            Log.error("Unknow option: " + optionId)
        }
    }


    // private
    height: confirmationText.height + buttonRow.height
    width: buttonRow.width
    anchors.centerIn: parent
    color: "snow"

    // TODO: fix
    property int margin: 25

    // TODO: title

    Text {
        id: confirmationText
        text: questionText
        anchors.horizontalCenter: dialog.horizontalCenter
        anchors.top: dialog.top
        anchors.topMargin: margin
        font.bold: true
        font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText // TODO: to somewhere else
    }

    Rectangle  {
        id: buttonRow
        height: option1Button.height + margin
        // left margin, button, margin between buttons, button, right margin
        // TODO: fix (binding loop)

        width: margin + option1Button.width + margin + option2Button.width + margin

        anchors.top: confirmationText.bottom
        anchors.topMargin: margin
        anchors.left: dialog.left

        RowLayout {
            //anchors.fill: parent
            anchors.centerIn: parent
            // TODO: fix
            spacing: 50
            GButton {
                id: option1Button
                label: option1Text
                onButtonClicked: option1Selected()
            }
            GButton {
                id: option2Button
                label: option2Text
                onButtonClicked: option2Selected()
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_1) { option1Selected(); event.accepted = true }
        if (event.key === Qt.Key_2) { option2Selected()(); event.accepted = true }
    }

    Component.onCompleted: {
        Log.initLog("ConfirmatioDialog", gsettings.logLevel)
    }
}
