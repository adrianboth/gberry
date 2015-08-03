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

// DownloadableGamesView lists games that can be downloaded
//  - browsing games (image, description)
//  - downloading game

Item {
    id: self
    focus: true

    signal backSelected()
    signal launchRequested(string gameId)
    signal downloadRequested(string gameId)
    signal errorOccurred(string errorMsg)

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
            gameDetails.processControlAction(action)

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

            Text {
                id: guestLoginShowingOnlyFreeAppsText
                text: ActivePlayerModel.activePlayerIsGuest ? qsTr("For guests only free games are shown") : qsTr("Player") + " " + playersManager.playerName(ActivePlayerModel.activePlayerId)
                font.pixelSize: gdisplay.mediumSizeText
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: gdisplay.touchCellWidth()
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: contentArea

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: gdisplay.touchCellWidth()
                anchors.bottomMargin: gdisplay.touchCellHeight()
                border.width: 1
                border.color: "gray"
                color: "snow"

                width: contentArea.width - listViewBackground.width * 1.1

                DownloadableGameDetails {
                    id: gameDetails
                    anchors.fill: parent

                    onGameDownloadRequested: {
                        console.debug("GAME DOWNLOAD!")
                        // TODO: what happens when downloading initiated
                        //  ?? if game is already installed, then say so and provide possibility to launch

                        //  first and later
                        //    a) details will show download progress
                        //       - not possible to download again
                        //

                        // when download finished
                        //    LATER: we could have a feeback box that download finished
                        //    some kind of trigger so that if details open -> updates


                        downloadRequested(gamesUiModel.get(gameList.currentIndex).id)

                        // TODO: somehow update details
                        //     -> check download ok stuff
                    }

                    onGameLaunchRequested: {
                        console.debug("GAME LAUNCH!")
                        // we trust that game is found locally
                        launchRequested(gamesUiModel.get(gameList.currentIndex).id)
                    }
                }
            }

            Rectangle {
                id: listViewBackground
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
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
                            text: qsTr("Webstore")
                            font.pixelSize: 55 //gdisplay.largeSizeText
                        }
                    }

                    onCurrentIndexChanged: {
                        updateDetails()
                        if (currentIndex === 0)
                            self.enabledControlActions = self.firstItemSelectedControlActions
                        else if (currentIndex === gamesUiModel.count - 1)
                            self.enabledControlActions = self.lastItemSelectedControlActions
                        else
                            self.enabledControlActions = self.defaultControlActions
                    }

                    function updateDetails() {
                        var selectedGame = gamesUiModel.get(currentIndex)
                        if (typeof(selectedGame) !== 'undefined') {
                            gameDetails.readValues(selectedGame)
                        } else {
                            // TODO: more reseting to gameDetails
                            gameDetails.gameFullId = ""
                            gameDetails.gameName = "undefined"
                            gameDetails.gameDescription = "undefined description"
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
                                    color: "lightblue"
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
                                    color: wrapper.ListView.isCurrentItem ? "black" : "black"
                                    font.pixelSize: gdisplay.mediumSizeText
                                }
                            }
                        }
                    }

                    model: gamesUiModel
                    delegate: contactsDelegate
                    focus: true
                }
            }

            ListModel {
                id: gamesUiModel
                property bool modelPopulated: false

                // model: id, name, description

                /*
                function onLocalGamesAvailable() {
                    console.debug("### onLocalGamesAvailable()")

                    var gameIds = GameModel.localGameIds()
                    for (var i = 0; i < gameIds.length; i++) {
                        localGamesModel.append(GameModel.game(gameIds[i]))
                    }

                    gameList.updateDetails()
                }
                */

                function onGamesAvailable() {
                    console.debug("### onGamesAvailable()")
                    // either got initial set of games of there is an update but read all
                    gamesUiModel.clear()

                    // TODO: we could use map()
                    var gameIds = DownloadableGamesModel.gameIds()
                    for (var i = 0; i < gameIds.length; i++) {
                        gamesUiModel.append(DownloadableGamesModel.game(gameIds[i]))
                    }

                    gameList.updateDetails()
                }

                Component.onCompleted: {
                    console.debug("### game model onCompleted()")
                    /*
                    GameModel.localGamesAvailable.connect(onLocalGamesAvailable)
                    var available = GameModel.requestLocalGames()
                    if (available)
                        onLocalGamesAvailable()
                    */
                    DownloadableGamesModel.gamesAvailable.connect(onGamesAvailable)
                    DownloadableGamesModel.gamesUpdated.connect(onGamesAvailable)
                    //DownloadableGamesModel.requestGames();

                    // TODO: actually games should be freshed more often, like when showing this view: other wise requires restart of mainui
                    //       (in that we could make request() but if returns true, then update immediately to show what we have)
                }
            }

        }

    }

    onVisibleChanged: {
        if (visible) {
            // TODO: we good have some kind of waiting dialog

            // note that as each active player may see different games we need
            // to do fresh every time. But model can cache (internally) for
            // players what they see
            gamesUiModel.clear()
            if (DownloadableGamesModel.requestGames()) {
                gamesUiModel.onGamesAvailable()
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

    function onActivePlayerIdChanged() {
        // we have opened this view by activating certain player, if he leaves
        // then we need to close view.
        // (Main view have already shown feedback dialog)

        if (visible && !ActivePlayerModel.hasActivePlayer) {
            backSelected()
        }
    }

    function onGamesRequestFailed() {
        errorOccurred(qsTr("Failed to download game list"))
    }

    Component.onCompleted: {
        ActivePlayerModel.activePlayerIdChanged.connect(onActivePlayerIdChanged)
        DownloadableGamesModel.gamesRequestFailed.connect(onGamesRequestFailed)
    }
}
