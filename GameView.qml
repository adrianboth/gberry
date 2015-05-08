import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtMultimedia 5.0

Rectangle {
    id: self

    // TODO: how to define in common place gradient for all views
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    function start() {
        // TODO: needed?
    }
    function stop() {
        // TODO: needed?
    }

    Item {
        id: boardMargins
        anchors.centerIn: parent
        width: board.width
        height: board.height

        Canvas {
            id: board
            anchors.centerIn: parent
            // canvas size
            width: 3*cellXSize + 2*emptyMargin + 2* linedMargin
            height: 3*cellYSize + 2*emptyMargin + 2* linedMargin

            // TODO: make relative calculation
            property int cellXSize: 100
            property int cellYSize: 100
            property int emptyMargin: cellXSize * 0.35 // enough margins to get shadows fully visible
            property int linedMargin: cellXSize * 0.15
            property int fullMargin: emptyMargin + linedMargin

            onPaint: {
                var ctx = getContext("2d")

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
                ctx.lineTo(board.width - emptyMargin, cellYSize + fullYMargin)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(emptyMargin, 2*cellYSize + fullYMargin)
                ctx.lineTo(board.width - emptyMargin, 2*cellYSize + fullYMargin)
                ctx.stroke()

                // first vertical linedMargin
                ctx.beginPath()
                ctx.moveTo(cellXSize + fullXMargin, 0 + emptyMargin)
                ctx.lineTo(cellXSize + fullXMargin, board.height - emptyMargin)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(2*cellXSize + fullXMargin, 0 + emptyMargin)
                ctx.lineTo(2*cellXSize + fullXMargin, board.height - emptyMargin)
                ctx.stroke()
            }

            // clears all cells
            function clear() {
                // TODO
            }

            // type=x|o
            function drawItemTo(x, y, type) {
                // TODO:

                // this is not real model, just for drawing
                //  -> we are not making any checks
            }



            Item {
                id: cross
                // TODO: x, y for testing
                x: board.fullMargin
                y: board.fullMargin
                width: board.cellXSize
                height: board.cellYSize

                //color: "green" // for testing

                property int xCoord: 0
                property int yCoord: 0
                property bool itemSelected: false
                property bool isCrossItem: true
                property bool isGrayed: false

                Canvas {
                    //x: 0 + crossMarginX
                    //y: 0 + crossMarginY
                    width: board.cellXSize //- 2*crossMarginX
                    height: board.cellYSize //- 2*crossMarginY
                    visible: parent.itemSelected

                    property int crossMarginX: board.cellXSize * 0.1
                    property int crossMarginY: board.cellYSize * 0.1
                    property int circleMargin: board.cellXSize * 0.05

                    onPaint: {
                        var ctx = getContext("2d")
                        if (parent.isGrayed) {
                            ctx.fillStyle="#FF0000";
                            ctx.fillRect(0 + crossMarginX,
                                         0 + crossMarginY, this.width - crossMarginX, this.height - crossMarginY)

                        } else if (parent.isCrossItem) {
                            // setup the stroke
                            ctx.lineWidth = 5
                            ctx.strokeStyle = "blue"
                            //ctx.shadowBlur=5;
                            //ctx.shadowOffsetX=5
                            //ctx.shadowOffsetY=5
                            //ctx.shadowColor="blue";

                            ctx.beginPath()
                            ctx.moveTo(0 + crossMarginX, 0 + crossMarginY)
                            ctx.lineTo(this.width - crossMarginX, this.height - crossMarginY)
                            ctx.stroke()

                            ctx.beginPath()
                            ctx.moveTo(this.width - crossMarginY, 0 + crossMarginY)
                            ctx.lineTo(0 + crossMarginX, this.height - crossMarginY)
                            ctx.stroke()

                        } else {
                            // circle item
                            ctx.lineWidth = 5
                            ctx.strokeStyle = "red"

                            ctx.beginPath()
                            ctx.arc(this.width/2, this.height/2, this.width/2 - 2*circleMargin, 0, Math.PI*2, true)
                            ctx.stroke()
                        }

                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!parent.itemSelected) {
                            console.debug("Selected: " + parent.xCoord + "," + parent.yCoord)
                            parent.itemSelected = true
                        }
                    }
                }
            }
        }


    }
}

// ok, because of mouse click, I need to populate the whole grid

// canvas size needs to be larger
//  a) grid going little bit over cell size
//  b) shadows not stripped
