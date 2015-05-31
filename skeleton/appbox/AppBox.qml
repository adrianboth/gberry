import QtQuick 2.0

// TODO: create button that sends data back to mainui

Rectangle {
    color: "lightgrey"
    border.width: 1
    border.color: "black"
    width: 200
    height: 50
    anchors.centerIn: parent
    Text {
      text: "CONTENT FROM MAINUI"
      anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: outgoingMessage("hello mainui")
    }

    // -- API --

    signal outgoingMessage(var data)

    function incomingMessage(data) {
        console.debug("### MAINUI APPBOX MSG: " + data)
        outgoingMessage("got appbox msg")
    }
}
