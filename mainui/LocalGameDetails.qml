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

            Text {
                anchors.centerIn: parent
                id: nameLabel

                text: gameName
                font.pixelSize: 55 //gdisplay.largeSizeText
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

