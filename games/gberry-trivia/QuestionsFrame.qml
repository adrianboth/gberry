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

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: gdisplay.touchCellHeight()
        color: "green"


        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                id: titleRow

                Layout.preferredHeight: nameLabel.implicitHeight
                Layout.fillWidth: true
                color: "yellow"

                Text {
                    anchors.centerIn: parent
                    id: nameLabel

                    text: qsTr("Questions")
                    font.pixelSize: 55 //gdisplay.largeSizeText
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    text: "2/10"
                    font.pixelSize: gdisplay.smallSizeText
                }

            }

            QuestionArea {
                id: questionArea
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            AnswersArea {
                id: answersArea
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

}
