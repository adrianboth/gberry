/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
    property int textPixelSize: gdisplay.mediumSizeText
    property bool showTitle: false
    property string acknowledgeText: qsTr("OK")

    signal acknowledged()

    function close() {
        dialog.visible = false
    }

    // private
    property int textMargin: textPixelSize // from one text line height

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
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: errorText
                text: errorMessage

                y: parent.y + textMargin // top margin
                anchors.horizontalCenter: textarea.horizontalCenter

                font.bold: true
                font.pixelSize: textPixelSize // TODO: to somewhere else

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
                labelTextPixelSize: textPixelSize
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
