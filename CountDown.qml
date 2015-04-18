import QtQuick 2.4

Rectangle {
    id: self
    property int countFrom: 5 // seconds, counting downwards
    property int _currentNumber: 0

    signal finished()
    function start() {
        self.visible = true
        number.text = countFrom
        _currentNumber = countFrom
        timer.running = true
    }

    Text {
        id: number
        font.pixelSize: gdisplay.largeSize * gdisplay.ppmText
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
                    // timer finished but still show zero for a momemt
                    finished()
                }
            }

        }
    }

}

