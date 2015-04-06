import QtQuick 2.0
import QtQuick.Layouts 1.1

import "gb:/js/DeveloperLog.js" as Log

Rectangle {
    color: "snow"
    // TODO: does radius change depending on pixel density
    radius: 20
    antialiasing: true

    // TODO: now assuming same vertical and horizontal density
    //       (might be different)
    width: columnLayout.implicitWidth + itemHeight * 0.5 * 2
    height: columnLayout.implicitHeight + itemHeight * 0.5 * 2 // same top and bottom margins as spacing

    signal playGameSelected()
    signal exitGameSelected()

    function moveFocusToNext() {
        console.debug("Next focus")
       //root.nextItemInFocusChain(true)
        if (playGameMenuEntry.focused) {
            exitGameMenuEntry.focused = true
            playGameMenuEntry.focused = false

        } else {
            exitGameMenuEntry.focused = false
            playGameMenuEntry.focused = true
        }
    }

    function moveFocusToPrevious() {
        console.debug("Prev focus")
        //root.nextItemInFocusChain(false)
        if (playGameMenuEntry.focused) {
            exitGameMenuEntry.focused = true
            playGameMenuEntry.focused = false
        }
        else {
            exitGameMenuEntry.focused = false
            playGameMenuEntry.focused = true
        }
    }

    function selectCurrent()
    {
        console.debug("Select current")
        if (playGameMenuEntry.focused)
            playGameMenuEntry.select()
        if (exitGameMenuEntry.focused)
            exitGameMenuEntry.select()
    }

    property color menuItemBgColor: "#2db6e1"
    property int menuItemRadius: 20
    property int textPixelHeight: gdisplay.mediumSize * gdisplay.ppmText
    property int itemHeight: textPixelHeight + (textPixelHeight * 0.25 *2) // with margins
    //property int preferredWidth:
    // TODO: calculate all texts and select maximum, and use minimum

    ColumnLayout {
        id: columnLayout
        anchors.centerIn: parent
        //anchors.fill: parent
        //height: playGameMenuEntry.implicitHeight + exitGameMenuEntry.implicitHeight + 2*35
        spacing: itemHeight * 0.5
        //spacing: 100

        Rectangle {
            id: playGameMenuEntry
            color: menuItemBgColor
            focus: true
            property bool focused: true

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: parent.maxMenuTextWidth + itemHeight * 0.25 * 2
            Layout.preferredHeight: itemHeight
            //anchors.margins: 15
            radius: menuItemRadius
            border.width: focused ? 5: 0
            border.color: "black"

            Text {
                id: playGameMenuEntryText
                text: "Play Game"
                //anchors.fill: parent
                anchors.centerIn: parent
                //verticalAlignment: Text.AlignVCenter
                //horizontalAlignment: Text.AlignHCenter
                font.pixelSize: textPixelHeight
                smooth: true

                // does these have any effect?
                style: Text.Raised
                styleColor: "transparent"
            }

            function select() {
                playGameSelected()
            }

            Keys.onPressed: {
                console.debug("######## KEY PRESSED (menu)")
                if (event.key === Qt.Key_Up)
                    moveFocusToNext()
                if (event.key === Qt.Key_Down)
                    moveFocusToPrevious()
                if (event.key === Qt.Key_Return)
                    selectCurrent()
            }


            Component.onCompleted: {
                playGameMenuEntry.forceActiveFocus()
            }
        }

        Rectangle {
            id: exitGameMenuEntry
            color: menuItemBgColor
            property bool focused: false
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 150
            Layout.preferredHeight: textPixelHeight
            anchors.margins: 15
            radius: menuItemRadius
            antialiasing: true

            border.width: focused ? 5: 0
            border.color: "black"

            Text {
                id: exitGameMenuEntryText
                text: "Exit"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: textPixelHeight
                smooth: true
            }

            function select() {
                exitGameSelected()
            }
        }

        function calculateMaxMenuTextWidth() {
            // implicit size is set by default for Text
            var itemList = [playGameMenuEntryText, exitGameMenuEntryText]
            var maxW = 0
            for (var i = 0; i < itemList.length; i++) {
                Log.debug("Check width: " + itemList[i].implicitWidth)
                Log.debug("Check height: " + itemList[i].implicitWidth)

                if (itemList[i].implicitWidth > maxW)
                    maxW = itemList[i].implicitWidth
            }
            Log.debug("Found max width: " + maxW)
            return maxW
        }

        property int maxMenuTextWidth: calculateMaxMenuTextWidth()
    }

    Component.onCompleted: {
        Log.initLog("MainMenu", gsettings.logLevel)

        columnLayout.maxMenuTextWidth = columnLayout.calculateMaxMenuTextWidth()
    }
}

