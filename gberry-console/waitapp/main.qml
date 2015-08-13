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

    GSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }


    Rectangle {
        id: mainarea
        color: "lightsteelblue"
        anchors.fill: parent

        border.color: "slategray"
        /*
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }
        */
        Image {
            id: gblogo
            source: "images/gberry_logol_with_text.svg"
            sourceSize.width: 200
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: gdisplay.touchCellHeight()/2
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: hourglass
            source: "images/hourglass.svg"
            sourceSize.width: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -textImage.basicWidth*1.2 / 2 - width
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: textImage
            property int basicWidth: 500
            source: "images/wait_text.svg"
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            sourceSize.width: basicWidth
            //sourceSize.height: 250


            SequentialAnimation on sourceSize.width {
                loops: Animation.Infinite

                PropertyAnimation { duration: 2500; to: textImage.basicWidth * 1.2 }
                PropertyAnimation { duration: 2500; to: textImage.basicWidth * 0.8}
            }
        }
        /*
        Text {
            anchors.centerIn: parent
            text: qsTr("Starting ... please wait ...")
            font.pixelSize: gdisplay.mediumSizeText

            // not best and smoothiest animation ... but enough for testing
            SequentialAnimation on font.pixelSize {
                loops: Animation.Infinite

                PropertyAnimation { duration: 2000; to: gdisplay.mediumSizeText * 1.2 }
                PropertyAnimation { duration: 2000; to: gdisplay.mediumSizeText * 0.8}
            }
        }*/


    }

    function onPlayerIn(pid, playerName)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertMessage("[" + playerName + "] New player")

        var js = {action: "DefineGeneralActions",
                  actions: []} // no actions
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "DefineAppBox",
              data: AppBoxMaster.dataStr()}

        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "ShowAppBox"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))
    }

    function onPlayerOut(pid, playerName)
    {
        console.log("Player left: id = " + pid)
        messageBoard.insertMessage("[" + playerName + "] Player left")
    }

    function onPlayerMessageReceived(pid, data)
    {
        // nothing to do
    }

    /*
    MessageBoard {
        id: messageBoard
        opacity: 0.5
        visible: gsettings.developmentMode

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }
    */

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")
    }
}
