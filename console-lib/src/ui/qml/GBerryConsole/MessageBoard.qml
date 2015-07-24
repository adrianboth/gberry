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
    height: 100
    border.width: 1
    anchors.bottomMargin: -1 // hide bottom border on screen
    color: "yellow"
    opacity: 0.5

    property string messageText: ""

    function insertMessage(msg) {
        messageText = msg + "\n" + messageText
        // TODO: cut lines at some point
    }

    function insertPlayerMessage(pid, msg) {
        var pname = playersManager.playerName(pid)
        insertMessage("[" + pname +"] " + msg)
    }

    Text {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 5
        id: textArea
        text: messageText
    }
}

