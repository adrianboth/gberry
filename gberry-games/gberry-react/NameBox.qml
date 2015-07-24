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

