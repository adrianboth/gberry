/**
 * ConfirmationDialog
 */
import QtQuick 2.4
import QtQuick.Layouts 1.1

import "js/DeveloperLog.js" as Log


Rectangle {
    id: dialog
    visible: false // by default hidden
    z: 1000 // tries to be always top most item

    // public
    property string questionText: "<undefined>"
    property string questionId: "<undefined>"
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
    //property int buttonMargin: textMargin

    function debug() {
        console.debug("confirmationText.height: " + confirmationText.height)
        console.debug("confirmationText.width: " + confirmationText.width)
        console.debug("option1Button w+h:" + option1Button.width + "x" + option1Button.height + ", text=" + option1Button.label)
        console.debug("option2Button w+h:" + option2Button.width + "x" + option2Button.height + ", text=" + option2Button.label)
    }

    height: column.height
    width: column.width
    anchors.centerIn: parent
    color: "snow"
    radius: 20 // TODO: should change based on screen size?

    // TODO: titlebar

    ColumnLayout {
        id: column
        anchors.centerIn: parent
        spacing: 0

        Item {
            id: textarea
            Layout.preferredHeight: confirmationText.contentHeight + 2 * textMargin // margin is based on line height
            Layout.preferredWidth: confirmationText.width + 2 * textMargin
            Layout.fillWidth: true

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
            //color: "red"
            id: buttonRow

            Layout.preferredHeight: buttonRowLayout.maxButtonHeight + gdisplay.touchCellHeight()
            Layout.preferredWidth: buttonRowLayout.width + buttonRowLayout.spacing * 2
            Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter

            RowLayout {
                id: buttonRowLayout
                //anchors.fill: parent
                anchors.centerIn: parent
                spacing: gdisplay.touchCellWidth()

                property int maxButtonWidth: Math.max(option1Button.buttonWidth, option2Button.buttonWidth)
                property int maxButtonHeight: option1Button.height // buttons have same height

                GButton {
                    id: option1Button
                    label: option1Text
                    Layout.preferredWidth: parent.maxButtonWidth
                    Layout.preferredHeight: buttonHeight
                    Layout.fillWidth: true
                    onButtonClicked: option1Selected()
                }

                GButton {
                    id: option2Button
                    label: option2Text
                    Layout.preferredWidth: parent.maxButtonWidth
                    Layout.preferredHeight: buttonHeight
                    Layout.fillWidth: true
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
