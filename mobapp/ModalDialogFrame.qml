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
 
 import QtQuick 2.2

// This component is used to bring modal dialog effect.
// In achieve that this components needs to be direct child of root window, otherwise
// it can't fill whole view area.

Item {
    id: self
    anchors.fill: parent
    property color fadeColor: "black"
    property real  bgFadeAreaOpacity: 0.3
    property int   verticalCenterOffset: 0
    property var   content

    visible: false // default, content.visible will follow
    enabled: visible

    function show() { visible = true; content.visible = true }
    function hide() { visible = false; content.visible = false }

    signal backgroundClicked()

    // Fades existing components on background.
    // Note that actual content can't be child of this because they inherit opacity
    Rectangle {
        id: fadeBackground
        anchors.fill: parent
        opacity: bgFadeAreaOpacity
        color: fadeColor
    }

    MouseArea {
        anchors.fill: parent
        // steal all events i.e. prevent other components on the background getting them
        onClicked: backgroundClicked()
    }

    Component.onCompleted: {
        if (content !== null) {
            content.parent = self
            content.anchors.horizontalCenter = self.horizontalCenter
            content.anchors.verticalCenter = self.verticalCenter
            content.anchors.verticalCenterOffset = verticalCenterOffset //+topbar.height
            //content.visible = true // ModalFrameDialog controls visibility on parent level
        } else {
            console.error("[ModalDialogFrame] No 'content' defined!")
        }
    }
}
