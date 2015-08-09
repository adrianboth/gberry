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

    signal devAnswerClicked(var answerId)
    signal readyToAcceptAnswers()

    function showWait() {
        waitView.visible = true
        questionsView.visible = false
    }

    function start() {
        if (root.debugSkipCountDown) {
            waitView.visible = false
            questionsView.visible = true
            readyToAcceptAnswers()
        } else {
            countdown.start()
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: gdisplay.touchCellHeight()
        color: theme.boxAreaBackgroundColor
        radius: 10
        border.color: theme.boxAreaBorderColor
        border.width: 1


        Rectangle {
            id: titleRow

            height: nameLabel.implicitHeight * 1.1
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            color: theme.headingColor
            radius: 10

            Text {
                anchors.centerIn: parent
                id: nameLabel

                text: qsTr("Questions")
                font.pixelSize: 55 //gdisplay.largeSizeText
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.margins: gdisplay.touchCellWidth()
                text: currentQuestion.currentQuestionIndex + "/" + currentQuestion.maxQuestionsCount
                font.pixelSize: gdisplay.mediumSizeText
            }

        }

        Item {
            id: questionsContentArea
            anchors.top: titleRow.bottom
            anchors.left: parent.left; anchors.right: parent.right
            anchors.bottom: parent.bottom

            ColumnLayout {
                id: questionsView
                anchors.fill: parent

                QuestionArea {
                    id: questionArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                AnswersArea {
                    id: answersArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    onDevAnswerClicked: self.devAnswerClicked(answerId)
                }

                Item { // spacer
                    Layout.fillWidth: true
                    Layout.preferredHeight: gdisplay.touchCellHeight() / 2
                }
            }

            Item {
                id: waitView
                //color: "yellow"
                anchors.fill: parent

                CountDown {
                    anchors.centerIn: parent
                    id: countdown
                    //countFrom: root.debugSkipCountDown ? 0 : 3
                    onFinished: fadeAnimation.running = true // countdown will fade itself
                }
            }
        }
    }

    ParallelAnimation {
        id: fadeAnimation
        running: false

        SequentialAnimation {
            PropertyAction { target: questionsView; property: "opacity"; value: 0 }
            PropertyAction { target: questionsView; property: "visible"; value: true }
            NumberAnimation {
                target: questionsView
                property: "opacity"
                from: 0; to: 1; duration: 500
            }
            ScriptAction { script: self.readyToAcceptAnswers() }

        }
        SequentialAnimation{
            NumberAnimation {
                target: waitView
                property: "opacity"
                from: 1; to: 0; duration: 250
            }
            PropertyAction { target: waitView; property: "visible"; value: false }
            PropertyAction { target: waitView; property: "opacity"; value: 1 }
        }
    }
}
