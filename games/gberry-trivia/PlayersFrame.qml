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

import GBerry 1.0

Item {
    id: self

    Rectangle {
        id: listViewBackground
        anchors.fill: parent
        /*
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins:
        */
        //width: parent.width - 5
        anchors.margins: gdisplay.touchCellHeight()
        //property int realWidth: width - anchors.margins

        color: "snow"
        border.width: 1
        border.color: "gray"

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                id: headerBackground
                Layout.fillWidth: true
                Layout.preferredHeight: headerText.implicitHeight
                color: "lightgray"
                //x: 1
                //height: headerText.implicitHeight
                //width: listViewBackground.realWidth

                Text {
                    anchors.centerIn: parent
                    id: headerText
                    text: qsTr("Players")
                    font.pixelSize: 55 //gdisplay.largeSizeText
                }
            }

            ListView {
                id: playersList
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component {
                    id: playerRowDelegate

                    Rectangle {
                        id: wrapper
                        width: listViewBackground.realWidth
                        height: playerNameText.implicitHeight + gdisplay.touchCellHeight() // +margins
                        color: ListView.isCurrentItem ? "orange" : "white"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.debug("Clicked " + index)
                                playersList.currentIndex = index
                            }
                        }

                        Row {
                            anchors.verticalCenter: parent.verticalCenter

                            // spacer
                            Item {
                                width: gdisplay.smallSizeText / 2
                                height: gdisplay.mediumSizeText
                            }

                            Text {
                                id: playerPointsText
                                anchors.verticalCenter: parent.verticalCenter
                                text: points
                                font.pixelSize: gdisplay.mediumSizeText
                            }

                            Item {
                                width: gdisplay.mediumSizeText
                                height: gdisplay.mediumSizeText
                            }

                            Text {
                                id: playerNameText
                                anchors.verticalCenter: parent.verticalCenter
                                text: name
                                color: wrapper.ListView.isCurrentItem ? "black" : "black" // TODO
                                font.pixelSize: gdisplay.mediumSizeText
                            }

                        }
                    }
                }

                model: playerPointsModel
                delegate: playerRowDelegate
                focus: true
            }
        }
    }

    ListModel {
        id: playerPointsModel

        // TODO: test data
        ListElement {
            name: "Tero"
            points: 2
        }

        ListElement {
            name: "Sanna"
            points: 1
        }

        Component.onCompleted: {

        }
    }
}
