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
 
 import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

import GBerry 1.0

Rectangle {
    id: self
    anchors.fill: parent

    property string hostNameToConnect: "<undefined>"
    property string buttonText: qsTr("Connect")
    property string messageText: hostNameToConnect

    signal connectToConsoleRequested(var hostName)
    signal disconnectFromConsoleRequested()

    border.color: "slategray"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: gdisplay.smallSizeText * 0.5

        Item {
            //color: "green"
            Layout.fillWidth: true
            Layout.preferredHeight: messageLabel.implicitHeight

            Text {
                id: messageLabel
                text: messageText
                anchors.centerIn: parent
                wrapMode: Text.WordWrap
                width: Math.min(parent.width * 0.85, implicitWidth)
                font.pixelSize: gdisplay.smallSizeText
                horizontalAlignment: Text.AlignHCenter // center dispate of word wrapping
                onTextChanged: doLayout() // re-align after text changes (otherwise doesn't center correctly)
            }
        }

        Item {
            //color: "red"
            Layout.fillWidth: true
            Layout.preferredHeight: connectButton.height
            Layout.preferredWidth: connectButton.width

            GButton {
                id: connectButton
                width: buttonWidth
                height: buttonHeight
                enabled: state != "CONNECTING"

                label: buttonText
                labelTextPixelSize: gdisplay.smallSizeText

                anchors.centerIn: parent
                onButtonClicked: {
                    if (self.state === "DISCONNECTED") {
                        console.log("CONNECT TO " + self.hostNameToConnect)
                        connectToConsoleRequested(self.hostNameToConnect)
                    } else {
                        disconnectFromConsoleRequested()
                    }
                }
            }
        }
    }

    state: "DISCONNECTED" // initial state

    states: [
        State {
            name: "DISCONNECTED"
            PropertyChanges { target: self
                              buttonText: qsTr("Connect")
                              messageText: hostNameToConnect }
        },
        State {
            name: "CONNECTING"
            PropertyChanges { target: self
                              buttonText: qsTr("Disconnect")
                              messageText: qsTr("Connecting") + " " + hostNameToConnect }
        },
        State {
            name: "CONNECTED"
            PropertyChanges { target: self
                              buttonText: qsTr("Disconnect")
                              messageText: qsTr("Connected") + " " + hostNameToConnect }
        },
        State {
            name: "CONNECTED_NO_APP"
            PropertyChanges { target: self
                              buttonText: qsTr("Disconnect")
                              messageText: qsTr("Connected to") + " " + hostNameToConnect + " " + qsTr("but no application running. Waiting ...") }
        }

    ]

}

