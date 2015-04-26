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
    property string errorMessage: "<undefined>"
    property bool showTitle: false
    property string acknowledgeText: qsTr("OK")

    signal acknowledged()

    function close() {
        dialog.visible = false
    }

    // private
    property int textMargin: gdisplay.touchCellHeight() // from one text line height

    /*
    function debug() {
        console.debug("errorText.height: " + errorText.height)
        console.debug("errorText.contentHeight: " + errorText.contentHeight)
        console.debug("errorText.implicitHeight: " + errorText.implicitHeight)
        console.debug("errorText.width: " + errorText.width)
        console.debug("acknowledgeButton w+h:" + acknowledgeButton.width + "x" + acknowledgeButton.height + ", text=" + acknowledgeButton.label)
    }

    onVisibleChanged: debug()
    */

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
            Layout.preferredHeight: errorText.contentHeight + 2 * textMargin // margin is based on line height
            Layout.preferredWidth: errorText.width + 2 * textMargin
            Layout.fillWidth: true

            Text {
                id: errorText
                text: errorMessage

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
            id: buttonBox

            Layout.preferredHeight: acknowledgeButton.height + gdisplay.touchCellHeight()
            Layout.minimumWidth: acknowledgeButton.width + gdisplay.touchCellWidth()
            Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter


            GButton {
                id: acknowledgeButton
                label: acknowledgeText
                anchors.centerIn: parent
                width: buttonWidth
                height: buttonHeight
                onButtonClicked: {
                    dialog.visible = false
                    acknowledged()
                }
            }
        }

    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Enter) { acknowledge(); event.accepted = true }
    }

    Component.onCompleted: {
        Log.initLog("GErrorDialog", gsettings.logLevel)
    }
}
