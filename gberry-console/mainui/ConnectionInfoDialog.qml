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

import GBerry 1.0

Rectangle {
    id: self
    visible: false // by default hidden
    z: 1000 // tries to be always top most item

    // public
    property color bgColor: "snow"
    property real initialOpacity: 1
    property int textPixelSize: gdisplay.smallSize * gdisplay.ppmText

    property int preferredHeight: mainarea.height + 2*textMargin
    property int preferredWidth: mainarea.width + 2*textMargin

    signal closed()

    function triggerCloseFade() {
        fadeAnimator.running = true
    }

    function close() {
        self.visible = false
        self.opacity = initialOpacity
        closed()
    }


    // private
    property int textMargin: gdisplay.touchCellHeight() // from one text line height

    color: "snow"
    radius: 20 // TODO: should change based on screen size?
    opacity: initialOpacity

    ColumnLayout {
        id: mainarea
        anchors.centerIn: parent

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Waiting players ...")
            font.bold: true
            font.pixelSize: gdisplay.mediumSizeText * 1.5
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: gdisplay.touchCellHeight()
        }

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: {
                var msg
                if (playersManager.numberOfPlayers === 1)
                    msg = qsTr(" player connected")
                else
                    msg = qsTr(" players connected")

                return playersManager.numberOfPlayers.toString() + msg
            }

            font.pixelSize: gdisplay.mediumSizeText
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.top: parent.top
            //anchors.topMargin: gdisplay.touchCellHeight()
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: gdisplay.touchCellHeight()
        }

        GridLayout {
            id: textarea
            //height: textLabel.contentHeight + textMargin // margin is based on line height
            //width: textLabel.width // + 2 * textMargin
            //anchors.centerIn: parent
            columns: 3

            Text {
                text: "WLAN"
                font.bold: true
                font.pixelSize: textPixelSize
            }
            Text { text: " " }
            Text { text: " " }
            Text { text: " " }
            Text {
                text: "SSID"
                Layout.preferredWidth: 2*implicitWidth
                font.pixelSize: textPixelSize
            }
            Text {
                text: "GBerryConsole"
                font.pixelSize: textPixelSize
            }
            Text { text: " " }
            Text {
                Layout.preferredWidth: 2*implicitWidth
                text: "Password"
                font.pixelSize: textPixelSize
            }

            Text {
                text: "f1wAre2015"
                font.pixelSize: textPixelSize
            }

            Text {
                text: "Console"
                font.bold: true
                font.pixelSize: textPixelSize
            }
            Text { text: " " }
            Text { text: " " }
            Text { text: " " }
            Text {
                text: "Address"
                Layout.preferredWidth: 2*implicitWidth
                font.pixelSize: textPixelSize
            }

            Text {
                text: "192.168.42.1"
                font.pixelSize: textPixelSize
            }
        }
    }

    SequentialAnimation {
        id: fadeAnimator
        running: false

        OpacityAnimator {
            target: self
            from: initialOpacity
            to: 0
            duration: 1000
            running: false
        }
        ScriptAction { script: close() }
    }

    Timer {
        id: closingTimer
        //interval: showingTime
        //onTriggered: fadeAnimator.running = true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            triggerCloseFade()
        }
    }

    Component.onCompleted: {
        //Log.initLog("ConnectionInfo", ApplicationSettingsJS.logLevel)
    }
}
