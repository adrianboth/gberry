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
 
 /**
 * ConfirmationDialog
 */
import QtQuick 2.4
import QtQuick.Layouts 1.1

import "js/DeveloperLog.js" as Log

// TODO: we could stack message and drop them downwards when they expire

Rectangle {
    id: self
    visible: false // by default hidden
    z: 1000 // tries to be always top most item

    // public
    property string feedbackMessage: "<undefined>"
    default property int showingTime: 1000 // ms
    property color bgColor: "snow"
    property real initialOpacity: 0.6
    property int textPixelSize: gdisplay.smallSize * gdisplay.ppmText

    function show(message) {
        if (visible) {
            // TODO: now just concatenating
            feedbackBox.feedbackMessage += " " + js["message"]
            closingTimer.running = false
            fadeAnimator.running = false
            opacity = initialOpacity

            closingTimer.running = true
        } else {
            feedbackMessage = message
            visible = true
        }
    }

    property int preferredHeight: textarea.height

    function close() {
        self.visible = false
        self.opacity = initialOpacity
    }

    onVisibleChanged: {
        if (visible)
            closingTimer.running = true
    }

    // private
    property int textMargin: gdisplay.touchCellHeight() // from one text line height

    color: "snow"
    radius: 20 // TODO: should change based on screen size?
    opacity: initialOpacity

    Item {
        id: textarea
        height: textLabel.contentHeight + textMargin // margin is based on line height
        width: textLabel.width // + 2 * textMargin
        anchors.centerIn: parent

        Text {
            id: textLabel
            text: feedbackMessage

            y: parent.y + textMargin // top margin
            anchors.centerIn: parent

            font.bold: false
            font.pixelSize: textPixelSize

            // needed for text wrapping to work
            width: self.width * 0.85

            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }

    SequentialAnimation {
        id: fadeAnimator
        running: false

        OpacityAnimator {
            target: self
            from: initialOpacity
            to: 0
            duration: 1000
            running: false
        }
        ScriptAction { script: close() }
    }

    Timer {
        id: closingTimer
        interval: showingTime
        onTriggered: fadeAnimator.running = true
    }

    Component.onCompleted: {
        Log.initLog("GFeedbackDialog", gsettings.logLevel)
    }
}
