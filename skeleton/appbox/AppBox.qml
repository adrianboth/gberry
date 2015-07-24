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

// TODO: create button that sends data back to mainui

Rectangle {
    color: "lightgrey"
    border.width: 1
    border.color: "black"
    width: 200
    height: 50
    anchors.centerIn: parent
    Text {
      text: "CONTENT FROM MAINUI"
      anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: outgoingMessage("hello mainui")
    }

    // -- API --

    signal outgoingMessage(var data)

    function incomingMessage(data) {
        console.debug("### MAINUI APPBOX MSG: " + data)
        outgoingMessage("got appbox msg")
    }
}
