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
import QtQuick.Layouts 1.1
import QtMultimedia 5.0

Rectangle {
    id: self

    // TODO: how to define in common place gradient for all views
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    MediaPlayer {
        id: playMusic
        // TODO: testing: too big for qrc
        //source: Assets.filePath("file:/home/tevuore/workspace/gberry/gberry-react-game/assets/BeatOne.wav")
        source: Assets.filePath("BeatOne.wav")
        // TODO: common with count down
        volume: 0
        Behavior on volume { SmoothedAnimation { velocity:  50 } }
    }

    onVisibleChanged: {
        if (self.visible) {
            playMusic.play()
            playMusic.volume = 0.2
        } else {
            playMusic.stop()
            playMusic.volume = 0
        }
    }

    function start() {
        playMusic.volume = 1.0
    }
    function stop() {
        playMusic.volume = 0.2
    }

    function setNumbers(listOfNumbers) {
        // TODO: assuming there is four
        firstbox.number = listOfNumbers[0]
        secondbox.number = listOfNumbers[1]
        thirdbox.number = listOfNumbers[2]
        fourthbox.number = listOfNumbers[3]
    }

    // relative coords
    function xForLeftMostBox() {
        return boxline.x + firstbox.x
    }
    // relative coords
    function yForBoxLine() {
        return boxline.y + firstbox.y
    }
    function stepSize() {
        // box width + margin
        return firstbox.width + boxline.spacing
    }

    RowLayout {
        id: boxline
        anchors.centerIn: parent
        spacing: gdisplay.largeSize * gdisplay.ppmText

        NumberBox {
            id: firstbox
        }

        NumberBox {
            id: secondbox
        }

        NumberBox {
            id: thirdbox
        }

        NumberBox {
            id: fourthbox
        }
    }
}

