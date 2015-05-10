import QtQuick 2.0
import QtQuick.Layouts 1.1


Rectangle {
    id: self
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    Item {
        id: boardview
        anchors.centerIn: parent

        visible: false

        property bool myturn: false
        property bool endgame: false

        onMyturnChanged: {
            console.debug("MyTurn changed! " + myturn.toString())
            selectLabelText()
        }

        onEndgameChanged: {
            console.debug("Endgame changed! " + endgame.toString())
            selectLabelText()
        }

        function selectLabelText() {
            if (endgame) {
                turnTextLabel.text = qsTr("Game over!")
            } else if (myturn) {
                turnTextLabel.text = qsTr("Your turn!")
            } else {
                turnTextLabel.text = qsTr("Wait!")
            }
        }

        onVisibleChanged: {
            if (visible) {
                board.resetBoard()
            }
        }

        ColumnLayout {
            anchors.centerIn: parent

            Text {
                id: turnTextLabel
                Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Wait!")
                font.pixelSize: 24
            }

            Canvas {
                id: board
                opacity: boardview.myturn ? 1 : (0.5 - boardview.endgame ? 0.4 : 0)
                enabled: boardview.myturn

                // canvas size
                Layout.preferredWidth: 3*cellXSize + 2*emptyMargin + 2* linedMargin
                Layout.preferredHeight: 3*cellYSize + 2*emptyMargin + 2* linedMargin

                // on mobile device it depends from orientation
                // TODO: expecting now portrait
                property int cellXSize: root.width / 5
                property int cellYSize: cellXSize

                property int emptyMargin: cellXSize * 0.30 // enough margins to get shadows fully visible
                property int linedMargin: cellXSize * 0.15
                property int fullMargin: emptyMargin + linedMargin

                // TODO: board
                onPaint: {
                    console.debug("onPaint(): Drawing grid")

                    var ctx = getContext("2d")
                    //ctx.clearRect ( 0 , 0 , board.width, board.height );

                    // setup the stroke
                    ctx.lineWidth = 5
                    ctx.strokeStyle = "black"

                    // total four lines
                    // drawing direction:
                    //   - first horizontal lines from top to bottom
                    //   - then vertical lines from left to right

                    // we have empty margin (just empty space) to have space for shadows
                    // then lines go little bit further than is the cell size
                    //  -> i.e x line length = 3*cellXSize + 2*linedMargin
                    //
                    var fullYMargin = emptyMargin + linedMargin
                    var fullXMargin = emptyMargin + linedMargin

                    // first horizontal line
                    ctx.beginPath()
                    ctx.moveTo(emptyMargin, cellYSize + fullYMargin)
                    ctx.lineTo(this.width - emptyMargin, cellYSize + fullYMargin)
                    ctx.stroke()

                    ctx.beginPath()
                    ctx.moveTo(emptyMargin, 2*cellYSize + fullYMargin)
                    ctx.lineTo(this.width - emptyMargin, 2*cellYSize + fullYMargin)
                    ctx.stroke()

                    // first vertical linedMargin
                    ctx.beginPath()
                    ctx.moveTo(cellXSize + fullXMargin, 0 + emptyMargin)
                    ctx.lineTo(cellXSize + fullXMargin, this.height - emptyMargin)
                    ctx.stroke()

                    ctx.beginPath()
                    ctx.moveTo(2*cellXSize + fullXMargin, 0 + emptyMargin)
                    ctx.lineTo(2*cellXSize + fullXMargin, this.height - emptyMargin)
                    ctx.stroke()
                }

                function initBoard() {
                    // 3x3 grid
                    var xStart = board.fullMargin
                    var yStart = board.fullMargin
                    var x, y, component, sprite

                    for (var i = 0; i < 3; i++) {
                        for (var j = 0; j < 3; j++) {
                            console.log("Creating objects");
                            x = xStart + i * board.cellXSize
                            y = yStart + j * board.cellYSize
                            var recipe = 'import QtQuick 2.0; Rectangle { opacity: 0; color: "lightgray"; '
                            recipe += 'x: ' + (x + 2) + '; y:' + (y + 2) + '; '
                            recipe += 'property int xx: ' + i + '; '
                            recipe += 'property int yy: ' + j + '; '
                            recipe += 'width: ' + (board.cellXSize - 4) + '; height: ' + (board.cellYSize - 4) + '; '
                            recipe += 'MouseArea { anchors.fill: parent; onClicked: { board.onCellSelected(' + i + ', ' + j + ') } }'
                            recipe += ' }'
                            sprite = Qt.createQmlObject(recipe, board, 'BoardCell');
                            // TODO: are these destroyed automatically or do we need to destroy
                            //       them manually when parent is destroyed
                        }
                    }
                }

                function onCellSelected(x, y) {
                    console.debug("Cell selected: " + x + ", " + y)
                    self.cellSelected(x, y)
                }

                function markCell(x, y) {
                    for (var i = 0; i < children.length; i++) {
                        if (children[i].xx === x && children[i].yy === y) {
                            children[i].opacity = 0.5
                            break
                        }
                    }
                }

                function resetBoard() {
                    console.debug("Reseting board")
                    for (var i = 0; i < board.children.length; i++) {
                        board.children[i].opacity = 0
                    }
                }

                Component.onCompleted: {
                    initBoard()
                }

            }

            // TODO: indication of turn of other ...
        }
    }

    // -- alternative

    Item {
        id: dialog
        anchors.centerIn: parent
        visible: true // by default hidden

        z: 1000 // tries to be always top most item

        property string text: "<undefined>"
        property string buttonText: "<undefined>"
        property bool showButton: true

        height: column.height
        width: column.width
        //color: "snow"
        //radius: 20 // TODO: should change based on screen size?

        ColumnLayout {
            id: column
            anchors.centerIn: parent
            spacing: gdisplay.touchCellWidth()/2

            Text {
                id: confirmationText
                Layout.preferredHeight: confirmationText.contentHeight
                Layout.preferredWidth: confirmationText.width
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                //Layout.fillWidth: true

                text: dialog.text

                font.bold: true
                font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText // TODO: to somewhere else

                // needed for text wrapping to work
                width: Math.min(implicitWidth, root.width * 0.75) // max width 75% from screen

                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Rectangle  {
                //color: "red"
                id: button
                visible: dialog.showButton

                Layout.preferredHeight: button.buttonHeight
                Layout.preferredWidth: button.buttonWidth
                //Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter


                    //anchors.centerIn: parent
                    //width: buttonWidth
                    //height: buttonHeight
                    color: "#2db6e1"

                    property string text: "<undefined>"
                    property int buttonWidth: buttonLabel.implicitWidth + gdisplay.touchCellWidth() // + margins
                    property int buttonHeight: buttonLabel.implicitHeight + gdisplay.touchCellHeight()

                    radius: 20
                    antialiasing: true

                    Text {
                        id: buttonLabel
                        anchors.centerIn: parent
                        text: dialog.buttonText
                        smooth: true
                        font.bold: true
                        font.pixelSize: gdisplay.touchCellHeight() * 1.5
                    }

                    MouseArea {
                        id: buttonMouseArea
                        anchors.fill: parent
                        onClicked: {
                            console.debug("Button clicked!")
                            self.buttonClicked()
                        }
                    }
                //}
            }

        }
    }

    // -- API --

    // Outgoing AppBoxMessages
    //   js: json object
    signal outgoingMessage(var js)

    // Incoming AppBoxMessages
    //   js: json object
    function incomingMessage(js) {

        if (!"action" in js) {
            console.error("Unknown message format: " + js)
            return
        }
        console.debug("IncomingMessage: " + js["action"])

        if (js["action"] === "MoveToState") {
            var state  = js["state"]
            self.state = state

        } else if (js["action"] === "MarkCell") {
            board.markCell(js["x"], js["y"])
        }
    }

    function cellSelected(x, y) {
        var js = {action: "SelectCell", x: x, y: y}
        outgoingMessage(js)
    }

    function buttonClicked() {
        // depends from the state what button is doing
        if (state === "WAITING_PLAYERS") {
            joinSelected()
        } else if (state === "JOINED_WAITING") {
            cancelJoin()
        } else if (state === "END_GAME") {
            nextGame()
        }

        // TODO: end game situation
    }

    function joinSelected() {
        var js = {action: "JoinGame"}
        outgoingMessage(js)
    }

    function cancelJoin() {
        var js = {action: "CancelJoin"}
        outgoingMessage(js)
    }

    function nextGame() {
        var js = {action: "NextGame"}
        outgoingMessage(js)
    }

    Component.onCompleted: {
        //dialog.text = "Join to game"
        //button.text = "Join"
    }

    state: "INITIAL"

    states: [
            State { name: "INITIAL"
            PropertyChanges { target: boardview; visible: false }
            /*
            PropertyChanges {
                target: dialog
                visible: true
                text: "Please wait ..."
                showButton: false
            }*/
            },

            State {
                name: "WAITING_PLAYERS"
                PropertyChanges { target: boardview; visible: false }
                PropertyChanges {
                    target: dialog
                    visible: true
                    text: "Waiting players to join ..."
                    showButton: true
                    buttonText: "Join"
                }
            },
            State {
                name: "JOINED_WAITING"
                PropertyChanges { target: boardview; visible: false }
                PropertyChanges {
                    target: dialog
                    visible: true
                    text: "Joined to game, waiting others ..."
                    showButton: true
                    buttonText: "Cancel"
                }
            },
            State {
                name: "PLAY_GAME_MY_TURN"
                PropertyChanges { target: boardview; visible: true; myturn: true; endgame: false }
                PropertyChanges { target: dialog; visible: false }
            },
            State {
                name: "PLAY_GAME_WAIT_TURN"
                PropertyChanges { target: boardview; visible: true; myturn: false; endgame: false}
                PropertyChanges { target: dialog; visible: false }
            },
            State {
                name: "WAIT_GAME"
                PropertyChanges { target: boardview; visible: false }
                PropertyChanges {
                    target: dialog
                    visible: true
                    text: "Game ongoing, please wait ..."
                    showButton: false
                }
            },
            State {
                name: "END_GAME"
                PropertyChanges { target: boardview; visible: true; endgame: true }
                PropertyChanges {
                    target: dialog
                    visible: true
                    text: ""
                    buttonText: "Next"
                    showButton: true
                }
            }

        ]

}
