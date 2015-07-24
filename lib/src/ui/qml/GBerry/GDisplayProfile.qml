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
 
 import QtQuick 2.2
import QtQuick.Window 2.2

import "js/DeveloperLog.js" as Log

Rectangle {
    // TODO: separate density for x and y
    property real ppmX: DisplayProfile.xdpi / 25.4 //Screen.pixelDensity
    property real ppmY: DisplayProfile.ydpi / 25.4
    property real ppmText: ppmX
    property real scaleFactor: 1.0 // TODO: testing, do we need this on qml side?

    property int windowWidth: DisplayProfile.windowWidth * scaleFactor
    property int windowHeight: DisplayProfile.windowHeight* scaleFactor

    // size on millimeters
    property real tinySize: 2.5 * scaleFactor
    property real smallSize: 5 * scaleFactor
    property real mediumSize: 10 * scaleFactor
    property real largeSize: 20 * scaleFactor

    function touchCellWidth() {
        return mediumSize * ppmText
    }
    function touchCellHeight() {
        return mediumSize * ppmText
    }

    function text_mm(size_mm) {
        return size_mm * ppmText
    }

    property real largeSizeText: text_mm(largeSize)
    property real mediumSizeText: text_mm(mediumSize)
    property real smallSizeText: text_mm(smallSize)
    property real tinySizeText: text_mm(tinySize)

    property real sizeCellFactor10: 1
    property real sizeCellFactor1: 0.1
    property real sizeCellFactor2: 0.2
    property real sizeCellFactor3: 0.3
    property real sizeCellFactor4: 0.4
    property real sizeCellFactor5: 0.5
    property real sizeCellFactor6: 0.6
    property real sizeCellFactor7: 0.7
    property real sizeCellFactor8: 0.8
    property real sizeCellFactor9: 0.9

    // TODO: do I yet physical dimensions somewhere? to calculate x & y separately

    function adjust(w, h) {
        Log.debug("window size: " + w + "x" + h)

        // TODO: real scaling
    }

    // TODO: func to check portrait, ie width < height

    function printDebug() {
        Log.debug("ppmX: " + ppmX)
        Log.debug("ppmY: " + ppmY)
        Log.debug("dpi: " + DisplayProfile.xdpi + "x" + DisplayProfile.ydpi)
        Log.debug("window size: " + DisplayProfile.windowWidth + "x" + DisplayProfile.windowHeight)
        Log.debug("DisplayProfile.sizeScaleFactor: " + DisplayProfile.sizeScaleFactor)
        Log.debug("Local scaleFactor: " + scaleFactor)
        Log.debug("orientation: " + Screen.orientation)
        Log.debug("primaryOrientation: " + Screen.primaryOrientation)
        Log.debug("screenWidth: " + Screen.width)
        Log.debug("screenHeight: " + Screen.height)
        Log.debug("desktopAvailableWidth: " + Screen.desktopAvailableWidth)
        Log.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)
        Log.debug("devicePixelRatio: " + Screen.devicePixelRatio)
        Log.debug("pixelDensity: " + Screen.pixelDensity)
        Log.debug("windowWidth: " + root.width)
        Log.debug("windowHeight: " + root.height)

        if (root.contentItem != null) {
            Log.debug("contentItem: " + root.contentItem.height)
        } else {
            Log.debug("NO CONTENT ITEM")
        }

        // TODO: combine above prints
        // TODO: additional information for physical size

        // TODO: on small mobile small size should be different than on large tablet

        // TODO: do we need ref sizes as we are using physical size
    }

    property bool debugInfoVisible: false
    visible: false
    anchors.fill: parent

    Rectangle {
        width: touchCellWidth()
        height: touchCellHeight()
        x: 0
        y: 0
        color: "red"
    }

    Rectangle {
        width: largeSize * ppmX
        height: largeSize * ppmY
        anchors.bottom: parent.bottom
        color: "green"
    }

    Text {
        text: "MediumSize"
        font.pixelSize: mediumSizeText
        anchors.bottom: parent.bottom
    }

    Component.onCompleted: {
        Log.initLog("GDisplayProfile", gsettings.logLevel)

        printDebug()
    }
}

