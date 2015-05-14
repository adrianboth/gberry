import QtQuick 2.4

Item {
    //color: "green"
    id: self

    // board size calculation is relaying of defined height
    // if this component

    function showWinningLine(x1, y1, x2, y2) {
        winningLinesCanvas.x1 = x1
        winningLinesCanvas.y1 = y1
        winningLinesCanvas.x2 = x2
        winningLinesCanvas.y2 = y2
        winningLinesCanvas.visible = true
        winningLinesCanvas.requestPaint()
    }

    Canvas {
        id: board
        renderStrategy: Canvas.Cooperative
        anchors.centerIn: parent
        // canvas size
        width: 3*cellXSize + 2*emptyMargin + 2* linedMargin
        height: 3*cellYSize + 2*emptyMargin + 2* linedMargin

        // on TV screen height is more limiting
        property int cellXSize: cellYSize

        //property int cellYSize: root.height / 6.5
        // trying to show as big board as possible
        property int cellYSize: parent.height * 0.85 / 3 // 0.85 comes from margins (0.3+0.15)/3

        property int emptyMargin: cellXSize * 0.30 // enough margins to get shadows fully visible
        property int linedMargin: cellXSize * 0.15
        property int fullMargin: emptyMargin + linedMargin
        property int fullXMargin: emptyMargin + linedMargin
        property int fullYMargin: emptyMargin + linedMargin

        property int x1: 0
        property int y1: 0
        property int x2: 0
        property int y2: 0

        property bool firstTime: true

        onPaint: {

            if (firstTime) {
                console.debug("onPaint(): Drawing grid")
                var ctx = getContext("2d")
                ctx.save()
                ctx.clearRect ( 0 , 0 , board.width, board.height );

                // setup the stroke
                ctx.lineWidth = 5
                ctx.strokeStyle = "black"
                ctx.shadowBlur=20;
                ctx.shadowOffsetX=15
                ctx.shadowOffsetY=15
                ctx.shadowColor="black";

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

                ctx.restore()
                firstTime = false
            }
        }

    }

    Canvas {
        id: winningLinesCanvas
        anchors.centerIn: parent
        width: board.width; height: board.height

        property int x1: 0
        property int y1: 0
        property int x2: 2
        property int y2: 2

        visible: false // initial

        onPaint: {
            console.debug("onPaint(): Drawing winning line")
            var ctx = getContext("2d")
            ctx.save()
            ctx.clearRect ( 0 , 0 , this.width, this.height );

            //ctx.fillStyle = 'rgb(100%, 70%, 30%)'
            //ctx.fillRect(0,0,this.width, this.height)

            // always drawing from left to right

            // origin in the middle of cell

            var extraXMod = 0
            var extraYMod = 0

            if (x1 === x2) {
                extraYMod = board.cellYSize/2
            } else if (y1 === y2) {
                extraXMod = board.cellXSize/2
            } else if ( y1 < y2) {
                // both different
                extraXMod = board.cellXSize/2
                extraYMod = board.cellYSize/2
            } else if (y1 > y2) {
                // diagonal line from left to right
                extraXMod = board.cellXSize/2
                extraYMod = -board.cellYSize/2
            }

            var x1Coord = x1 * board.cellXSize + board.cellXSize/2 + board.fullXMargin - extraXMod
            var y1Coord = y1 * board.cellYSize + board.cellYSize/2 + board.fullYMargin - extraYMod
            var x2Coord = x2 * board.cellXSize + board.cellXSize/2 + board.fullXMargin + extraXMod
            var y2Coord = y2 * board.cellYSize + board.cellYSize/2 + board.fullYMargin + extraYMod

            //console.debug("x1:" + x1.toString() + ", y1:" + y1.toString())
            //console.debug("x2:" + x2.toString() + ", y2:" + y2.toString())
            //console.debug("board.cellXSize:" + board.cellXSize.toString() + ", board.cellYSize:" + board.cellYSize.toString())
            //console.debug("board.fullXMargin:" + board.fullXMargin.toString()) // + ", extraXMod:" + extraXMod.toString())
            //console.debug("board.fullYMargin:" + board.fullYMargin.toString()) //+ ", extraYMod:" + extraYMod.toString())
            //console.debug("x1Coord:" + x1Coord.toString() + ", y1Coord:" + y1Coord.toString())
            //console.debug("x2Coord:" + x2Coord.toString() + ", y2Coord:" + y2Coord.toString())

            ctx.lineWidth = 10
            ctx.strokeStyle = "black"

            ctx.beginPath()

            ctx.moveTo(x1Coord, y1Coord)
            ctx.lineTo(x2Coord, y2Coord)

            ctx.stroke()
            ctx.restore()
        }
    }

    // board items are on own "layer"
    Item {
        id: boardCrossItems
        visible: true
        //color: "yellow"
        width: board.width; height: board.height
        anchors.centerIn: parent
    }

    Item {
        id: boardCircleItems
        visible: true
        //color: "yellow"
        width: board.width; height: board.height
        anchors.centerIn: parent
    }

    function initBoard() {
        console.log("Initializing board: creating objects");
        // 3x3 grid
        var xStart = board.fullMargin
        var yStart = board.fullMargin
        var x, y, component, sprite

        for (var i = 0; i < 3; i++) {
            for (var j = 0; j < 3; j++) {
                component = Qt.createComponent("BoardItem.qml");
                x = xStart + i * board.cellXSize
                y = yStart + j * board.cellYSize
                sprite = component.createObject(boardCrossItems, {isCrossItem: true, x: x, y: y, xCoord: i, yCoord: j}); //"itemSelected": true

                if (sprite === null) {
                    console.log("Error creating object");
                }

                sprite = component.createObject(boardCircleItems, {isCrossItem: false, x: x, y: y, xCoord: i, yCoord: j}); //"itemSelected": true

                if (sprite === null) {
                    console.log("Error creating object");
                }
            }
        }
    }

    function resetBoard() {
        console.debug("Reseting board")
        winningLinesCanvas.visible = false

        for (var i = 0; i < boardCrossItems.children.length; i++) {
            boardCrossItems.children[i].itemSelected = false
        }
        for (i = 0; i < boardCircleItems.children.length; i++) {
            boardCircleItems.children[i].itemSelected = false
        }
    }

    function markCell(type, x, y) {
        var c, items
        if (type === "x")
            items = boardCrossItems.children
        else
            items = boardCircleItems.children

        for (var i = 0; i < items.length; i++) {
            c = items[i]
            if (c.xCoord === x && c.yCoord === y) {
                c.itemSelected = true
                break
            }
        }
    }

    Component.onCompleted: {
        self.initBoard()

        //showWinningLine(0, 0, 2, 0)
        //showWinningLine(1, 0, 1, 2)
        //showWinningLine(0, 0, 2, 2)
        //showWinningLine(0, 2, 2, 0)
        //board.test()
    }

}
