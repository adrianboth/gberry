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

import GBerry 1.0

// TODO: using mousearea (just for history), but didn't realize that may have blocked normal press feedback
//       (button haven't received events)

Rectangle {
    id: basicControls

    property int maxHeight: 0 // not defined
    property int maxWidth: 0 // not defined

    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }


    // "Up", "Down", "Right", "Left", "OK"
    signal buttonPressed(var buttonID)

    function enable(buttonList) {
        var children = basicControls.children

        if (buttonList.length === 0) {
            Log.debug("Enabling all control buttons")
            for (var c = 0; c < buttons.lenght; c++) {
                buttons[c].enable()
            }

        } else {
            // only selected are enabled
            Log.debug("Enabling: " + buttonList.toString())
            var childrenById = {}

            // first disable all and built util map
            for (var c = 0; c < buttons.length; c++) {
                // couldn't find a way to refer to 'id'
                childrenById[buttons[c].objectName] = buttons[c]
                buttons[c].disable()
            }

            // enable requested
            for (var i = 0; i < buttonList.length; i++) {
                childrenById[buttonList[i].toLowerCase()].enable()
            }
        }
    }

    property int defaultButtonWidthMargin: gdisplay.touchCellWidth() / 2
    property int buttonWidthMargin: defaultButtonWidthMargin * buttonScaler

    property int defaultButtonHeightMargin: gdisplay.touchCellHeight() / 2
    property int buttonHeightMargin: defaultButtonHeightMargin * buttonScaler

    property var buttons: [back, ok, up, down, left, right]

    // private
    property real buttonScaler: 1.0

    onVisibleChanged: {
        if (visible)
            calcScaling()
    }

    onMaxHeightChanged: { calcScaling() }
    onMaxWidthChanged: { calcScaling() }

    function calcScaling() {
        if (visible) {
            //console.log("############### CALC BasicControls height")
            // how to make items smaller?
            //  -> decrease margin and buttons proportionally until ok
            //     -> use scaler

            // only if max have been defined
            var buttonScalerCandinate = 1.0
            if (maxHeight !== 0) {
                var heightCandinate = topButtonRow.defaultHeight + biggerButtonArea.defaultHeight * buttonScalerCandinate
                //console.debug("### maxHeight: " + maxHeight + ", heightCandinate: " + heightCandinate)
                if (heightCandinate > maxHeight) {
                    buttonScalerCandinate = maxHeight / heightCandinate
                    console.debug("### buttonScalerCandinate after height calc: " + buttonScaler)
                }

            }

            if (maxWidth !== 0) {
                var widthCandinate = biggerButtonArea.defaultWidth
                if (widthCandinate > maxWidth) {
                    var widthScalerCandinate = maxWidth / widthCandinate
                    if (widthScalerCandinate < buttonScalerCandinate) {
                        buttonScalerCandinate = widthScalerCandinate
                        //console.debug("### buttonScalerCandinate after width calc: " + buttonScaler)
                    }
                }
            }

            buttonScaler = buttonScalerCandinate
        }
    }

    Column {
        anchors.centerIn: parent


        Item {
            id: topButtonRow
            //color: "green"
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.bottom: biggerButtonArea.top
            width: biggerButtonArea.width
            height: back.buttonHeight
            property int defaultHeight: back.defaultButtonHeight


            BasicControlsButton {
                id: back
                width: (buttonTextImplicitWidth + buttonHorizontalMargin) * buttonScaler
                objectName: "back"
                buttonText: "Back"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: (gdisplay.touchCellWidth() / 2) * buttonScaler
                onButtonPressed: basicControls.buttonPressed("Back")
                scalingFactor: buttonScaler
            }
        }

        Item {
            id: biggerButtonArea
            //color: "red"
            //anchors.centerIn: parent
            width: 3 * ok.buttonWidth + 4 * buttonWidthMargin
            height: 3 * ok.buttonHeight + 4 * buttonHeightMargin
            property int defaultHeight: 3 * ok.defaultButtonHeight + 4 * defaultButtonHeightMargin
            property int defaultWidth: 3 * ok.defaultButtonWidth + 4 * defaultButtonWidthMargin

            BasicControlsButton {
                id: ok
                objectName: "ok"
                buttonText: "OK"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                onButtonPressed: basicControls.buttonPressed("OK")
                scalingFactor: buttonScaler
            }

            BasicControlsButton {
                id: up
                objectName: "up"
                buttonText: "U"
                anchors.bottom: ok.top
                anchors.horizontalCenter: ok.horizontalCenter
                anchors.bottomMargin: buttonHeightMargin
                onButtonPressed: basicControls.buttonPressed("Up")
                scalingFactor: buttonScaler
            }

            BasicControlsButton {
                id: right
                objectName: "right"
                buttonText: "R"
                anchors.left: ok.right
                anchors.verticalCenter: ok.verticalCenter
                anchors.leftMargin: buttonWidthMargin
                onButtonPressed: basicControls.buttonPressed("Right")
                scalingFactor: buttonScaler
            }

            BasicControlsButton {
                id: left
                objectName: "left"
                buttonText: "L"
                anchors.right: ok.left
                anchors.verticalCenter: ok.verticalCenter
                anchors.rightMargin: buttonWidthMargin
                onButtonPressed: basicControls.buttonPressed("Left")
                scalingFactor: buttonScaler
            }

            BasicControlsButton {
                id: down
                objectName: "down"
                anchors.top: ok.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: buttonHeightMargin
                buttonText: "D"
                onButtonPressed: basicControls.buttonPressed("Down")
                scalingFactor: buttonScaler
            }
        }
    }

    Component.onCompleted: {
        Log.initLog("BasicControls", gsettings.logLevel)
    }

}
