import QtQuick 2.3

// component showing running time
Text {
    id: self

    function start() {
        _startTime = new Date().valueOf()
        timer.running = true
    }
    function reset() {
        setTime(0.0)
    }

    function stop() {
        _stopTime = new Date().valueOf()
        timer.running = false

    }
    function restart() {
        stop()
        reset()
        start()
    }

    // returns seconds
    function finalTime() {
        return ((_stopTime - _startTime)/1000).toFixed(1)
    }

    property real _startTime: 0.0
    property real _stopTime: 0.0

    text: "0.0"
    z: 100 // always on top when visible
    font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText

    function setTime(time) {
        // 0.0 resolution
        self.text = time.toFixed(1)
    }

    Timer {
        id: timer
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            var currentTime = new Date().valueOf() // milliseconds
            setTime((currentTime-_startTime) / 1000)
        }
    }
}
