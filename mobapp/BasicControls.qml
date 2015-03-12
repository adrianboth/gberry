import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

// TODO: using mousearea (just for history), but didn't realize that may have blocked normal press feedback
//       (button haven't received events)

Rectangle {
    width: 100
    height: 62

    id: basicControls

    // "Up", "Down", "Right", "Left", "OK"
    signal buttonPressed(var buttonID)

    BasicControlsButton {
        id: ok
        buttonText: "OK"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        onButtonPressed: basicControls.buttonPressed("OK")
    }

    BasicControlsButton {
        id: up
        buttonText: "U"
        anchors.bottom: ok.top
        anchors.horizontalCenter: ok.horizontalCenter
        anchors.bottomMargin: 15
        onButtonPressed: basicControls.buttonPressed("Up")
    }

    BasicControlsButton {
        id: right
        buttonText: "R"
        anchors.left: ok.right
        anchors.verticalCenter: ok.verticalCenter
        anchors.leftMargin: 15
        onButtonPressed: basicControls.buttonPressed("Right")
    }

    BasicControlsButton {
        id: left
        buttonText: "L"
        anchors.right: ok.left
        anchors.verticalCenter: ok.verticalCenter
        anchors.rightMargin: 15
        onButtonPressed: basicControls.buttonPressed("Left")
    }

    BasicControlsButton {
        id: down
        anchors.top: ok.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 15
        buttonText: "D"
        onButtonPressed: basicControls.buttonPressed("Down")
    }

}

