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

    property real buttonOpacity: 1.0
    property int buttonTextFontPixelSize: gdisplay.mediumSizeText
    property int feedbackTextSize: gdisplay.smallSizeText
    property int pointsTextSize: gdisplay.smallSizeText * 0.75

    property int points: 10 // TODO: test points

    property color boxAreaBorderColor: "gray"
    property color boxAreaBackgroundColor: "snow"
    property color answerNormalBackgroundColor: "lightblue"
    property color answerWrongBackgroundColor: "#FFB6C1" // light pink
    property color answerCorrectBackgroundColor: "blue"

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
                text: "SOME TEXT FEED BACK" // "undefined"
                anchors.centerIn: parent
                font.pixelSize: self.feedbackTextSize

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

        Rectangle {
            id: answerArea
            color: "snow"
            radius: 10
            //Layout.fillWidth: true
            //Layout.preferredHeight: feedbackText.implicitHeight * 1.5
            //Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: answerButtonBox.height + gdisplay.touchCellHeight()
            Layout.preferredWidth: answerButtonBox.width + gdisplay.touchCellWidth()

            ColumnLayout {
                id: answerButtonBox
                anchors.centerIn: parent

                Rectangle {
                    id: button1
                    property string answerId: "a"
                    property string answerLabel: answerId.toUpperCase()

                    Layout.preferredWidth: Layout.preferredHeight * 3
                    Layout.preferredHeight: answerIdLabel.implicitHeight

                    color: self.answerNormalBackgroundColor
                    radius: 10
                    antialiasing: true
                    opacity: buttonOpacity

                    state: "WAIT_SELECTION"
                    onStateChanged: {
                        if (state === "WAIT_SELECTION")
                            color = self.answerNormalBackgroundColor
                        else if (state === "CORRECT")
                            color = self.answerCorrectBackgroundColor
                        else if (state === "NOT_CORRECT")
                            color = self.answerWrongBackgroundColor
                        else
                            console.warn("Unknown state: " + state)
                    }

                    Text {
                        id: answerIdLabel
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            buttonPressed(parent.answerId)
                        }
                    }
                }

                Rectangle {
                    id: button2
                    property string answerId: "b"
                    property string answerLabel: answerId.toUpperCase()

                    Layout.preferredWidth: Layout.preferredHeight * 3
                    Layout.preferredHeight: answerIdLabel.implicitHeight

                    color: self.answerNormalBackgroundColor
                    radius: 10
                    antialiasing: true
                    opacity: buttonOpacity

                    state: "WAIT_SELECTION"
                    onStateChanged: {
                        if (state === "WAIT_SELECTION")
                            color = self.answerNormalBackgroundColor
                        else if (state === "CORRECT")
                            color = self.answerCorrectBackgroundColor
                        else if (state === "NOT_CORRECT")
                            color = self.answerWrongBackgroundColor
                        else
                            console.warn("Unknown state: " + state)
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            buttonPressed(parent.answerId)
                        }
                    }
                }

                Rectangle {
                    id: button3
                    property string answerId: "c"
                    property string answerLabel: answerId.toUpperCase()

                    Layout.preferredWidth: Layout.preferredHeight * 3
                    Layout.preferredHeight: answerIdLabel.implicitHeight

                    color: self.answerNormalBackgroundColor
                    radius: 10
                    antialiasing: true
                    opacity: buttonOpacity

                    state: "WAIT_SELECTION"
                    onStateChanged: {
                        if (state === "WAIT_SELECTION")
                            color = self.answerNormalBackgroundColor
                        else if (state === "CORRECT")
                            color = self.answerCorrectBackgroundColor
                        else if (state === "NOT_CORRECT")
                            color = self.answerWrongBackgroundColor
                        else
                            console.warn("Unknown state: " + state)
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            buttonPressed(parent.answerId)
                        }
                    }
                }

                Rectangle {
                    id: button4
                    property string answerId: "d"
                    property string answerLabel: answerId.toUpperCase()

                    Layout.preferredWidth: Layout.preferredHeight * 3
                    Layout.preferredHeight: answerIdLabel.implicitHeight

                    color: self.answerNormalBackgroundColor
                    radius: 10
                    antialiasing: true
                    opacity: buttonOpacity

                    state: "WAIT_SELECTION"
                    onStateChanged: {
                        if (state === "WAIT_SELECTION")
                            color = self.answerNormalBackgroundColor
                        else if (state === "CORRECT")
                            color = self.answerCorrectBackgroundColor
                        else if (state === "NOT_CORRECT")
                            color = self.answerWrongBackgroundColor
                        else
                            console.warn("Unknown state: " + state)
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            buttonPressed(parent.answerId)
                        }
                    }
                }
            }
        }

        Item {
            id: pointsArea
            Layout.fillWidth: true
            Layout.preferredHeight: pointsLabel.implicitHeight * 1.5
            Text {
                id: pointsLabel
                anchors.centerIn: parent
                text: qsTr("Points") + " " + self.points
                font.pointSize: self.pointsTextSize
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
        console.debug("### MAINUI APPBOX MSG: " + js.toString())

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
        } else if (js["action"] === "Ping") {
            console.debug("Ping received: " + js["msg"])
        } else {
            console.warn("Unknown command: " + js["action"])
        }

    }

    function buttonPressed(answerId) {
        console.debug("Button pressed: " + answerId)

        if (self.enabled) {
            // TODO: send
            var js = {action: "AnswerSelected", number: answerId}
            console.debug("Sending message out from AppBox")
            outgoingMessage(js)
        }
    }
}

// TODO: try to use components defined in a pkg

