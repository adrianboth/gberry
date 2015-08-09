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

    function updateScores(newData) {
        // find out changes
        var oldPointsByName = {}
        for (var i = 0; i < playerPointsModel.count; i++) {
            //print("### object: " + playerPointsModel.get(i).toString())
            //print("### typeof(): " + typeof(playerPointsModel.get(i)))
            oldPointsByName[playerPointsModel.get(i)["name"]] = playerPointsModel.get(i)["points"]
        }
        //console.debug("### oldMap: " + oldPointsByName.toString())

        playerPointsModel.clear()
        for (var j = 0; j < newData.length; j++) {
            var newPoints = newData[j]["points"]
            var oldPoints = oldPointsByName[newData[j]["name"]]
            //console.debug("### new: " + newPoints.toString() + ", old: " + oldPoints)
            var change = 0

            if (typeof(oldPoints) === "undefined") {
                change = 0
            } else if (newPoints > oldPoints) {
                change = 1
            }
            //console.debug("### change: " + change.toString())
            newData[j]["change"] = change
            playerPointsModel.append(newData[j])
        }
    }

    Rectangle {
        id: listViewBackground
        radius: 10

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

        color: theme.boxAreaBackgroundColor
        border.width: 1
        border.color: theme.boxAreaBorderColor

        ColumnLayout {
            anchors.fill: parent

            Item  {
                Layout.fillWidth: true
                Layout.preferredHeight: headerText.implicitHeight * 1.1

                Rectangle {
                    id: headerBackground
                    radius: 10
                    anchors.fill: parent
                    anchors.margins: 1

                    color: theme.headingColor
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
                            id: playerRow
                            anchors.verticalCenter: parent.verticalCenter

                            property int baseFontSize: gdisplay.mediumSizeText
                            property real fontScaling: 1.0

                            // spacer
                            Item {
                                width: gdisplay.smallSizeText / 2
                                height: gdisplay.mediumSizeText
                            }

                            Text {
                                id: playerPointsText
                                anchors.verticalCenter: parent.verticalCenter
                                text: points
                                font.pixelSize: gdisplay.mediumSizeText * playerRow.fontScaling
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
                                font.pixelSize: gdisplay.mediumSizeText * playerRow.fontScaling
                            }

                            SequentialAnimation {
                                id: scoredAnimation
                                property bool runOnce: false
                                running: change === 1 && !runOnce // model is cleaned up everytime
                                NumberAnimation { target: playerRow; property: "fontScaling"; to: 1.5; duration: 500 }
                                NumberAnimation { target: playerRow; property: "fontScaling"; to: 1.0; duration: 500 }
                                PropertyAction { target: scoredAnimation; property: "runOnce"; value: true }
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
            change: 0
        }

        ListElement {
            name: "Sanna"
            points: 1
            change: 0
        }

        Component.onCompleted: {

        }
    }
}
