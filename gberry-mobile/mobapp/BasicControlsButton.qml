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
 
 import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2


Button {
    id: button

    signal buttonPressed()

    property string buttonGradientStartColor: "#eee"
    property string buttonGradientStopColor: "#ccc"
    property string buttonText: "-" // default

    // size could be scaled based on screen size
    property real scalingFactor: 1.0

    property int defaultButtonFontPixelSize: gdisplay.smallSizeText
    property int buttonFontPixelSize: defaultButtonFontPixelSize * scalingFactor

    property int defaultButtonHorizontalMargin: gdisplay.touchCellWidth()
    property int buttonHorizontalMargin: defaultButtonHorizontalMargin * scalingFactor

    property int defaultButtonVerticalMargin: gdisplay.touchCellHeight()
    property int buttonVerticalMargin: defaultButtonVerticalMargin * scalingFactor

    property int defaultButtonWidth: defaultButtonFontPixelSize + defaultButtonHorizontalMargin
    property int buttonWidth: defaultButtonWidth * scalingFactor

    property int defaultButtonHeight: defaultButtonFontPixelSize + defaultButtonVerticalMargin
    property int buttonHeight: defaultButtonHeight * scalingFactor

    property alias buttonTextImplicitWidth: buttonLabel.implicitWidth


    function enable() {
        button.enabled = true
        button.opacity = 1.0
    }
    function disable() {
        button.enabled = false
        button.opacity = 0.5
    }

    // defaults
    width: buttonWidth
    height: buttonHeight

    style: ButtonStyle {
        background: Rectangle {
                    border.width: 1
                    border.color: "#888"
                    radius: 4
                    gradient: Gradient {
                        // by default button didn't provide any feedback and some reason
                        // checking up.pressed condition didn't work
                        GradientStop { position: 0 ; color: button.buttonGradientStartColor }
                        GradientStop { position: 1 ; color: button.buttonGradientStopColor }
                    }
                }
    }

    Text {
        id: buttonLabel
        text: button.buttonText
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: buttonFontPixelSize
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            buttonPressed()
            button.buttonGradientStartColor = "#ccc"
            button.buttonGradientStopColor = "#aaa"
            feedbackTimer.running = true
        }
    }

    Timer {
        id: feedbackTimer
        interval: 100
        running: false
        repeat: false
        onTriggered: {
            button.buttonGradientStartColor = "#eee"
            button.buttonGradientStopColor = "#ccc"
        }
    }

}
