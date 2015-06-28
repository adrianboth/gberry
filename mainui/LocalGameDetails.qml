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
                font.pixelSize: gdisplay.largeSizeText

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

