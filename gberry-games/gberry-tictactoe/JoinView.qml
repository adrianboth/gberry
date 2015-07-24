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
 
 /*
  Overview: In this view it is waited that two players join the game.

  There are:
   - titles
   - name boxes sliding from sides when players join
   - game start is triggered when second slide animation has finished

*/

import QtQuick 2.4
import QtQuick.Layouts 1.1

import "PlayerWaitingModel.js" as PlayerWaitingModel

Item {
    id: self

    signal secondPlayerJoinedAnimationStarted()
    signal secondPlayerJoinedAnimationFinished()

    function joinFirstPlayer(name) {

        firstPlayerBox.name = name
        xTransitionAnimation1.enabled = true

        firstPlayerBox.x = root.width / 2 - firstPlayerBox.preferredWidth / 2 // move to center
        // setting 'x' meant animation started -> trigger when finished
        firstPlayerBox.visible = true
    }

    function joinSecondPlayer(name) {

        secondPlayerBox.name = name
        xTransitionAnimation2.enabled = true
        secondPlayerBox.x = root.width / 2 - secondPlayerBox.preferredWidth / 2 // move to center
        // setting 'x' meant animation started -> trigger when finished
        secondPlayerBox.visible = true
        secondPlayerJoinedAnimationStarted()
    }

    function reset() {
        xTransitionAnimation1.enabled = false
        firstPlayerBox.reset()
        xTransitionAnimation2.enabled = false
        secondPlayerBox.reset()
    }

    ColumnLayout {
        anchors.centerIn: parent

        Item {
            //color: "green"
            Layout.preferredWidth: gameTitle.implicitWidth
            Layout.preferredHeight: gameTitle.implicitHeight
            Layout.alignment: Qt.AlignHCenter

            FontLoader { id: fixedFont; name: "FreeSans" }
            Text {
                id: gameTitle
                anchors.centerIn: parent
                text: "TicTacToe"
                font.family: fixedFont.name
                font.bold: true
                font.pixelSize: gdisplay.text_mm(15)
            }
            Component.onCompleted: console.debug("font pixel size: " + gameTitle.font.pixelSize)
        }

        // --

        // some room between title and other text
        Item {
            Layout.preferredWidth: waitingText.implicitWidth
            Layout.preferredHeight: waitingText.implicitHeight

        }

        // ---
        Item {
            //color: "green"
            Layout.preferredWidth: waitingText.implicitWidth
            Layout.preferredHeight: waitingText.implicitHeight
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: waitingText
                anchors.fill: parent
                text: qsTr("Waiting players to join ...")
                font.family: defaultFont.name
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

            // we are expecting two players
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
                        sizeScaleFactor: 1.5
                        //Layout.alignment: Qt.AlignHCenter

                        name: "test"
                        isCrossItem: true
                        //bgColor: "blue"

                        Behavior on x { id: xTransitionAnimation1
                            animation: SmoothedAnimation {

                                velocity: 500
                            } }

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
                        sizeScaleFactor: 1.5
                        //Layout.alignment: Qt.AlignHCenter

                        name: "foobar"
                        isCrossItem: false
                        //bgColor: "red"

                        Behavior on x {
                            id: xTransitionAnimation2

                            animation: SequentialAnimation {

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
        propagateComposedEvents: true

        onClicked: {
            console.debug("JOINVIEW MOUSEAREA")
            // always passing events forward
            mouse.accepted = false
            //gameTitle.font.pixelSize++
            //console.debug("NEW PIX SIZE: " + gameTitle.font.pixelSize)

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
        PlayerWaitingModel.callbacks.player1Joined.connect(joinFirstPlayer)
        PlayerWaitingModel.callbacks.player2Joined.connect(joinSecondPlayer)

        // for dev time
        //joinFirstPlayer("foobar")
        //joinSecondPlayer("abc")
    }

}
