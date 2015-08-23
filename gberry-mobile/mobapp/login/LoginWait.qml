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
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import ".." // for ViewTitleBar

Item {
    id: self
    anchors.fill: parent

    signal viewClosed()
    signal loginOk()
    signal loginFailed()
    //signal login(string userName)
    //signal logout(string userName)

    Rectangle {
        id: view
        anchors.centerIn: parent
        anchors.margins: border.width

        width: mainColumn.width
        height: mainColumn.height
        radius: 10
        color: "snow"
        //color: "red" // for debug
        border.width: 1
        border.color: "lightgray"

        ColumnLayout {
            id: mainColumn
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0


            ViewTitleBar {
                id: titleBar
                titleText: qsTr("Wait")
                Layout.fillWidth: true
                Layout.preferredHeight: preferredHeight

                onCloseSelected: onClose()
            }

            // divider between titlebar and other content
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                color: "darkgrey"
            }

            Item { // for margins
                id: content
                //color: "green"
                Layout.preferredWidth: waitTextLabel.implicitWidth + gdisplay.touchCellWidth()
                Layout.preferredHeight: waitTextLabel.implicitHeight + gdisplay.touchCellHeight()
                Layout.alignment: Qt.AlignHCenter

                Text {
                    id: waitTextLabel
                    anchors.centerIn: parent
                    text: qsTr("Logging in ... wait ...")
                }

            }
        }
    }

    GErrorDialog {
        id: errorDialog
        visible: false
        textPixelSize: gdisplay.smallSizeText

        //onVisibleChanged: console.debug("VISIBLE CHANGED FOR ERROR")
        function show(msg) { errorMessage = msg; visible = true }
        function hide() { visible = false; errorMessage = "" }

        onAcknowledged: { hide(); self.viewClosed() }
    }

    // -------------------------------------------------------------------------

    // actions when view is about to close
    function onClose() {
        viewClosed() // signal

        // TODO: cancel or what kind of state of ongoing login
    }

    function onLoginOk() {
        console.debug("Login ok")
        viewClosed()
        loginOk();

    }

    function onLoginFailed(errorMessage) {
        errorDialog.show("Login failed: " + errorMessage)
        loginFailed()
    }

    Component.onCompleted: {
        LoginModel.loginOk.connect(onLoginOk)
        LoginModel.loginFailed.connect(onLoginFailed)
    }
}


