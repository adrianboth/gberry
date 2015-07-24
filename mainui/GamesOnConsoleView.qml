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
 
 import QtQuick 2.2
import QtQuick.Layouts 1.1
import GBerry 1.0

// GamesOnConsole lists local games and enables
//  - browsing games (image, description)
//  - launching game

Item {
    id: self
    focus: true

    signal backSelected()
    signal launchRequested(string gameId)

    readonly property var firstItemSelectedControlActions: ["Down", "OK", "Back"]
    readonly property var defaultControlActions: ["Up", "Down", "OK", "Back"]
    readonly property var lastItemSelectedControlActions: ["Up", "OK", "Back"]

    property var enabledControlActions: defaultControlActions // defaults


    function processControlAction(action) {
        if (action === "Up")
            gameList.decrementCurrentIndex()
        else if (action === "Down")
            gameList.incrementCurrentIndex()
        else if (action === "Back")
            backButton.triggerButtonClick()
        else
            localGameDetails.processControlAction(action)

    }

    ColumnLayout {
        anchors.fill: parent

        Item {
            id: navigationBar

            Layout.preferredHeight: backButton.height + gdisplay.touchCellHeight()
            Layout.fillWidth: true

            GButton {
                id: backButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: gdisplay.touchCellWidth()

                width: buttonWidth
                height: buttonHeight

                label: qsTr("Back")

                onButtonClicked: backSelected()

            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: contentArea

            Rectangle {
                id: listViewBackground
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 1
                width: parent.width * 0.33

                property int realWidth: width - anchors.margins

                color: "snow"
                border.width: 1
                border.color: "gray"

                ListView {
                    id: gameList

                    anchors.fill: parent

                    header:  Rectangle {
                        color: "lightgray"
                        x: 1
                        height: headerText.implicitHeight
                        width: listViewBackground.realWidth

                        Text {
                            anchors.centerIn: parent
                            id: headerText
                            text: qsTr("Games")
                            font.pixelSize: 55 //gdisplay.largeSizeText
                        }
                    }

                    onCurrentIndexChanged: {
                        updateDetails()
                        if (currentIndex === 0)
                            self.enabledControlActions = firstItemSelectedControlActions
                        else if (currentIndex === localGamesModel.count - 1)
                            self.enabledControlActions = lastItemSelectedControlActions
                        else
                            self.enabledControlActions = defaultControlActions
                    }

                    function updateDetails() {
                        var selectedGame = localGamesModel.get(currentIndex)
                        if (typeof(selectedGame) !== 'undefined') {
                            localGameDetails.gameName = selectedGame.name
                            localGameDetails.gameDescription = selectedGame.description
                        } else {
                            localGameDetails.gameName = "undefined"
                            localGameDetails.gameDescription = "undefined description"
                        }

                    }

                    Component {
                        id: contactsDelegate
                        Rectangle {
                            id: wrapper
                            width: listViewBackground.realWidth
                            height: contactInfo.implicitHeight + gdisplay.touchCellHeight() // +margins
                            color: ListView.isCurrentItem ? "orange" : "white"

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.debug("Clicked " + index)
                                    gameList.currentIndex = index
                                }
                            }

                            Row {
                                anchors.verticalCenter: parent.verticalCenter

                                // spacer
                                Item {
                                    width: gdisplay.smallSizeText / 2
                                    height: gdisplay.mediumSizeText
                                }

                                Rectangle  { // fake image
                                    width: gdisplay.smallSizeText
                                    height: gdisplay.smallSizeText
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "lightblue" // TODO: some common place for colors
                                    //source: "file"
                                }

                                Item {
                                    width: gdisplay.touchCellWidth() / 2
                                    height: gdisplay.mediumSizeText
                                }

                                Text {
                                    id: contactInfo
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: name
                                    color: wrapper.ListView.isCurrentItem ? "black" : "black" // TODO
                                    font.pixelSize: gdisplay.mediumSizeText
                                }
                            }
                        }
                    }

                    model: localGamesModel
                    delegate: contactsDelegate
                    focus: true
                }
            }

            ListModel {
                id: localGamesModel
                property bool modelPopulated: false

                // model: id, name, description

                function onLocalGamesAvailable() {
                    console.debug("### onLocalGamesAvailable()")
                    localGamesModel.clear()

                    var gameIds = GameModel.localGameIds()
                    for (var i = 0; i < gameIds.length; i++) {
                        localGamesModel.append(GameModel.game(gameIds[i]))
                    }

                    gameList.updateDetails()
                }

                Component.onCompleted: {
                    console.debug("### game model onCompleted()")
                    GameModel.localGamesAvailable.connect(onLocalGamesAvailable)
                    GameModel.localGamesUpdated.connect(onLocalGamesAvailable)

                    var available = GameModel.requestLocalGames()
                    if (available)
                        onLocalGamesAvailable()
                }
            }

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: gdisplay.touchCellWidth()
                anchors.bottomMargin: gdisplay.touchCellHeight()
                border.width: 1
                border.color: "gray"
                color: "snow"

                width: contentArea.width - listViewBackground.width * 1.1

                LocalGameDetails {
                    id: localGameDetails
                    anchors.fill: parent

                    onGameLauchRequested: {
                        console.debug("GAME LAUNCH!")
                        launchRequested(localGamesModel.get(gameList.currentIndex).id)
                    }
                }
            }
        }

    }

    Keys.onPressed: {
        console.debug("######## KEY PRESSED")
        if (event.key === Qt.Key_Up)
            processControlAction("Up")
        if (event.key === Qt.Key_Down)
            processControlAction("Down")
        if (event.key === Qt.Key_Return)
            processControlAction("OK")
        if (event.key === Qt.Key_Escape)
            processControlAction("Back")
    }
}
/*

*/
