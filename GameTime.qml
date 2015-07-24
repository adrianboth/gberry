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
        gametimelabel.text = time.toFixed(1)
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


