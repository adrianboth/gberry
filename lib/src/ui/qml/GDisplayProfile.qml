import QtQuick 2.2

import "../js/DeveloperLog.js" as Log

Item {
    property int defaultTextPixelSize: 30
    property int defaultMemButtonSize: 35
    property int memButtonSize: 100
    property real textScale: 1.0
    property real gameSupportButtonsScale: textScale

    property real ppmmX: 5.0
    property real ppmmY: 6.0
    property real ppmmText: 1.5

    property real smallSize: 5
    property real mediumSize: 10
    property real largeSize: 10

    function adjust(w, h) {
        Log.debug("window size: " + w + "x" + h)

        // TODO: real scaling
    }

    Component.onCompleted: {
        Log.initLog("GDisplayProfile", gsettings.logLevel)
    }
}

