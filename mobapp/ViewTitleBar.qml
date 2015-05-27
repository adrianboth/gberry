import QtQuick 2.0

Item {
    id: self

    signal closeSelected()

    property string titleText: "<undefined>"
    // width is adjustable
    property int preferredHeight: gdisplay.touchCellHeight()

    Text {
        text: titleText
        font.pixelSize: gdisplay.smallSizeText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: gdisplay.touchCellHeight() / 4
    }

    Rectangle {
        id: closeButton
        width: closeButton.height
        height: parent.height * 0.65
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: gdisplay.touchCellHeight() / 4
        radius: 50 // circle
        border.width: 1
        border.color: "grey"
        color: "lightgrey"

        Canvas {
            id: cross
            // canvas size
            width: parent.width; height: parent.height
            // handler to override for drawing
            property real crossLineMarginX: 0.3 * cross.width
            property real crossLineMarginY: 0.3 * cross.height

            onPaint: {
                // get context to draw with
                var ctx = getContext("2d")
                // setup the stroke
                ctx.lineWidth = 2
                ctx.strokeStyle = "black"
                // setup the fill
                ctx.fillStyle = "steelblue"
                // begin a new path to draw
                ctx.beginPath()

                ctx.moveTo(0 + crossLineMarginX, 0 + crossLineMarginY)
                ctx.lineTo(cross.width - crossLineMarginX, cross.height - crossLineMarginY)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(cross.width - crossLineMarginX, 0 + crossLineMarginY)
                ctx.lineTo(0 + crossLineMarginX, cross.height - crossLineMarginY)
                ctx.stroke()
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                closeSelected()
            }
        }
    }
}
