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

Item {
    id: self
    property string answerId: "A"
    property string answerText: "This is answer demo"
    property int preferredHeight: answerIdLabel.implicitHeight

    signal answerClicked()

    onAnswerTextChanged: state = "WAIT_SELECTION"

    function answerSelected(isCorrectAnswer) {
        if (isCorrectAnswer) {
            state = "CORRECT"
        } else {
            state = "NOT_CORRECT"
        }
    }

    // TODO: how to get information about correct answer
    // TODO: how long to show feed back info

    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.preferredHeight: answerIdLabel.implicitHeight
            Layout.preferredWidth: answerIdLabel.implicitWidth
            color: "blue"

            Text {
                id: answerIdLabel
                text: answerId + ")"
                font.pixelSize: gdisplay.mediumSizeText
            }
        }

        Rectangle {
            id: answerTextBackground

            Layout.preferredHeight: answerTextLabel.implicitHeight
            Layout.fillWidth: true
            color: "lightblue"

            Text {
                id: answerTextLabel
                text: answerText
                font.pixelSize: gdisplay.mediumSizeText
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.debug("Answer " + answerId + " clicked")
        }
    }

    state: "WAIT_SELECTION"

    states: [
        State {
            name: "WAIT_SELECTION"
            PropertyChanges { target: answerTextBackground; color: "lightblue" }
        },
        State {
            name: "CORRECT"
            PropertyChanges { target: answerTextBackground; color: "blue" }
        },
        State {
            name: "NOT_CORRECT"
            PropertyChanges { target: answerTextBackground; color: "red" }
        }
    ]
}

