import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import GBerry 1.0

// TODO: using mousearea (just for history), but didn't realize that may have blocked normal press feedback
//       (button haven't received events)

Rectangle {
    id: basicControls

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
            for (var c = 0; c < children.lenght; c++) {
                children[c].enable()
            }
            ok.enable(); up.enable(); down.enable(); right.enable(); left.enable()

        } else {
            // only selected are enabled
            Log.debug("Enabling: " + buttonList.toString())
            var childrenById = {}
            for (var c = 0; c < children.length; c++) {
                // couldn't find a way to refer to 'id'
                childrenById[children[c].objectName] = children[c]
                children[c].disable()
            }

            for (var i = 0; i < buttonList.length; i++) {
                childrenById[buttonList[i].toLowerCase()].enable()
            }
        }
    }

    BasicControlsButton {
        id: ok
        objectName: "ok"
        buttonText: "OK"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        onButtonPressed: basicControls.buttonPressed("OK")
    }

    BasicControlsButton {
        id: up
        objectName: "up"
        buttonText: "U"
        anchors.bottom: ok.top
        anchors.horizontalCenter: ok.horizontalCenter
        anchors.bottomMargin: 15
        onButtonPressed: basicControls.buttonPressed("Up")
    }

    BasicControlsButton {
        id: right
        objectName: "right"
        buttonText: "R"
        anchors.left: ok.right
        anchors.verticalCenter: ok.verticalCenter
        anchors.leftMargin: 15
        onButtonPressed: basicControls.buttonPressed("Right")
    }

    BasicControlsButton {
        id: left
        objectName: "left"
        buttonText: "L"
        anchors.right: ok.left
        anchors.verticalCenter: ok.verticalCenter
        anchors.rightMargin: 15
        onButtonPressed: basicControls.buttonPressed("Left")
    }

    BasicControlsButton {
        id: down
        objectName: "down"
        anchors.top: ok.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 15
        buttonText: "D"
        onButtonPressed: basicControls.buttonPressed("Down")
    }

    Component.onCompleted: {
        Log.initLog("BasicControls", gsettings.logLevel)
    }

}

