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
    property int showAnswer: 0
    property int preferredHeight: answerIdLabel.implicitHeight * 2

    visible: answerText != ""

    signal answerClicked()

    onShowAnswerChanged: {
        if (showAnswer !== 0) {
            answerSelected(showAnswer === 1)
        } else {
            state = "WAIT_SELECTION"
        }
    }

    function answerSelected(isCorrectAnswer) {
        if (isCorrectAnswer) {
            state = "CORRECT"
        } else {
            state = "NOT_CORRECT"
        }
    }

    // TODO: how to get information about correct answer
    // TODO: how long to show feed back info
    Rectangle {
        id: answerTextBackground
        anchors.fill: parent
        anchors.leftMargin: gdisplay.touchCellWidth()
        anchors.rightMargin: gdisplay.touchCellWidth()
        color: theme.answerNormalBackgroundColor
        radius: 10

        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right

            Item {
                // spacer
                Layout.fillHeight: true
                Layout.preferredWidth: gdisplay.touchCellWidth() / 5
            }

            Item {
                Layout.preferredHeight: answerIdLabel.implicitHeight
                Layout.preferredWidth: answerIdLabel.implicitWidth

                Text {
                    id: answerIdLabel
                    text: answerId + ")"
                    font.pixelSize: gdisplay.mediumSizeText
                }
            }

            Item {
                // spacer
                Layout.fillHeight: true
                Layout.preferredWidth: gdisplay.touchCellWidth() / 5
            }

            Item {
                Layout.preferredHeight: answerTextLabel.implicitHeight
                Layout.fillWidth: true

                Text {
                    id: answerTextLabel
                    text: answerText
                    font.pixelSize: gdisplay.mediumSizeText
                }
            }

            Image {
                id: correctWrongImage

                //visible: true
                //source: "images/wrong.svg"
                visible: self.state !== "WAIT_SELECTION"
                source: self.state === "CORRECT" ? "images/correct.svg" : "images/wrong.svg"

                sourceSize.height: answerTextLabel.implicitHeight * 1.5
                fillMode: Image.PreserveAspectFit
            }

            Item {
                // spacer
                Layout.fillHeight: true
                Layout.preferredWidth: gdisplay.touchCellWidth() / 5
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.debug("Answer " + answerId + " clicked")
            answerClicked()
        }
    }

    state: "WAIT_SELECTION"

    states: [
        State {
            name: "WAIT_SELECTION"
            PropertyChanges { target: answerTextBackground; color: theme.answerNormalBackgroundColor }
        },
        State {
            name: "CORRECT"
            PropertyChanges { target: answerTextBackground; color: theme.answerCorrectBackgroundColor }
        },
        State {
            name: "NOT_CORRECT"
            PropertyChanges { target: answerTextBackground; color: theme.answerWrongBackgroundColor }
        }
    ]
}
