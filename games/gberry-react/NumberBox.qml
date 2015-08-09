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
    // what number is shown in button
    property int number: 0

    // TODO: scaling on correct proportions
    width: gdisplay.largeSize * gdisplay.ppmX
    height: gdisplay.largeSize * gdisplay.ppmY

    color: "gray"
    border.width: 2
    border.color: "lightgray"
    radius: 5

    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightgray" }
        GradientStop { position: 1.0; color: "darkgray" }
    }

    Text {
        anchors.centerIn: parent
        text: number.toString()
        color: "black"
        font.pixelSize: Math.min(55, gdisplay.largeSize * gdisplay.ppmText)
    }
}

