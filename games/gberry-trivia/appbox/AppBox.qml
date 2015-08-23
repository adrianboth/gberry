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
    property int buttonTextFontPixelSize: gdisplay.mediumSizeText * scaler
    property int feedbackTextSize: gdisplay.smallSizeText * scaler
    property int pointsTextSize: gdisplay.smallSizeText * 0.75 * scaler
    property real scaler: 1.0

    property int points: 0

    property color boxAreaBorderColor: "gray"
    property color boxAreaBackgroundColor: "snow"
    property color answerNormalBackgroundColor: "lightblue"
    property color answerWrongBackgroundColor: "#FFB6C1" // light pink
    property color answerCorrectBackgroundColor: "blue"
    property color answerMyCorrectBackgroundColor: "green"

    onHeightChanged: {
        console.debug("appbox height: " + mainLayout.height)
        console.debug("appbox default height: " + mainLayout.defaultHeight)
    }

    onVisibleChanged: {
        if (visible)
            calcScaling()
    }

    property int maxHeight: self.height
    property int maxWidth: self.width

    onMaxHeightChanged: { calcScaling() }
    onMaxWidthChanged: { calcScaling() }

    function calcScaling() {
        if (visible) {
            //console.log("############### CALC height")
            // how to make items smaller?
            //  -> decrease margin and buttons proportionally until ok
            //     -> use scaler

            // only if max have been defined
            var scalerCandinate = 1.0
            if (maxHeight !== 0) {
                var heightCandinate = mainLayout.defaultHeight
                //console.debug("### maxHeight: " + maxHeight + ", heightCandinate: " + heightCandinate)
                if (heightCandinate > maxHeight) {
                    scalerCandinate = maxHeight / heightCandinate
                    console.debug("### buttonScalerCandinate after height calc: " + scalerCandinate)
                }

            }

            if (maxWidth !== 0) {
                var widthCandinate = mainLayout.defaultWidth
                if (widthCandinate > maxWidth) {
                    var widthScalerCandinate = maxWidth / widthCandinate
                    if (widthScalerCandinate < scalerCandinate) {
                        scalerCandinate = widthScalerCandinate
                        //console.debug("### buttonScalerCandinate after width calc: " + scalerCandinate)
                    }
                }
            }

            scaler = scalerCandinate
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.centerIn: parent
        property int defaultHeight: feedback.defaultHeight + answerArea.defaultHeight + pointsArea.defaultHeight + 2*spacing
        property int defaultWidth: answerArea.defaultWidth

        Item {
            //color: "orange"
            id: feedback
            //Layout.preferredWidth: feedbackText.implicitWidth * 1.5
            //Layout.fillWidth: true
            Layout.preferredWidth: answerButtonBox.width + gdisplay.touchCellWidth() * scaler
            Layout.preferredHeight: feedbackText.implicitHeight * 1.5
            Layout.alignment: Qt.AlignHCenter

            property int defaultHeight:feedbackText.implicitHeight * 1.5

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
                function setStaticFeedback(txt) {
                    // onTextChanged() doesn't work as text is not every time really
                    // chaning, just same text again

                    feedback.setNormalMode()
                    feedbackText.text = txt
                    feedbackText.opacity = 1.0
                    feedbackTimer.running = false
                }
                Timer {
                    id: feedbackTimer
                    running: false
                    interval: 175
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
                interval: 200; repeat: true
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
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: (answerButtonBox.height + gdisplay.touchCellHeight()) * scaler
            Layout.preferredWidth: (answerButtonBox.width + gdisplay.touchCellWidth()) * scaler

            property int defaultHeight: 4*self.buttonTextFontPixelSize * 1.5 + gdisplay.touchCellHeight()
            property int defaultWidth : self.buttonTextFontPixelSize * 3 * 1.5

            property int buttonPressFeedbackTime: 250

            ColumnLayout {
                id: answerButtonBox
                anchors.centerIn: parent
                property bool notSelectedDisabled: false

                function pickAnswerColor(answerState, buttonPressedDown) {
                    console.debug("### button state: " + answerState)
                    var buttonColor
                    if (answerState === "WAIT_SELECTION")
                        buttonColor = self.answerNormalBackgroundColor
                    else if (answerState === "CORRECT")
                        buttonColor = self.answerCorrectBackgroundColor
                    else if (answerState === "NOT_CORRECT")
                        buttonColor = self.answerWrongBackgroundColor
                    else if (answerState === "MY_CORRECT")
                        buttonColor = self.answerMyCorrectBackgroundColor
                    else {
                        console.warn("Unknown state: " + answerState)
                        buttonColor = "white"
                    }

                    //return buttonColor
                    return buttonPressedDown ? Qt.darker(buttonColor) : buttonColor
                }

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
                    property bool buttonPressedDown: false
                    enabled: state === "WAIT_SELECTION" && !answerButtonBox.notSelectedDisabled

                    state: "WAIT_SELECTION"
                    onStateChanged: pickColor()
                    function pickColor() { color = answerButtonBox.pickAnswerColor(state, buttonPressedDown) }

                    Text {
                        id: answerIdLabel
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.buttonPressedDown = true
                            parent.color = Qt.darker(parent.color)
                            button1PressFeedbackTimer.running = true
                            buttonPressed(parent.answerId)
                        }
                    }

                    Timer {
                        id: button1PressFeedbackTimer
                        interval: answerArea.buttonPressFeedbackTime; running: false; repeat: false
                        onTriggered: {
                            parent.buttonPressedDown = false
                            parent.pickColor()
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

                    enabled: state === "WAIT_SELECTION" && !answerButtonBox.notSelectedDisabled
                    property bool buttonPressedDown: false
                    state: "WAIT_SELECTION"
                    onStateChanged: pickColor()
                    function pickColor() { color = answerButtonBox.pickAnswerColor(state, buttonPressedDown) }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.buttonPressedDown = true
                            parent.color = Qt.darker(parent.color)
                            button2PressFeedbackTimer.running = true
                            buttonPressed(parent.answerId)
                        }
                    }

                    Timer {
                        id: button2PressFeedbackTimer
                        interval: answerArea.buttonPressFeedbackTime; running: false; repeat: false
                        onTriggered: {
                            parent.buttonPressedDown = false
                            parent.pickColor()
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

                    enabled: state === "WAIT_SELECTION" && !answerButtonBox.notSelectedDisabled
                    property bool buttonPressedDown: false
                    state: "WAIT_SELECTION"
                    onStateChanged: pickColor()
                    function pickColor() { color = answerButtonBox.pickAnswerColor(state, buttonPressedDown) }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.buttonPressedDown = true
                            parent.color = Qt.darker(parent.color)
                            button3PressFeedbackTimer.running = true
                            buttonPressed(parent.answerId)
                        }
                    }

                    Timer {
                        id: button3PressFeedbackTimer
                        interval: answerArea.buttonPressFeedbackTime; running: false; repeat: false
                        onTriggered: {
                            parent.buttonPressedDown = false
                            parent.pickColor()
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

                    enabled: state === "WAIT_SELECTION" && !answerButtonBox.notSelectedDisabled
                    property bool buttonPressedDown: false
                    state: "WAIT_SELECTION"
                    onStateChanged: pickColor()
                    function pickColor() { color = answerButtonBox.pickAnswerColor(state, buttonPressedDown) }

                    Text {
                        anchors.centerIn: parent
                        text: parent.answerLabel
                        font.pixelSize: self.buttonTextFontPixelSize
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.buttonPressedDown = true
                            parent.color = Qt.darker(parent.color)
                            button4PressFeedbackTimer.running = true
                            buttonPressed(parent.answerId)
                        }
                    }

                    Timer {
                        id: button4PressFeedbackTimer
                        interval: answerArea.buttonPressFeedbackTime; running: false; repeat: false
                        onTriggered: {
                            parent.buttonPressedDown = false
                            parent.pickColor()
                        }
                    }
                }
            }
        }

        Item {
            id: pointsArea
            Layout.fillWidth: true
            Layout.preferredHeight: pointsLabel.implicitHeight * 1.5 * scaler

            property int defaultHeight: self.pointsTextSize * 1.5

            Text {
                id: pointsLabel
                anchors.centerIn: parent
                text: qsTr("Points") + " " + self.points
                font.pointSize: self.pointsTextSize
            }
        }
    }

    state: "WAIT"

    states: [
        State {
            name: "WAIT"
            PropertyChanges { target: pointsLabel; visible: false }
            PropertyChanges { target: answerButtonBox; opacity: 0.5 }
            PropertyChanges { target: answerButtonBox; notSelectedDisabled: true }
            StateChangeScript { script: feedback.setWaitMode() }
        },
        State {
            name: "ACCEPT_ANSWERS"
            PropertyChanges { target: pointsLabel; visible: true }
            PropertyChanges { target: answerButtonBox; opacity: 1 }
            PropertyChanges { target: answerButtonBox; notSelectedDisabled: false }
            StateChangeScript { script: feedback.setNowMode() }
        },
        State {
            name: "WAIT_ANSWER_FEEDBACK"
            //PropertyChanges { target: pointsLabel; visible: true }
            PropertyChanges { target: answerButtonBox; notSelectedDisabled: true }
            PropertyChanges { target: answerButtonBox; opacity: 0.8 }
            //StateChangeScript { script: feedback.setNowMode() }
        },
        State {
            name: "ACCEPT_ANSWERS_AFTER_FEEDBACK"
            PropertyChanges { target: pointsLabel; visible: true }
            PropertyChanges { target: answerButtonBox; opacity: 1 }
            PropertyChanges { target: answerButtonBox; notSelectedDisabled: false }
            //StateChangeScript { script: feedback.setNowMode() }
        },
        State {
            name: "SHOW_ANSWERS"
            //PropertyChanges { target: pointsLabel; visible: true }
            PropertyChanges { target: answerButtonBox; notSelectedDisabled: true }
            PropertyChanges { target: answerButtonBox; opacity: 1 }
            StateChangeScript { script: feedback.setNormalMode() }
        }

    ]


    // -- API --

    // Outgoing AppBoxMessages
    //   data: json object
    signal outgoingMessage(var data)

    property bool enabled: false
    property int questionId: 0

    // Incoming AppBoxMessages
    //   js: json object
    function incomingMessage(js) {
        console.debug("### MAINUI APPBOX MSG: " + js.toString())

        //outgoingMessage("got appbox msg")

        if (!"action" in js) {
            console.error("Unknown message format: " + js)
            return
        }

        if (js["action"] === "WaitGameToStart") {
            state = "WAIT"
            self.enabled = false
            buttons.map(function (b) {
                b.state = "WAIT_SELECTION"
            })

            self.points = 0

        } else if (js["action"] === "AcceptAnswers") {
            state = "ACCEPT_ANSWERS"
            self.enabled = true

            var enableButtons = js["enable"]
            buttons.map(function (b) {
                b.visible = false
                b.state = "WAIT_SELECTION"
            })

            enableButtons.map(function (answerId) {
                buttonsByAnswerId[answerId].visible = true
            })

            questionId = js["question_id"]
        }

        /// TODO
        /*
        if (js["action"] === "DisableControls") {
            // do not fade this text
            feedback.setWaitMode()
            buttonOpacity = 0.5
            self.enabled = false

        } else if (js["action"] === "EnableControls") {
            self.enabled = true
            feedback.setNowMode()
            buttonOpacity = 1.0
        */

        else if (js["action"] === "CorrectAnswerFeedback") {
            // TODO: how localization of these would go? ... game provides, but client selects?
            feedbackText.setFeedback(qsTr("Correct!"))
            buttonsByAnswerId[js["answer_id"]].state = "MY_CORRECT"
            self.points = js["points"]
            self.enabled = false
            state = "ACCEPT_ANSWERS_AFTER_FEEDBACK"

        } else if (js["action"] === "WrongAnswerFeedback") {
            feedbackText.setFeedback(qsTr("Wrong! Wait ..."))
            buttonsByAnswerId[js["answer_id"]].state = "NOT_CORRECT"
            accepAnswerAfterFeedbackTimer.running = true
            //state = "ACCEPT_ANSWERS_AFTER_FEEDBACK"

        } else if (js["action"] === "ShowAnswers") {
            if (js["status"] === "GameOver") {
                feedbackText.setFeedback(qsTr("Game over"))
            } else {
                feedbackText.setFeedback(qsTr("Answers"))
            }
            buttons.map(function (button) { button.state = "NOT_CORRECT" })
            buttonsByAnswerId[js["correct_answer"]].state = "CORRECT"
            self.enabled = false
            state = "SHOW_ANSWERS"


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
            state = "WAIT_ANSWER_FEEDBACK"
            var js = {"action": "AnswerSelected",
                      "answer": answerId,
                      "question_id": questionId}
            console.debug("Sending message out from AppBox")
            outgoingMessage(js)
        }
    }

    Timer {
        id: accepAnswerAfterFeedbackTimer
        running: false; repeat: false; interval: 1000
        onTriggered: {
            if (state === "WAIT_ANSWER_FEEDBACK") {
                state = "ACCEPT_ANSWERS_AFTER_FEEDBACK"
                feedbackText.setFeedback(qsTr("Try again!"))
            }
        }
    }

    property var buttons: [button1, button2, button3, button4]
    property var buttonsByAnswerId

    Component.onCompleted: {
        buttonsByAnswerId = {}
        buttons.forEach(function (button) {
            console.debug("MAPPING BUTTON: " + button.answerId)
            buttonsByAnswerId[button.answerId] = button
        })
        print("### buttons map: " + buttonsByAnswerId.toString())
    }
}

// TODO: try to use components defined in a pkg

