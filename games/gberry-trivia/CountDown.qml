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

Rectangle {
    id: self
    color: "snow"
    property int countFrom: 3 // seconds, counting downwards
    property int _currentNumber: 0

    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightgray" }
        GradientStop { position: 1.0; color: "darkgray" }
    }
    border.width: 2
    border.color: "lightgray"

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

    opacity: 0.9

    Text {
        id: number
        font.pixelSize: Math.min(55, gdisplay.largeSize * gdisplay.ppmText)
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

