import QtQuick 2.3
import QtQuick.Layouts 1.1


Rectangle {
    id: self
    property string name: "undefined"
    property color bgColor: isCrossItem ? gsettings.crossColor : gsettings.circleColor
    property bool isCrossItem: true
    property real sizeScaleFactor: 1.0

    property int preferredWidth: row.width + gdisplay.smallSize * gdisplay.ppmText
    property int preferredHeight: row.height
    antialiasing: true
    radius: 5
    border.width: 1
    border.color: Qt.lighter(bgColor)

    onBgColorChanged: {
        self.color = bgColor
        self.border.color = Qt.lighter(bgColor)
    }

    RowLayout {
        id: row
        anchors.centerIn: parent
        spacing: gdisplay.touchCellWidth() *0.5

        Canvas {
            id: sign
            property int size: nameBoxText.font.pixelSize * 0.8 // pixelSize is fixed, doesn't change based on name text
            Layout.preferredHeight: size
            Layout.preferredWidth: size

            property int crossMarginX: size * 0.2
            property int crossMarginY: size * 0.2
            property int circleMargin: size * 0.1

            onPaint: {
                var ctx = getContext("2d")
                if (self.isCrossItem) {
                    // setup the stroke
                    ctx.lineWidth = 5
                    ctx.strokeStyle = "black"

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
                    ctx.strokeStyle = "black"

                    ctx.beginPath()
                    ctx.arc(this.width/2, this.height/2, this.width/2 - 2*circleMargin, 0, Math.PI*2, true)
                    ctx.stroke()
                }
            }
        }

        Text {
            id: nameBoxText
            //horizontalAlignment: Text.AlignHCenter
            Layout.preferredWidth: nameBoxText.implicitWidth + gdisplay.smallSize * gdisplay.ppmText
            //Layout.preferredHeight: nameBoxText.implicitHeight + gdisplay.smallSize * gdisplay.ppmText
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            text: self.name
            font.pixelSize: gdisplay.mediumSize * self.sizeScaleFactor * gdisplay.ppmText
        }
    }

    function shake() {
        self.rotation -= 15
        shakeTimer1.running = true
    }

    Behavior on rotation {
        SmoothedAnimation { duration: 500 } // ms
    }

    Timer {
        id: shakeTimer1
        interval: 200
        onTriggered: {
            self.rotation += 30
            shakeTimer2.running = true
        }
    }
    Timer {
        id: shakeTimer2
        interval: 200
        onTriggered: {
            self.rotation -=15
        }
    }
}

