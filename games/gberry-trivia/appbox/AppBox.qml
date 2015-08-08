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
 
import QtQuick 2.0
import QtQuick.Layouts 1.1

// TODO: create button that sends data back to mainui

Rectangle {
    id: self
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    property real buttonOpacity: 0.5
    property int buttonTextFontPixelSize: gdisplay.touchCellHeight()


    ColumnLayout {
        anchors.centerIn: parent

        Item {
            id: feedback
            //Layout.preferredWidth: feedbackText.implicitWidth * 1.5
            Layout.fillWidth: true
            Layout.preferredHeight: feedbackText.implicitHeight * 1.5
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                id: feedbackBox
                anchors.fill: parent
                radius: 10
            }

            Text {
                id: feedbackText
                text: "" // "undefined"
                anchors.centerIn: parent
                font.pixelSize: gdisplay.mediumSizeText * 0.75

                function setFeedback(txt) {
                    // onTextChanged() doesn't work as text is not every time really
                    // chaning, just same text again

                    feedback.setNormalMode()
                    feedbackText.text = txt
                    feedbackText.opacity = 1.0
                    feedbackTimer.running = true
                }
                Timer {
                    id: feedbackTimer
                    running: false
                    interval: 100
                    repeat: true
                    onTriggered: {
                        if (feedbackText.opacity == 0) {
                            feedbackTimer.running = false
                        } else {
                            feedbackText.opacity -= 0.1
                        }
                    }
                }
            }

            function setWaitMode() {
                feedbackText.text = "Wait..."
                feedbackBox.visible = true
                feedbackBox.color = "red"
                feedbackBox.border.width = 1
                feedbackBox.border.color = Qt.lighter(feedbackBox.color)
                feedbackBox.opacity = 1.0
                feedbackText.opacity = 1.0
            }
            function setNowMode() {
                feedbackText.text = "Now"
                feedbackBox.visible = true
                feedbackBox.color = "green"
                feedbackBox.border.width = 1
                feedbackBox.border.color = Qt.lighter(feedbackBox.color)
                feedbackBox.opacity = 1.0
                feedbackText.opacity = 1.0
                nowFadeTimer.running = true
            }

            function setNormalMode() {
                // no box visible, text will fade in normal mode
                feedbackBox.visible = false
                if (nowFadeTimer.running == true)
                    nowFadeTimer.running = false
            }

            Timer {
                id: nowFadeTimer
                interval: 100; repeat: true
                onTriggered: {
                    if (feedbackBox.opacity == 0) {
                        nowFadeTimer.running = false
                    } else {
                        feedbackBox.opacity -= 0.1
                        feedbackText.opacity -= 0.1
                    }

                }
            }

        }

        RowLayout {
            Rectangle {
                id: button1
                property int number: 1
                // TODO: make physically square
                width: buttonText1.implicitHeight
                height: buttonText1.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText1
                    text: button1.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button1.number)
                    }
                }
            }
            Rectangle {
                id: button2
                property int number: 2
                // TODO: make physically square
                width: buttonText2.implicitHeight
                height: buttonText2.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText2
                    text: button2.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button2.number)
                    }
                }
            }
            Rectangle {
                id: button3
                property int number: 3
                // TODO: make physically square
                width: buttonText3.implicitHeight
                height: buttonText3.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText3
                    text: button3.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button3.number)
                    }
                }
            }
        }

        RowLayout {
            Rectangle {
                id: button4
                property int number: 4
                // TODO: make physically square
                width: buttonText4.implicitHeight
                height: buttonText4.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText4
                    text: button4.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button4.number)
                    }
                }
            }
            Rectangle {
                id: button5
                property int number: 5
                // TODO: make physically square
                width: buttonText5.implicitHeight
                height: buttonText5.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText5
                    text: button5.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button5.number)
                    }
                }
            }
            Rectangle {
                id: button6
                property int number: 6
                // TODO: make physically square
                width: buttonText6.implicitHeight
                height: buttonText6.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText6
                    text: button6.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button6.number)
                    }
                }
            }
        }
        RowLayout {
            Rectangle {
                id: button7
                property int number: 7
                // TODO: make physically square
                width: buttonText7.implicitHeight
                height: buttonText7.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText7
                    text: button7.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button7.number)
                    }
                }
            }
            Rectangle {
                id: button8
                property int number: 8
                // TODO: make physically square
                width: buttonText8.implicitHeight
                height: buttonText8.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText8
                    text: button8.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button8.number)
                    }
                }
            }
            Rectangle {
                id: button9
                property int number: 9
                // TODO: make physically square
                width: buttonText9.implicitHeight
                height: buttonText9.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText9
                    text: button9.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button9.number)
                    }
                }
            }
        }
        RowLayout {
            Item { // to get last button into middle
                // TODO: make physically square
                width: buttonText0.implicitHeight
                height: buttonText0.implicitHeight
            }

            Rectangle {
                id: button0
                property int number: 0
                // TODO: make physically square
                width: buttonText0.implicitHeight
                height: buttonText0.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1
                radius: 10
                antialiasing: true
                opacity: buttonOpacity

                Text {
                    anchors.centerIn: parent
                    id: buttonText0
                    text: button0.number.toString()
                    font.pixelSize: self.buttonTextFontPixelSize
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button0.number)
                    }
                }
            }
        }

    }

    // -- API --

    // Outgoing AppBoxMessages
    //   data: json object
    signal outgoingMessage(var data)

    property bool enabled: false

    // Incoming AppBoxMessages
    //   js: json object
    function incomingMessage(js) {
        console.debug("### MAINUI APPBOX MSG: " + js)

        //outgoingMessage("got appbox msg")

        if (!"action" in js) {
            console.error("Unknown message format: " + js)
            return
        }

        if (js["action"] === "DisableControls") {
            // do not fade this text
            feedback.setWaitMode()
            buttonOpacity = 0.5
            self.enabled = false

        } else if (js["action"] === "EnableControls") {
            self.enabled = true
            feedback.setNowMode()
            buttonOpacity = 1.0

        } else if (js["action"] === "CorrectNumberFeedback") {
            // TODO: how localization of these would go? ... game provides, but client selects?
            feedbackText.setFeedback("OK!")
        } else if (js["action"] === "InvalidNumberFeedback") {
            feedbackText.setFeedback("Try again!")
        }
    }

    function buttonPressed(number) {
        console.debug("Button pressed: " + number.toString())

        if (self.enabled) {
            // TODO: send
            var js = {action: "NumberPressed", number: number.toString()}
            console.debug("Sending message out from AppBox")
            outgoingMessage(js)
        }
    }
}

// TODO: try to use components defined in a pkg
