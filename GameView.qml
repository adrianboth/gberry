import QtQuick 2.0
import QtQuick.Layouts 1.1

import "GameModel.js" as GameModel

Item {
    id: self

    property string player1Name: "xxx"
    property string player2Name: "yyy"

    signal gameClosed()

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
            } else {
                mouse.accepted = false
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

        property int fullHeight: height + anchors.topMargin

        Text {
            id: headerText
            text: qsTr("TURN")
            font.bold: false
            font.pixelSize: gdisplay.mediumSize* playerBoxes.sizeScaleFactor * gdisplay.ppmText

            Behavior on x { SmoothedAnimation { duration: 500; } } // ms

            // TODO: these could be states
            function moveToPlayer1() {
                this.text = qsTr("TURN")
                this.font.bold = false
                this.x = Math.max(0, (firstPlayerBox.x + firstPlayerBox.width/2) - implicitWidth / 2)
            }

            function moveToPlayer2() {
                this.text = qsTr("TURN")
                this.font.bold = false
                this.x = Math.min(root.width - implicitWidth, (secondPlayerBox.x + secondPlayerBox.width/2) - implicitWidth / 2)
            }

            function moveToCenter() {
                this.text = qsTr("TURN")
                this.font.bold = false
                this.x = parent.width / 2 - implicitWidth / 2
            }

            function gameEndedDraw() {
                this.text = qsTr("DRAW!")
                this.font.bold = true
                this.x = parent.width / 2 - implicitWidth / 2
            }

            function player1Won() {
                this.text = qsTr("WINNER!")
                this.font.bold = true
                this.x = Math.max(0, (firstPlayerBox.x + firstPlayerBox.width/2) - implicitWidth / 2)
            }

            function player2Won() {
                this.text = qsTr("WINNER!")
                this.font.bold = true
                this.x = Math.min(root.width - implicitWidth, (secondPlayerBox.x + secondPlayerBox.width/2) - implicitWidth / 2)
            }
        }
    }

    Item {
        //color: "yellow"
        id: playerBoxes
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        width: root.width
        height: firstPlayerBox.height

        property int xMargin: gdisplay.touchCellWidth() / 2
        property int fullHeight: height
        property real sizeScaleFactor: 1.5

        NameBox {
            id: firstPlayerBox
            x: parent.xMargin
            height: preferredHeight
            width: preferredWidth
            sizeScaleFactor: parent.sizeScaleFactor

            name: player1Name
            isCrossItem: true
        }

        NameBox {
            id: secondPlayerBox
            x: root.width - preferredWidth - parent.xMargin // initial position outside the screen on left side
            height: preferredHeight
            width: preferredWidth
            sizeScaleFactor: parent.sizeScaleFactor

            name: player2Name
            isCrossItem: false
        }
    }

    Board {
        id: gameboard
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: playerBoxes.bottom

        width: parent.width
        height: root.height - header.fullHeight - playerBoxes.fullHeight - gametime.height
    }

    // game time is just a reference
    GameTime {
        id: gametime
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: preferredWidth
        height: preferredHeight
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
