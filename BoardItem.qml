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
    property bool isCrossItem: truee

    Canvas {
        id: canvas
        renderStrategy: Canvas.Cooperative

        // x & y are set when BoardItem is instantiated

        width: board.cellXSize //- 2*crossMarginX
        height: board.cellYSize //- 2*crossMarginY
        visible: parent.itemSelected

        property int crossMarginX: board.cellXSize * 0.1
        property int crossMarginY: board.cellYSize * 0.1
        property int circleMargin: board.cellXSize * 0.05

        onPaint: {
            var ctx = canvas.getContext('2d')
            ctx.save()
            ctx.clearRect (0 , 0 , canvas.width, canvas.height)

            if (isCrossItem) {
                console.debug("### PAINT CROSS")

                // setup the stroke
                ctx.lineWidth = 5
                ctx.strokeStyle = gsettings.crossColor

                ctx.beginPath()
                ctx.moveTo(0 + canvas.crossMarginX, 0 + canvas.crossMarginY)
                ctx.lineTo(canvas.width - canvas.crossMarginX, canvas.height - canvas.crossMarginY)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(canvas.width - canvas.crossMarginY, 0 + canvas.crossMarginY)
                ctx.lineTo(0 + canvas.crossMarginX, canvas.height - canvas.crossMarginY)
                ctx.stroke()

            } else {
                console.debug("### PAINT CIRCLE")

                ctx.lineWidth = 5
                ctx.strokeStyle = gsettings.circleColor

                ctx.beginPath()
                ctx.arc(canvas.width/2, canvas.height/2, canvas.width/2 - 2*canvas.circleMargin, 0, Math.PI*2, true)
                ctx.stroke()
            }
            ctx.restore()
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
                GameModel.markCell(parent.xCoord, parent.yCoord)
                // we are not marking visually item, game model will trigger
                // marking if everything is ok
            }

        }

    }
}
