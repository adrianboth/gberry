import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2


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
        property int endgame: 0 // 0 == no end, 1 == you won, 2 == opponent won

        onMyturnChanged: {
            console.debug("MyTurn changed! " + myturn.toString())
            selectLabelText()
        }

        onEndgameChanged: {
            console.debug("Endgame changed! " + endgame.toString())
            selectLabelText()
        }

        function selectLabelText() {
            if (endgame === 1) {
                turnTextLabel.text = qsTr("WINNER!")
            } else if (endgame === 2) {
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
                font.pixelSize: gdisplay.smallSizeText
            }

            Canvas {
                id: board
                opacity: boardview.myturn ? 1 : (0.5 - boardview.endgame  !== 0 ? 0.4 : 0)
                enabled: boardview.myturn

                // canvas size
                Layout.preferredWidth: 3*cellXSize + 2*emptyMargin + 2* linedMargin
                Layout.preferredHeight: 3*cellYSize + 2*emptyMargin + 2* linedMargin

                // on mobile device it depends from orientation
                // TODO: expecting now portrait
                property int cellXSize: Screen.primaryOrientation === Qt.PortraitOrientation ? self.width / 5.5 : self.height / 5.5
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
                            //recipe += 'x: ' + (x + 2) + '; y:' + (y + 2) + '; '
                            recipe += 'x: board.fullMargin + xx * board.cellXSize + 2; y: board.fullMargin + yy * board.cellYSize + 2; '
                            recipe += 'property int xx: ' + i + '; '
                            recipe += 'property int yy: ' + j + '; '
                            //recipe += 'width: ' + (board.cellXSize - 4) + '; height: ' + (board.cellYSize - 4) + '; '
                            recipe += 'width: board.cellXSize - 4; height: board.cellYSize - 4; '

                            recipe += 'MouseArea { property bool pressStarted: false; anchors.fill: parent; '
                            recipe += 'onPressed: { pressStarted = true; parent.color = Qt.darker(parent.color) } '
                            recipe += 'onExited: { pressStarted = false } '
                            recipe += 'onReleased: { pressFeedbackTimer.restart(); if (pressStarted) { board.onCellSelected(' + i + ', ' + j + ') } } '
                            recipe += '} '
                            recipe += 'Timer { id: pressFeedbackTimer; running: false; repeat: false; interval: 150; '
                            recipe += 'onTriggered: { parent.color = "lightgray"} '
                            recipe += '}'

                            recipe += '}'

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
            spacing: gdisplay.touchCellWidth()/4

            Text {
                id: confirmationText
                Layout.preferredHeight: confirmationText.contentHeight
                Layout.preferredWidth: confirmationText.width
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                //Layout.fillWidth: true

                text: dialog.text

                font.bold: true
                font.pixelSize: gdisplay.smallSizeText

                // needed for text wrapping to work
                width: Math.min(implicitWidth, root.width * 0.75) // max width 75% from screen

                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Rectangle  {
                id: button
                visible: dialog.showButton

                Layout.preferredHeight: button.buttonHeight
                Layout.preferredWidth: button.buttonWidth
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter

                color: buttonBgColor

                property string text: "<undefined>"
                property int buttonWidth: buttonLabel.implicitWidth + gdisplay.touchCellWidth() / 2 // + margins
                property int buttonHeight: buttonLabel.implicitHeight + gdisplay.touchCellHeight() / 2

                property color buttonBgColor: "#2db6e1"
                property color buttonFgColor: "black"

                radius: 20
                antialiasing: true
                opacity: enabled ? 1 : 0.5

                Text {
                    id: buttonLabel
                    anchors.centerIn: parent
                    text: dialog.buttonText
                    smooth: true
                    font.bold: true
                    font.pixelSize: gdisplay.touchCellHeight() /2
                    color: buttonFgColor
                    onTextChanged: {
                        // abort possible feedback if text changed
                        buttonPressedFeedbackTimer.running = false
                        button.color = parent.buttonBgColor
                        buttonLabel.color = parent.buttonFgColor
                    }
                }

                MouseArea {
                    id: buttonMouseArea
                    anchors.fill: parent
                    property bool pressStarted: false
                    /*
                    onClicked: {
                        console.debug("Button clicked!")
                        self.buttonClicked()
                    }
                    */
                    onPressed: {
                        pressStarted = true
                        button.color = Qt.darker(parent.buttonBgColor)
                        buttonLabel.color = Qt.lighter(parent.buttonFgColor)
                    }

                    onExited: {
                        pressStarted = false
                    }

                    onReleased: {
                        if (pressStarted) {
                            self.buttonClicked()
                        }

                        buttonPressedFeedbackTimer.restart()
                    }
                }

                Timer {
                    id: buttonPressedFeedbackTimer
                    running: false; repeat: false
                    interval: 150
                    onTriggered: {
                        button.color = parent.buttonBgColor
                        buttonLabel.color = parent.buttonFgColor
                    }
                }

                Timer {
                    id: dialogButtonVisibleTimer
                    running: false; repeat: false
                    interval: 2000
                    onTriggered: {
                        //parent.color = "red"
                        parent.enabled = true
                    }
                }
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
        } else if (state === "END_GAME_LOOSER" || state === "END_GAME_WINNER") {
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
                PropertyChanges { target: button; enabled: false } // keep first disabled and after some time enable (prevents accidental presses)
                PropertyChanges {
                    target: dialogButtonVisibleTimer
                    running: true
                }
            },
            State {
                name: "PLAY_GAME_MY_TURN"
                PropertyChanges { target: boardview; visible: true; myturn: true; endgame: 0 }
                PropertyChanges { target: dialog; visible: false }
            },
            State {
                name: "PLAY_GAME_WAIT_TURN"
                PropertyChanges { target: boardview; visible: true; myturn: false; endgame: 0}
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
                name: "END_GAME_WINNER"
                PropertyChanges { target: boardview; visible: true; endgame: 1 }
                PropertyChanges {
                    target: dialog
                    visible: true
                    text: ""
                    buttonText: "Next"
                    showButton: true
                }
            },
            State {
                name: "END_GAME_LOOSER"
                PropertyChanges { target: boardview; visible: true; endgame: 2 }
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
