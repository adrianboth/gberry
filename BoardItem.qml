import QtQuick 2.4

import "GameModel.js" as GameModel

import "BoardItemRender.js" as BoardItemRender

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

    property var imageData: ""

    onItemSelectedChanged: {
        canvas.requestPaint() // todo: if rendenering in background thread then cross is visible for little time
        console.debug("### cross item changed")
    }

    Canvas {
        id: canvas
        renderStrategy: Canvas.Cooperative

        //x: 0 + crossMarginX
        //y: 0 + crossMarginY
        width: board.cellXSize //- 2*crossMarginX
        height: board.cellYSize //- 2*crossMarginY
        visible: parent.itemSelected

        property int crossMarginX: board.cellXSize * 0.1
        property int crossMarginY: board.cellYSize * 0.1
        property int circleMargin: board.cellXSize * 0.05

        onPaint: {
            //console.debug("### PAINTING:" + toDataURL())
            //devTimer.printTime()
            //if (self.imageData.length > 0)

            //var ctx = getContext("2d")
            //ctx.drawImage(Render._crossImage, 0, 0)
            BoardItemRender.drawCross(this)

            /*
            ctx.save()
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
            ctx.save()

            self.imageData = toDataURL()
            //devTimer.printTime()
            */
        }

        Component.onCompleted: {
            BoardItemRender.prerender(this)
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        z: 1000 // make sure on top
        onClicked: {
            devTimer.startWithEventQueueMarker()

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

