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

    property string gameName: qsTr("No games found")
    property string gameDescription: ""
    property string gameImageUrl: ""

    function readValues(gameMeta) {
        // these values come directly from the model
        gameName = gameMeta.name
        gameDescription = gameMeta.description

        if (typeof(gameMeta.catalog_image) !== "undefined") {
            console.debug("LocalGame: catalog_image: " + gameMeta.catalog_image)
            gameImageUrl = gameMeta.catalog_image
        } else {
            console.debug("LocalGame: no catalog_image defined")
        }

        freeTypeLabel.text = gameMeta.is_free ? qsTr("Free") : qsTr("Purchased")
        versionLabel.text = gameMeta.version
    }

    signal gameLauchRequested()

    onGameNameChanged: { launchButton.enabled = true }

    function processControlAction(action) {
        if (action === "OK") {
            launchButton.triggerButtonClick()
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
                    visible: Assets.isValidFilePath(gameImageUrl)
                    anchors.centerIn: parent
                    source: Assets.isValidFilePath(gameImageUrl) ? Assets.filePath(gameImageUrl) : ""
                    fillMode: Image.PreserveAspectFit
                    width: 100
                    height: 80
                }
            }

            Text {
                anchors.centerIn: parent
                id: nameLabel

                text: gameName
                font.pixelSize: 55 //gdisplay.largeSizeText
            }

            Text {
                id: freeTypeLabel
                text: "undefined"
                visible: text != "undefined"
                font.pixelSize: gdisplay.smallSizeText
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 10 // TODO: do not hardcode!
            }

            Text {
                id: versionLabel
                text: "undefined"
                visible: text != "undefined"
                font.pixelSize: gdisplay.smallSizeText
                anchors.top: freeTypeLabel.bottom
                anchors.right: parent.right
                anchors.margins: 10 // TODO: do not hardcode!
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
            Layout.preferredHeight: launchButton.buttonHeight + gdisplay.touchCellHeight()

            GButton {
                id: launchButton
                anchors.centerIn: parent
                width: buttonWidth
                height: buttonHeight
                enabled: false // initial until data comes in

                label: qsTr("Launch")

                onButtonClicked: gameLauchRequested()
            }
        }
    }
}

