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
    anchors.centerIn: parent
    anchors.fill: parent
    Text {
      text: "Please wait ..."
      anchors.centerIn: parent
    }



    // -- API --

    signal outgoingMessage(var data)

    function incomingMessage(data) {
        console.debug("### MAINUI APPBOX MSG: " + data)
        outgoingMessage("got appbox msg")
    }
}
