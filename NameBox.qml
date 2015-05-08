import QtQuick 2.0

Rectangle {
    id: self
    property string name: "undefined"
    property color bgColor: "white" // just default, each box should get own color
    width: nameBoxText.implicitWidth + gdisplay.smallSize * gdisplay.ppmText
    height: nameBoxText.implicitHeight + gdisplay.smallSize * gdisplay.ppmText
    antialiasing: true
    radius: 5
    border.width: 1
    border.color: Qt.lighter(bgColor)

    onBgColorChanged: {
        self.color = bgColor
        self.border.color = Qt.lighter(bgColor)
    }

    Behavior on x { SmoothedAnimation { velocity: 200 } }

    Text {
        id: nameBoxText
        text: self.name
        anchors.centerIn: parent
        font.pixelSize: gdisplay.smallSize * gdisplay.ppmText
    }

    function shake() {
        self.rotation -= 15
        shakeTimer1.running = true
    }

    Behavior on rotation {
        SmoothedAnimation { velocity: 500 }
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

