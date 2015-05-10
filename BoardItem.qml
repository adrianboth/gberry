import QtQuick 2.4

import "GameModel.js" as GameModel

Item {
    id: self

    // TODO: referring to above ...
    width: board.cellXSize
    height: board.cellYSize

    //color: "green" // for testing

    property int xCoord: 0
    property int yCoord: 0
    property bool itemSelected: false
    property bool isCrossItem: true
    property bool isGrayed: false

    onItemSelectedChanged: {
        canvas.requestPaint()
        console.debug("### cross item changed")
    }

    Canvas {
        id: canvas
        //x: 0 + crossMarginX
        //y: 0 + crossMarginY
        width: board.cellXSize //- 2*crossMarginX
        height: board.cellYSize //- 2*crossMarginY
        visible: parent.itemSelected

        property int crossMarginX: board.cellXSize * 0.1
        property int crossMarginY: board.cellYSize * 0.1
        property int circleMargin: board.cellXSize * 0.05

        onPaint: {
            console.debug("### PAINTING")
            var ctx = getContext("2d")
            context.clearRect ( 0 , 0 , canvas.width, canvas.height );

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
        z: 1000 // make sure on top
        onClicked: {
            if (!parent.itemSelected) {
                console.debug("Selected: " + parent.xCoord + "," + parent.yCoord)
                //parent.itemSelected = true

                if (GameModel.markCell(parent.xCoord, parent.yCoord)) {

                    //var cross = GameModel.isCrossItem(parent.xCoord, parent.yCoord)
                    //console.debug("### cross: " + cross.toString())
                    //parent.isCrossItem = GameModel.isCrossItem(parent.xCoord, parent.yCoord)
                    //parent.itemSelected = true
                }
            }
        }
    }
}

