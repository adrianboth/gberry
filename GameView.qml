import QtQuick 2.0
import QtQuick.Layouts 1.1

import "GameModel.js" as GameModel

Item {
    id: self

    // TODO: how to define in common place gradient for all views

    property string player1Name: "xxx"
    property string player2Name: "yyy"

    signal gameClosed()

    // TODO: make game playeable directly -> easier to develop

    function reset() {
        gametime.reset()
        headerText.moveToCenter()
        gameboard.enabled = false
        gameboard.resetBoard()
    }

    function player1Turn() {
        gameboard.enabled = true // very first turn will enable
        gametime.restart()
        headerText.moveToPlayer1()
    }

    function player2Turn() {
        gametime.restart()
        headerText.moveToPlayer2()
    }

    onVisibleChanged: {
        if (visible) {
            player1Name = GameModel.player1.name
            player2Name = GameModel.player2.name
        } else {
            reset() // prepare in advance for next game
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: {
            console.debug("CLICKED MAIN AREA")
            // handle clicks only in the end
            if (GameModel.isGameEnded()) {
                gameClosed()
                mouse.accepted = true
            }
        }
    }

    Item {
        id: header
        //color: "green"
        width: root.width
        height: headerText.implicitHeight

        anchors.top: self.top
        anchors.topMargin: headerText.font.pixelSize / 2
        //anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: headerText
            text: qsTr("Turn")
            //x: playerBoxes.xMargin
            //x: (firstPlayerBox.x + firstPlayerBox.width/2) - implicitWidth / 2
            //anchors.top: self.top
            //anchors.topMargin: font.pixelSize
            //anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText

            Behavior on x { SmoothedAnimation { velocity: 500; } }

            function moveToPlayer1() {
                this.text = qsTr("Turn")
                this.x = (firstPlayerBox.x + firstPlayerBox.width/2) - implicitWidth / 2
            }

            function moveToPlayer2() {
                this.text = qsTr("Turn")
                this.x = (secondPlayerBox.x + secondPlayerBox.width/2) - implicitWidth / 2
            }

            function moveToCenter() {
                this.text = qsTr("Turn")
                this.x = parent.width / 2 - implicitWidth / 2
            }

            function gameEndedDraw() {
                this.text = qsTr("DRAW!")
                this.x = parent.width / 2 - implicitWidth / 2
            }

            function player1Won() {
                this.text = qsTr("WINNER!")
                this.x = (firstPlayerBox.x + firstPlayerBox.width/2) - implicitWidth / 2
            }

            function player2Won() {
                this.text = qsTr("WINNER!")
                this.x = (secondPlayerBox.x + secondPlayerBox.width/2) - implicitWidth / 2
            }
        }
    }

    Item {
        //color: "yellow"
        id: playerBoxes
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: gameboard.top

        //height: 100
        property int xMargin: gdisplay.touchCellWidth() / 2

        NameBox {
            id: firstPlayerBox
            x: parent.xMargin
            height: preferredHeight
            width: preferredWidth
            //Layout.alignment: Qt.AlignHCenter

            name: player1Name
            isCrossItem: true
            //bgColor: "red"
        }

        NameBox {
            id: secondPlayerBox
            x: root.width - preferredWidth - parent.xMargin // initial position outside the screen on left side
            height: preferredHeight
            width: preferredWidth
            //Layout.alignment: Qt.AlignHCenter

            name: player2Name
            isCrossItem: false
            //bgColor: "red"
        }
    }

    // TODO: Players names in boxes
    //   Draw small circle (red) and cross (blue) next to names

    Board {
        id: gameboard
        anchors.centerIn: parent
    }

    // game time is just a reference
    GameTime {
        id: gametime
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    function onTurnSwitched(playerNumber) {
        if (playerNumber === 1) {
            player1Turn()
        } else {
            player2Turn()
        }
    }

    function onGameEndedDraw() {
        console.debug("### GAME DRAW!")
        gametime.stop()
        headerText.gameEndedDraw()
        console.debug("### GAME DRAW  2!")
    }

    function onGameEnded(playerNumber,x1, y1, x2, y2) {
        gametime.stop()
        if (playerNumber === 1)
            headerText.player1Won()
        else
            headerText.player2Won()

        gameboard.showWinningLine(x1, y1, x2, y2)
    }

    Component.onCompleted: {
        reset()
        GameModel.callbackTurnSwitched.turnSwitched.connect(onTurnSwitched)
        GameModel.callbacks.gameEndedDraw.connect(onGameEndedDraw)
        GameModel.callbacks.gameEnded.connect(onGameEnded)
        GameModel.callbacks.cellMarked.connect(gameboard.markCell)
    }

}
