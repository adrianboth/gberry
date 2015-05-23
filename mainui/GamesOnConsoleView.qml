import QtQuick 2.2
import QtQuick.Layouts 1.1
import GBerry 1.0

// GamesOnConsole lists local games and enables
//  - browsing games (image, description)
//  - launching game

Item {
    id: self

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
                            font.pixelSize: gdisplay.largeSizeText
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
                        localGameDetails.gameName = selectedGame.name
                        localGameDetails.gameDescription = selectedGame.description
                    }

                    Component {
                        id: contactsDelegate
                        Rectangle {
                            id: wrapper
                            width: listViewBackground.realWidth
                            height: contactInfo.implicitHeight + gdisplay.touchCellHeight() // +margins
                            color: ListView.isCurrentItem ? "black" : "red"

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.debug("Clicked " + index)
                                    gameList.currentIndex = index
                                }
                            }

                            Row {
                                anchors.verticalCenter: parent.verticalCenter

                                Rectangle  { // fake image
                                    width: gdisplay.mediumSizeText
                                    height: gdisplay.mediumSizeText
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "blue"
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
                                    color: wrapper.ListView.isCurrentItem ? "red" : "black"
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
                ListElement {
                    name: "TicTacToe"
                    image: "foobar"
                    description: "TicTacToe description very long lipsul oipsum. Lopsam lopsi alpsi liie."
                }
                ListElement {
                    name: "React"
                    image: "foobar"
                    description: "React description"
                }
                ListElement {
                    name: "TestGame"
                    image: "foobar"
                    description: "TestGame description"
                }

                Component.onCompleted: {
                    localGamesModel.clear()
                    var gameIds = GameModel.localGameIds()
                    for (var i = 0; i < gameIds.length; i++) {
                        localGamesModel.append(GameModel.game(gameIds[i]))
                    }

                    gameList.updateDetails()
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
                        launchRequested(localGamesModel.get(gameList.currentIndex).name)
                    }
                }
            }
        }

    }


}
/*

*/
