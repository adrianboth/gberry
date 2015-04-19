import QtQuick 2.4

Rectangle {
    id: self
    color: "snow"
    property int countFrom: 3 // seconds, counting downwards
    property int _currentNumber: 0

    signal finished()
    function start() {
        self.opacity = 0.9
        self.visible = true
        number.text = countFrom
        _currentNumber = countFrom
        timer.running = true
    }

    width: number.implicitHeight *1.5 // with margin
    height: number.implicitHeight *1.5 // with margin
    radius: 10
    border.width: 1
    border.color: Qt.lighter(self.color)
    opacity: 0.9

    Text {
        id: number
        font.pixelSize: gdisplay.largeSize * gdisplay.ppmText
        anchors.centerIn: parent
    }

    Timer {
        id: timer
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            // zero is shown 1s but when turning to 0 game starts
            if (_currentNumber === 0) {
                timer.running = false
                self.visible = false

            } else {
                _currentNumber--
                number.text = _currentNumber

                if (_currentNumber === 0) {
                    // timer finished but still show zero for a moment
                    finished()
                    zeroFadeAnimation.running = true

                }
            }

        }
    }


    NumberAnimation {
        id: zeroFadeAnimation
        target: self
        property: "opacity"
        running: false
        loops: 1
        from:  0.9
        to: 0
        duration: 500
        easing.type: Easing.InOutQuad
    }

}

