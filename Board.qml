import QtQuick 2.4

Item {
    id: self
    width: board.width
    height: board.height

    function showWinningLine(x1, y1, x2, y2) {
        board.drawWinningLine = true
        board.x1 = x1
        board.y1 = y1
        board.x2 = x2
        board.y2 = y2
        board.requestPaint()
    }

    Canvas {
        id: board
        anchors.centerIn: parent
        // canvas size
        width: 3*cellXSize + 2*emptyMargin + 2* linedMargin
        height: 3*cellYSize + 2*emptyMargin + 2* linedMargin

        // TODO: make relative calculation
        //property int cellXSize: 100
        //property int cellYSize: 100

        // on TV screen height is more limiting
        property int cellXSize: cellYSize
        property int cellYSize: root.height / 6.5

        property int emptyMargin: cellXSize * 0.30 // enough margins to get shadows fully visible
        property int linedMargin: cellXSize * 0.15
        property int fullMargin: emptyMargin + linedMargin

        property bool drawWinningLine: false
        property int x1: 0
        property int y1: 0
        property int x2: 0
        property int y2: 0

        property bool firstTime: true
        function test() {
            var ctx = getContext("2d")

            // setup the stroke
            ctx.lineWidth = 5
            ctx.ctx.beginPath()
            ctx.moveTo(0,0)
            ctx.lineTo(200,200)
            ctx.stroke()
        }

        // TODO: for some reason drawing winning line takes some time ... to much
        onPaint: {
            console.debug("onPaint()")

            var ctx = getContext("2d")
            ctx.clearRect ( 0 , 0 , board.width, board.height );

            if (firstTime) {
                console.debug("onPaint(): Drawing grid")
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

                //firstTime = false
            }

            if (drawWinningLine) {
                console.debug("onPaint(): Drawing winning line")
                // always from left to right

                // origin in the middle of cell

                var extraXMod = 0
                var extraYMod = 0
                if (x1 === x2) {
                    extraYMod = cellYSize/2
                } else if (y1 === y2) {
                    extraXMod = cellXSize/2
                } else if ( y1 < y2) {
                    // both different
                    extraXMod = cellXSize/2
                    extraYMod = cellYSize/2
                } else if (y1 > y2) {
                    // diagonal line from left to right
                    extraXMod = cellXSize/2
                    extraYMod = -cellYSize/2
                }

                var x1Coord = x1 * cellXSize + cellXSize/2 + fullXMargin - extraXMod
                var y1Coord = y1 * cellYSize + cellYSize/2 + fullYMargin - extraYMod
                var x2Coord = x2 * cellXSize + cellXSize/2 + fullXMargin + extraXMod
                var y2Coord = y2 * cellYSize + cellYSize/2 + fullYMargin + extraYMod

                ctx.lineWidth = 10
                ctx.strokeStyle = "black"
                ctx.shadowBlur = 0
                ctx.shadowOffsetX=0
                ctx.shadowOffsetY=0

                ctx.beginPath()
                ctx.moveTo(x1Coord, y1Coord)
                ctx.lineTo(x2Coord, y2Coord)
                ctx.stroke()
            }

        }

    }

    function initBoard() {
        // 3x3 grid
        var xStart = board.fullMargin
        var yStart = board.fullMargin
        var x, y, component, sprite

        for (var i = 0; i < 3; i++) {
            for (var j = 0; j < 3; j++) {
                console.log("Creating objects");
                component = Qt.createComponent("BoardItem.qml");
                x = xStart + i * board.cellXSize
                y = yStart + j * board.cellYSize
                sprite = component.createObject(board, {"x": x, "y": y, xCoord: i, yCoord: j}); //"itemSelected": true

                if (sprite == null) {
                    // Error Handling
                    console.log("Error creating object");
                }
            }
        }
    }

    function resetBoard() {
        console.debug("Reseting board")
        board.drawWinningLine = false
        for (var i = 0; i < board.children.length; i++) {
            board.children[i].itemSelected = false
        }

        board.requestPaint()
    }

    function markCell(type, x, y) {
        var c
        for (var i = 0; i < board.children.length; i++) {
            c = board.children[i]
            if (c.xCoord === x && c.yCoord === y) {
                c.isCrossItem = type === "x"
                c.itemSelected = true
                break
            }
        }

        board.requestPaint()
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
