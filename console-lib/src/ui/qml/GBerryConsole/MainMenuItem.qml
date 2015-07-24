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

    signal selected()

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
    implicitWidth: itemText.implicitWidth

    Text {
        id: itemText
        text: item.text
        anchors.centerIn: parent
        font.pixelSize: textPixelHeight
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: selected()
    }
}
