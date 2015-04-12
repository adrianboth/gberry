import QtQuick 2.2
import QtQuick.Window 2.2

import "js/DeveloperLog.js" as Log

Item {
    // TODO: separate density for x and y
    property real ppmX: Screen.pixelDensity
    property real ppmY: Screen.pixelDensity
    property real ppmText: Screen.pixelDensity
    property real scaleFactor: 1

    // size on millimeters
    property real smallSize: 5 * scaleFactor
    property real mediumSize: 10 * scaleFactor
    property real largeSize: 20 * scaleFactor

    function text_mm(size_mm) {
        return size_mm * ppmText
    }

    // TODO: do I yet physical dimensions somewhere? to calculate x & y separately

    function adjust(w, h) {
        Log.debug("window size: " + w + "x" + h)

        // TODO: real scaling
    }

    // TODO: func to check portrait, ie width < height

    function printDebug() {
        Log.debug("orientation: " + Screen.orientation)
        Log.debug("primaryOrientation: " + Screen.primaryOrientation)
        Log.debug("screenHeight: " + Screen.height)
        Log.debug("screenWidth: " + Screen.width)
        Log.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)
        Log.debug("desktopAvailableWidth: " + Screen.desktopAvailableWidth)
        Log.debug("devicePixelRatio: " + Screen.devicePixelRatio)
        Log.debug("pixelDensity: " + Screen.pixelDensity)
        Log.debug("windowHeight: " + root.height)
        Log.debug("windowWidth: " + root.width)

        // TODO: combine above prints
        // TODO: additional information for physical size

        // TODO: on small mobile small size should be different than on large tablet

        // TODO: do we need ref sizes as we are using physical size
    }

    Component.onCompleted: {
        Log.initLog("GDisplayProfile", gsettings.logLevel)

        printDebug()
    }
}

