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
import QtQuick.Controls 1.2

import GBerry 1.0
import GBerryConsole 1.0


Window {
    id: root
    visible: true
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    /*
    MainView {
        id: mainView
        anchors.fill: parent
        enabled: false
    }*/

    // Loader is required if
    //  a) Loading takes time
    //  b) or it would trigger sending message (e.g. to comms) -> need to wait connection ok

    Loader {
        id: mainViewLoader
        active: false
        source: "MainView.qml"
        visible: false
        anchors.fill: parent

        onLoaded: {
            item.initExistingPlayers()
            visible = true
            connectionWaitingDialog.visible = false
        }
    }

    Rectangle {
        id: connectionWaitingDialog
        visible: true
        color: "lightsteelblue"
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        Text {
            text: qsTr("Loading menu ...")
            anchors.centerIn: parent
            font.pixelSize: gdisplay.mediumSizeText
        }

    }

    MouseArea {
        height: gdisplay.touchCellHeight()
        width: gdisplay.touchCellWidth()
        anchors.top: parent.top
        anchors.right: parent.right
        onClicked: {
            Qt.quit()
        }

    }

    // =========================================================================

    function onConnectionChanged() {
        // TODO: how to wait until connection ok
        console.debug("### onConnectionChanged()")
        if (Connection.isConnected) {
            console.debug("IS CONNECTED")
            mainViewLoader.active = true
            connectionWaitingDialog.visible = false
        } else {
            connectionWaitingDialog.visible = true

            // TODO: how to disable
        }
    }

    function onActivatedChanged() {
        // currently this doesn't have any affect to mainui

        // as we don't keep any special list of player or states, this doesn't matter
        // but in other case we should discard our player list, reopens will come
    }

    function onPlayerIn(pid, pname)
    {
        // clear potential previous actions
        var js = {action: "DefineGeneralActions",actions: []} // no actions
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        console.log("New player in: id = " + pid + ", name = " + pname)
        if (mainViewLoader.status == Loader.Ready)
            mainViewLoader.item.onPlayerIn(pid, pname)

    }

    function onPlayerOut(pid, pname)
    {
        console.log("Player left: id = " + pid + ", name = " + pname)
        if (mainViewLoader.status == Loader.Ready)
            mainViewLoader.item.onPlayerOut(pid, pname)

    }

    function onPlayerMessageReceived(pid, data)
    {
        if (mainViewLoader.status == Loader.Ready)
            mainViewLoader.item.onPlayerMessageReceived(pid, data)

    }

    // TODO: how to pass function to view loaded by loader

    Component.onCompleted: {
        // make sure these are in place when connection opens
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        Connection.isConnectedChanged.connect(onConnectionChanged)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        // check current connection
        onConnectionChanged()
    }
}
