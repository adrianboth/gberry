import QtQuick 2.0

// TODO: create button that sends data back to mainui

Rectangle {
    color: "lightgrey"
    border.width: 1
    border.color: "black"
    anchors.centerIn: parent
    anchors.fill: parent
    Text {
      text: "Please wait ..."
      anchors.centerIn: parent
    }



    // -- API --

    signal outgoingMessage(var data)

    function incomingMessage(data) {
        console.debug("### MAINUI APPBOX MSG: " + data)
        outgoingMessage("got appbox msg")
    }
}
