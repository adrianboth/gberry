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
import QtQuick.Layouts 1.1

Rectangle {
    id: item

    // public
    property string text: "<undefined>"
    property url imageUrl: ""

    // 'pid' of player who initiated action
    signal selected(var pid)

    // expects parent to have 'textPixelHeight' -> common for all menu items
    property int textPixelHeight: parent.textPixelHeight
    property int maxTextWidth: parent.maxTextWidth
    property int itemHeight: parent.itemHeight

    // we need to our own focus marker as we are controlling outside the current focus
    property bool focused: false

    // private
    color: "#2db6e1"

    Layout.alignment: Qt.AlignCenter
    Layout.preferredWidth: maxTextWidth + itemHeight * 0.25 * 2
    Layout.preferredHeight: itemHeight
    radius: 20 // TODO: size scaling
    border.width: focused ? 5: 0 // TODO: size scaling
    border.color: "black"

    // width is accessed outside to calculate max width from entries
    implicitWidth: itemText.implicitWidth + itemImage.implicitWidth + gdisplay.touchCellWidth() * 4

    Image {
        id: itemImage
        visible: item.imageUrl != ""
        source: item.imageUrl
        sourceSize.height: itemHeight * 0.6
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: gdisplay.touchCellWidth()
    }

    Text {
        id: itemText
        text: item.text
        anchors.verticalCenter: parent.verticalCenter
        //anchors.left: itemImage.right
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: textPixelHeight
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: selected(-1) // if clicked by mouse no specific user is associated, meant for development time
    }
}
