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
    id: infobar

    property string numberOfPlayers: playersManager.numberOfPlayers
    property string playersText: "Players: " + playersManager.numberOfPlayers
    property string commsStatusText: Connection.isActivated ? qsTr("Connected") : "--"
    property string headServerStatusText: Connection.isActivated ? qsTr("Connected") : "--"
    property string timeText: "-"

    height: 30

    opacity: 0.5
    color: "grey"

    Text {
        id: commsStatus
        text: qsTr("Comms: ") + commsStatusText
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
        font.pixelSize: 12
    }


    Text {
        id: headServerStatus
        text: qsTr("HeadServer: ") + headServerStatusText
        anchors.left: commsStatus.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 50
        font.pixelSize: 12
    }

    Text {
        id: players
        text: playersText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: time
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        text: timeText
    }

    Timer {
         interval: 500; running: true; repeat: true
         onTriggered: {
             // TODO: should be property instead
             //console.debug("### UPDATING " + Connection.isActivated)
             time.text = new Date().toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
             infobar.commsStatusText = Connection.isActivated ? qsTr("Connected") : "--"
             infobar.headServerStatusText = Connection.isHeadServerConnected ? qsTr("Connected") : "--"
             //infobar.commsStatusText = comms.isOpen() ? qsTr("Connected") : "--"
         }
     }
}

