import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

import "appbox"

Window {
    id: appboxDebugWindow
    visible: true

    // TV screen for console app is scaled down to fit to screen
    //   -> there is no that problem with mobile -> take it off
    width: singleAppboxWidth * 2// / DisplayProfile.sizeScaleFactor // dimension mode
    height: singleAppboxHeight // / DisplayProfile.sizeScaleFactor

    property int singleAppboxWidth: calcX(realWidth)
    property int singleAppboxHeight: calcY(realHeigth) - calcY(platformTopBarPixelSize)

    // TODO: all components (derived from GDisplayProfile values) use size scale factor ...
    function calcX(pixels) {
        return pixels / realXdpi * DisplayProfile.xdpi
    }
    function calcY(pixels) {
        return pixels / realYdpi * DisplayProfile.ydpi
    }

    property real realWidth: 480.0
    property real realHeigth: 800.0
    property real realXdpi: 223.0
    property real realYdpi: 223.0
    property int platformTopBarPixelSize: 12

    // TODO: DisplayProfile also for debug window

    /*
    Rectangle {
        anchors.fill: parent
        color: "red"
    }
    */

    RowLayout {
        anchors.fill: parent
        spacing: 0

        AppBoxDebug {
            //anchors.fill: parent
            playerName: "Dev1"
            Layout.preferredWidth: singleAppboxWidth
            Layout.preferredHeight: singleAppboxHeight
            Layout.maximumWidth: singleAppboxWidth
            Layout.maximumHeight: singleAppboxHeight
        }

        AppBoxDebug {
            playerName: "Dev2"
            Layout.preferredWidth: singleAppboxWidth
            Layout.preferredHeight: singleAppboxHeight
            Layout.maximumWidth: singleAppboxWidth
            Layout.maximumHeight: singleAppboxHeight
        }

        /*
        AppBoxDebug {
            playerName: "Dev2"
        }
        */

    }

}
