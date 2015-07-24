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
 
 // ---
// MainMenu is list of vertical menu items
//
// Usage:
//   MainMenu {
//       id: mainmenu
//       anchors.centerIn: parent
//       items: [
//           MainMenuItem { text: qsTr("Play Game"); onSelected: playGameSelected() },
//           MainMenuItem { text: qsTr("Exit"); onSelected: exitGameSelected() }
//       ]
//   }
//


import QtQuick 2.0
import QtQuick.Layouts 1.1

import GBerry 1.0 // 'Log' comes here

Rectangle {
    id: mainmenu

    property list<MainMenuItem> items

    color: "snow"
    // TODO: does radius change depending on pixel density
    radius: 20
    antialiasing: true
    focus: true

    // TODO: now assuming same vertical and horizontal density
    //       (might be different)
    width: columnLayout.implicitWidth + columnLayout.itemHeight * 0.5 * 2
    height: columnLayout.implicitHeight + columnLayout.itemHeight * 0.5 * 2 // same top and bottom margins as spacing


    function moveFocusToNext() {
        console.debug("Next focus")

        var focusIndex = currentFocusIndex()
        var newFocusIndex = focusIndex + 1

        if (newFocusIndex === items.length)
            newFocusIndex = 0

        items[focusIndex].focused = false
        items[newFocusIndex].focused = true
    }

    function moveFocusToPrevious() {
        console.debug("Prev focus")

        var focusIndex = currentFocusIndex()
        var newFocusIndex = focusIndex - 1

        if (newFocusIndex < 0)
            newFocusIndex = items.length - 1

        items[focusIndex].focused = false
        items[newFocusIndex].focused = true
    }

    function currentFocusIndex() {
        var focusIndex = -1
        for (var i = 0; i < items.length; i++) {
            if (items[i].focused) {
                focusIndex = i;
                break;
            }
        }
        Log.debug("Focus index: " + focusIndex)
        return focusIndex
    }

    function selectCurrent() {
        console.debug("Select current")
        var focusIndex = currentFocusIndex()
        items[focusIndex].selected()
    }

    function calculateMaxMenuTextWidth() {
        // implicit size is set by default for Text
        var maxW = 0
        for (var i = 0; i < items.length; i++) {
            Log.trace("Check width: " + items[i].implicitWidth)
            Log.trace("Check height: " + items[i].implicitWidth)

            if (items[i].implicitWidth > maxW)
                maxW = items[i].implicitWidth
        }
        Log.trace("Found max width: " + maxW)
        return maxW
    }


    ColumnLayout {
        id: columnLayout

        property int textPixelHeight: gdisplay.mediumSize * gdisplay.ppmText
        property int itemHeight: textPixelHeight + (textPixelHeight * 0.25 *2) // with margins
        property int maxTextWidth: 100 // temporary default value, proper value is calculated

        anchors.centerIn: parent
        spacing: itemHeight * 0.5

        children: parent.items
    }

    Keys.onPressed: {
        console.debug("######## KEY PRESSED (menu)")
        if (event.key === Qt.Key_Up)
            moveFocusToPrevious()
        if (event.key === Qt.Key_Down)
            moveFocusToNext()
        if (event.key === Qt.Key_Return)
            selectCurrent()
    }

    Component.onCompleted: {
        Log.initLog("MainMenu", gsettings.logLevel)

        columnLayout.maxTextWidth = calculateMaxMenuTextWidth()
        // force focus to first item
        items[0].focused = true
    }
}
