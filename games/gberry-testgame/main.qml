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
 
import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0


Window {
    id: root
    visible: true
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    FontLoader { id: defaultFont
                 //name: "FreeSans"
                 source: "file:/usr/share/fonts/truetype/freefont/FreeSans.ttf"}

    // invisible area that can be used during dev time to exit app
    MouseArea {
        id: topRightExitArea
        anchors.top: parent.top; anchors.right: parent.right
        width: 25; height: 25
        onClicked: Qt.quit()
    }

    InfoBar {
        id: infobar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        z: 100

        gradient: Gradient {
            GradientStop { position: 0.0; color: "snow" }
            GradientStop { position: 1.0; color: "gray" }
        }
    }

    Rectangle {
        id: mainarea
        anchors.fill: parent
        color: "steelblue"
        /* Gradient doesn't look good on Raspi
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }
        */
    }

    MessageBoard {
        id: messageBoard
        opacity: 0.5
        visible: true

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    // hide inactive mouse
    MouseArea {
        id: hideMouseArea
        anchors.fill: parent
        cursorShape: Qt.BlankCursor
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            cursorShape = Qt.ArrowCursor
            hideMouseCursorTimer.restart()
        }

        Timer {
            id: hideMouseCursorTimer
            running: false
            interval: 10000 // 10 seconds
            repeat: false
            onTriggered: hideMouseArea.cursorShape = Qt.BlankCursor
        }

    }

    GFeedbackDialog {
        id: feedbackDialog
        visible: false
        //feedbackMessage: "This is a test message, quite long This is a test message, quite long This is a test message, quite long"
        showingTime: 3000 // ms
        height: preferredHeight
        textPixelSize: gdisplay.mediumSize * gdisplay.ppmText
        initialOpacity: 0.6
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    // -------------------------------------------------------------------------

    function onPlayerIn(pid)
    {
        console.log("Player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "ExitGame", actionName: "Exit Game"}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "DefineAppBox", data: AppBoxMaster.dataStr()}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        // in our case all actions happen in appbox -> we can show it all the time
        js = {action: "ShowAppBox"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))
    }

    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "Player left")
        var msg = qsTr("Player left the game.")
        var msgToSend = {action: "FeedbackMessage",
                     message: msg}
        playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
        feedbackDialog.show(msg)
    }

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)
        var js  = JSON.parse(data)
        var msgToSend, msg

        if (js["action"] === "GeneralAction") {
            if (js["id"] === "ExitGame") {
                Qt.quit()
            }

        } else if (js["action"] === "AppBoxMessage") {
            console.debug("AppBoxMessage")

        }
    }

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        //feedbackDialog.show("test message.")
    }

}
