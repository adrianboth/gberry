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

Item {
    id: self

    // TODO: why were not taking just ID and reading everything else from model
    property string gameFullId: ""
    property string gameApplicationId: ""
    property string gameName: qsTr("No games found")
    property string gameDescription: ""
    property string gameImageUrl: ""

    function readValues(gameMeta) {
        gameFullId = gameMeta.id
        gameApplicationId = gameMeta.application_id
        gameName = gameMeta.name
        gameDescription = gameMeta.description
        gameImageUrl = gameMeta.catalog_image
        console.debug("DEBUG: gameImageUrl = " + gameImageUrl)
        freeTypeLabel.text = gameMeta.is_free ? qsTr("Free") : qsTr("Commercial")
        versionLabel.text = gameMeta.version
    }

    signal gameDownloadRequested()
    signal gameLaunchRequested()

    onGameNameChanged: {
        /*
        // TODO: HACK!!! making to lower is evil hack (temporary, really!) because downloadable apps don't have really good unique id, instead it is derived from name
        var gameDetailsMap = GameModel.game(gameFullId)
        console.debug("### onGameNameChanged() checking " + gameFullId)
        console.debug("### gameDetailsMap " + gameDetailsMap.length)
        console.debug("### check: " + typeof(gameDetailsMap.id))

        var gameIds = GameModel.localGameIds()
        for (var i = 0; i < gameIds.length; i++) {
            console.debug("### local game id: " + GameModel.game(gameIds[i]).id)
            console.debug("### local game: " + GameModel.game(gameIds[i]).toString())
        }
        */
        checkItemStatus() // DOWNLOADING state

        // TODO: how to check if game is downloading
        //   -> if download has been started then temp appcfg should be on the disk (but has it updated to us)

    }

    function processControlAction(action) {
        if (action === "OK") {
            // TODO: we could check button enabled instead
            if (state === "DOWNLOADABLE" || state === "UPGRADEABLE") {
                downloadButton.triggerButtonClick()
            } else if (state === "DOWNLOADED" || state === "JUST_DOWNLOADED") {
                launchButton.triggerButtonClick()
            }
                //launchRequested(localGamesModel.get(gameList.currentIndex).name)
        }
    }


    ColumnLayout {
        anchors.fill: parent

        Item {
            id: titleRow

            Layout.preferredHeight: nameLabel.implicitHeight + gdisplay.touchCellHeight()
            Layout.fillWidth: true

            Item {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: nameLabel.left
                Image {
                    id: gameImage
                    visible: status === Image.Ready
                    anchors.centerIn: parent
                    source: gameImageUrl
                    fillMode: Image.PreserveAspectCrop
                    width: 100
                    height: 80
                }
            }

            Text {
                anchors.centerIn: parent
                id: nameLabel

                text: gameName
                font.pixelSize: 55 //gdisplay.mediumSizeText
            }

            Text {
                id: freeTypeLabel
                text: "undefined"
                visible: text != "undefined"
                font.pixelSize: gdisplay.smallSizeText
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 10 // TODO: do not hardcode
            }

            Text {
                id: versionLabel
                text: "undefined"
                visible: text != "undefined"
                font.pixelSize: gdisplay.smallSizeText
                anchors.top: freeTypeLabel.bottom
                anchors.right: parent.right
                anchors.margins: 10 // TODO: do not hardcode
            }
        }

        Item {
            id: statusRow

            Layout.preferredHeight: statusLabel.implicitHeight + gdisplay.touchCellHeight()
            Layout.fillWidth: true

            Text {
                anchors.centerIn: parent
                id: statusLabel

                // TODO: here comes download status
                // just example text here
                text: "will be replaced"
                color: "blue"
                font.pixelSize: gdisplay.mediumSizeText
            }
        }

        Item {
            id: descriptionArea
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: descriptionText
                anchors.fill: parent
                anchors.margins: gdisplay.touchCellHeight()
                font.pixelSize: gdisplay.mediumSizeText
                text: gameDescription
                wrapMode: Text.WordWrap
                // without specific width word wrapping doesn't work
                width: descriptionArea.width - 2*gdisplay.touchCellHeight()
            }
        }

        Item {
            id: buttonRow

            Layout.fillWidth: true
            Layout.preferredHeight: downloadButton.buttonHeight + gdisplay.touchCellHeight()

            GButton {
                id: downloadButton
                anchors.centerIn: parent
                width: buttonWidth
                height: buttonHeight
                enabled: gameFullId != "" // initial until data comes in

                label: self.state === "UPGRADEABLE" ? qsTr("Update") : qsTr("Download")

                onButtonClicked: {
                    gameDownloadRequested()
                }
            }

            // this button will occupy same location as above button, only one of them
            // will be visible at the time
            GButton {
                id: launchButton
                anchors.centerIn: parent
                width: buttonWidth
                height: buttonHeight
                enabled: true
                visible: false

                label: qsTr("Launch")

                onButtonClicked: gameLaunchRequested()
            }
        }
    }

    states: [
            State {
                name: "DOWNLOADABLE"
                PropertyChanges { target: downloadButton; visible: true }
                PropertyChanges { target: launchButton; visible: false }
                PropertyChanges { target: statusRow; visible: false }
            },
            State {
                name: "UPGRADEABLE"
                PropertyChanges { target: downloadButton; visible: true }
                PropertyChanges { target: launchButton; visible: false }
                PropertyChanges { target: statusRow; visible: true }
                PropertyChanges { target: statusLabel; text: qsTr("Earlier version is already installed."); }
            },
            State {
                name: "DOWNLOADED"
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: launchButton; visible: true }
                PropertyChanges { target: statusRow; visible: true; }
                PropertyChanges { target: statusLabel; text: qsTr("This game is already installed."); }
            },
            State {
                name: "JUST_DOWNLOADED"
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: launchButton; visible: true }
                PropertyChanges { target: statusRow; visible: true; }
                PropertyChanges { target: statusLabel; text: qsTr("Download finished."); }
            },
            State {
                name: "DOWNLOADING"
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: launchButton; visible: false }
                PropertyChanges { target: statusRow; visible: true; }
                PropertyChanges { target: statusLabel; text: qsTr("Downloading ..."); }
                // TODO: some how connect download status
            }
        ]

    // just some (not so good state for initial setup, case if no games at all)
    state: "DOWNLOADABLE"

    function checkItemStatus() {
        console.debug("### checking ongoing downloads")

        // list of application ids
        var ongoingDownloads = DownloadModel.ongoingDownloads()
        for (var i = 0; i < ongoingDownloads.length; i++) {
            if (ongoingDownloads[i] === gameFullId) {
                state = "DOWNLOADING"
                return
            }
        }

        // didn't find from downloads -> check if local

        var gameDetailsMap = GameModel.game(gameFullId)
        if (typeof(gameDetailsMap.id) !== 'undefined') {
            state = "DOWNLOADED"
            return
        }

        console.debug("No installed, but is there earlier versions: application_id=" + gameApplicationId)
        var earlierVersionId = GameModel.newestGameByApplicationId(gameApplicationId)

        // something found but that is not same as what we are checking
        if (earlierVersionId.length > 0 && earlierVersionId !== gameFullId) {
            console.debug("Compared: " + earlierVersionId + " vs " + gameFullId)
            state = "UPGRADEABLE"
            return
        }

        // no local game
        state = "DOWNLOADABLE"
    }

    // this is needed to avoid jumping between states with checkItemStatus()
    // as it takes (short) time to GameModel to update its data.
    function onDownloadFinished(gameFullId_) {
        if (gameFullId === gameFullId_) {
            state = "JUST_DOWNLOADED"
        }
    }

    Component.onCompleted: {
        // we are not restricting to any specific download
        //   -> whenever something happens -> check status
        DownloadModel.downloadInitiated.connect(checkItemStatus)
        DownloadModel.downloadAborted.connect(checkItemStatus)
        DownloadModel.downloadFinished.connect(onDownloadFinished)
    }
}
