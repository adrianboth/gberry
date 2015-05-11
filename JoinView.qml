import QtQuick 2.4
import QtQuick.Layouts 1.1

import "PlayerWaitingModel.js" as PlayerWaitingModel

Item {
    id: self

    signal secondPlayerJoinedAnimationFinished()

    function joinFirstPlayer(name) {

        firstPlayerBox.name = name
        firstPlayerBox.x = root.width / 2 - firstPlayerBox.preferredWidth / 2 // move to center
        // setting 'x' meant animation started -> trigger when finished
        firstPlayerBox.visible = true
    }

    function joinSecondPlayer(name) {

        secondPlayerBox.name = name
        secondPlayerBox.x = root.width / 2 - secondPlayerBox.preferredWidth / 2 // move to center
        // setting 'x' meant animation started -> trigger when finished
        secondPlayerBox.visible = true
    }

    function reset() {
        firstPlayerBox.reset()
        secondPlayerBox.reset()
    }

    // TODO: text Waiting players to join ...

    // TODO: boxes sliding from sides
    //       (names)
    //       once second on place -> start game
    //       (already before animation other players are disabled from joining)
    //       (joined players should have "joined!"

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter

        Item {
            //color: "red"
            Layout.preferredWidth: gameTitle.implicitWidth
            Layout.preferredHeight: gameTitle.implicitHeight
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: gameTitle
                text: "TicTacToe"
                font.pixelSize: gdisplay.text_mm(25)
                anchors.centerIn: parent
            }
        }
        // --

        // some room between title and other text
        Item {
            Layout.preferredWidth: waitingText.implicitWidth
            Layout.preferredHeight: waitingText.implicitHeight

        }

        // ---
        Item {
            Layout.preferredWidth: waitingText.implicitWidth
            Layout.preferredHeight: waitingText.implicitHeight
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: waitingText
                text: qsTr("Waiting players to join ...")
                font.pixelSize: gdisplay.text_mm(10)
            }
        }

        Item {
            Layout.preferredWidth: waitingText.implicitWidth
            Layout.preferredHeight: waitingText.implicitHeight / 2

        }

        // ---
        Item {
            id: joinedPlayersBox
            //color: "green" // debug

            Layout.preferredWidth: root.width
            Layout.preferredHeight: 200 // TODO: height doesn't really matter in this case ...

            // we are expecting two player
            //  -> we will show widgets once player has joined and we know a name

            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter

                Item {
                    id: track
                    Layout.preferredHeight: firstPlayerBox.preferredHeight
                    Layout.preferredWidth: self.width

                    NameBox {
                        id: firstPlayerBox
                        x: -width // initial position is hidded outside the screen
                        visible: false
                        height: preferredHeight
                        width: preferredWidth
                        //Layout.alignment: Qt.AlignHCenter

                        name: "test"
                        isCrossItem: true
                        //bgColor: "blue"

                        Behavior on x { SmoothedAnimation { velocity: 500 } }

                        function reset() {
                            // hide to prevent animation while going to initial position
                            visible = false
                            name = "<undefined>"
                            x = -width
                        }
                    }
                }

                Item {
                    id: track2
                    Layout.preferredHeight: secondPlayerBox.preferredHeight
                    Layout.preferredWidth: self.width

                    NameBox {
                        id: secondPlayerBox
                        x: root.width // initial position outside the screen on left side
                        height: preferredHeight
                        width: preferredWidth
                        //Layout.alignment: Qt.AlignHCenter

                        name: "foobar"
                        isCrossItem: false
                        //bgColor: "red"

                        Behavior on x {
                            SequentialAnimation {
                                SmoothedAnimation { velocity: 500 }
                                ScriptAction { script: {
                                        console.debug("Animation finished!")
                                        if (firstPlayerBox.visible)
                                            self.secondPlayerJoinedAnimationFinished()
                                    }
                                }
                            }
                       }

                        function reset() {
                            // hide to prevent animation while going to initial position
                            visible = false
                            name = "<undefined>"
                            x = root.width
                        }
                    }


                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            // these are just for testing on dev env
            console.debug("Devenv debug click for JoinView!")

            if (PlayerWaitingModel.numberOfJoinedPlayers() === 0) {
                //joinFirstPlayer("foobar")
                PlayerWaitingModel.joinPlayer(-1, "foobar")
            } else if (PlayerWaitingModel.numberOfJoinedPlayers() === 1) {
                //joinSecondPlayer("abc")
                PlayerWaitingModel.joinPlayer(-2, "abc")
            }
        }
    }


    Component.onCompleted: {
        //joinFirstPlayer("foobar")
        //joinSecondPlayer("abc")

        PlayerWaitingModel.callbacks.player1Joined.connect(joinFirstPlayer)
        PlayerWaitingModel.callbacks.player2Joined.connect(joinSecondPlayer)
    }

}
