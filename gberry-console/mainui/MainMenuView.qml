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

import GBerryConsole 1.0

Item {
    id: self
    onVisibleChanged: {
        if (visible)
            mainMenu.forceActiveFocus()
    }

    onFocusChanged: {
        if (focus)
            mainMenu.focus = true
    }

    property var enabledControlActions: ["Up", "Down", "OK"] // defaults

    function processControlAction(action, pid) {
        if (action === "Up")
            mainMenu.moveFocusToPrevious()
        else if (action === "Down")
            mainMenu.moveFocusToNext()
        else if (action === "OK")
            mainMenu.selectCurrent(pid)
    }

    property alias menuItems: mainMenu.items

    Image {
        id: gblogo
        source: "images/gberry_logo_with_text.svg"
        sourceSize.width: 500
        anchors.top: parent.top
        //anchors.left: parent.left
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.margins: gdisplay.touchCellHeight() *2
        fillMode: Image.PreserveAspectFit
    }

    MainMenu {
        id: mainMenu
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: gblogo.bottom
        anchors.margins: gdisplay.touchCellHeight()

    }

    Text {
        id: playersJoinedLabel
        text: {
            var msg
            if (playersManager.numberOfPlayers === 1)
                msg = qsTr(" player connected")
            else
                msg = qsTr(" players connected")

            return playersManager.numberOfPlayers.toString() + msg
        }

        font.pixelSize: gdisplay.mediumSizeText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: mainMenu.bottom
        anchors.topMargin: gdisplay.touchCellHeight()
    }

    Item {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        property string numberOfPlayers: playersManager.numberOfPlayers
        property string playersText: "Players: " + playersManager.numberOfPlayers
        property string commsStatusText: Connection.isActivated ? qsTr("Connected") : "--"
        property string headServerStatusText: Connection.isActivated ? qsTr("Connected") : "--"
        property string timeText: "-"

        height: commsStatus.implicitHeight

        Text {
            id: commsStatus
            visible: !Connection.isActivated
            text: qsTr("No connection to comms")
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: gdisplay.touchCellWidth()
            font.pixelSize: gdisplay.mediumSizeText
        }


        Text {
            id: headServerStatus
            visible: !Connection.isHeadServerConnected
            text: qsTr("No connection to GBerry server")
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: gdisplay.touchCellWidth()
            font.pixelSize: gdisplay.mediumSizeText
        }
    }
}

