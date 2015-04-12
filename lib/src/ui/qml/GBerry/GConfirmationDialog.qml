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
    property int textMargin: 0.2 * confirmationText.height // from one text line height
    property int buttonMargin: textMargin

    height: column.height
    width: column.width
    anchors.centerIn: parent
    color: "snow"
    radius: 20 // TODO: should change based on screen size?

    // TODO: titlebar

    ColumnLayout {
        id: column

        Item {
            id: textarea
            Layout.preferredHeight: confirmationText.contentHeight + 2 * textMargin // margin is based on line height
            Layout.preferredWidth: confirmationText.width + 2 * textMargin

            Text {
                id: confirmationText
                text: questionText

                y: parent.y + textMargin // top margin
                anchors.horizontalCenter: textarea.horizontalCenter

                font.bold: true
                font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText // TODO: to somewhere else

                // needed for text wrapping to work
                width: Math.min(implicitWidth, root.width * 0.75) // max width 75% from screen

                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }

        Item  {
            id: buttonRow
            Layout.preferredHeight: option1Button.height + 2* buttonMargin
            Layout.preferredWidth: option1Button.width + option2Button.width +  3 * buttonMargin
            Layout.alignment: Qt.AlignHCenter

            RowLayout {
                //anchors.fill: parent
                anchors.centerIn: parent
                spacing: option1Button.height * 0.5
                GButton {
                    id: option1Button
                    label: option1Text
                    onButtonClicked: option1Selected()
                }
                GButton {
                    id: option2Button
                    label: option2Text
                    onButtonClicked: {
                        //console.debug("dialog height: " + dialog.height)
                        //console.debug("buttonRow height: " + buttonRow.height)
                        //console.debug("text height: " + confirmationText.height)
                        //console.debug("text height: " + confirmationText.implicitHeight)
                        //console.debug("text height: " + confirmationText.contentHeight)
                        option2Selected()
                    }
                }
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
