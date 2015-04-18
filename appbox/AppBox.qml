import QtQuick 2.0
import QtQuick.Layouts 1.1

// TODO: create button that sends data back to mainui

Rectangle {
    id: self
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

    ColumnLayout {
        anchors.centerIn: parent

        RowLayout {
            Rectangle {
                id: button1
                property int number: 1
                // TODO: make physically square
                width: buttonText1.implicitHeight
                height: buttonText1.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText1
                    text: button1.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button1.number)
                    }
                }
            }
            Rectangle {
                id: button2
                property int number: 2
                // TODO: make physically square
                width: buttonText2.implicitHeight
                height: buttonText2.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText2
                    text: button2.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button2.number)
                    }
                }
            }
            Rectangle {
                id: button3
                property int number: 3
                // TODO: make physically square
                width: buttonText3.implicitHeight
                height: buttonText3.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText3
                    text: button3.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button3.number)
                    }
                }
            }
        }

        RowLayout {
            Rectangle {
                id: button4
                property int number: 4
                // TODO: make physically square
                width: buttonText4.implicitHeight
                height: buttonText4.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText4
                    text: button4.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button4.number)
                    }
                }
            }
            Rectangle {
                id: button5
                property int number: 5
                // TODO: make physically square
                width: buttonText5.implicitHeight
                height: buttonText5.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText5
                    text: button5.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button5.number)
                    }
                }
            }
            Rectangle {
                id: button6
                property int number: 6
                // TODO: make physically square
                width: buttonText6.implicitHeight
                height: buttonText6.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText6
                    text: button6.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button6.number)
                    }
                }
            }
        }
        RowLayout {
            Rectangle {
                id: button7
                property int number: 7
                // TODO: make physically square
                width: buttonText7.implicitHeight
                height: buttonText7.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText7
                    text: button7.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button7.number)
                    }
                }
            }
            Rectangle {
                id: button8
                property int number: 8
                // TODO: make physically square
                width: buttonText8.implicitHeight
                height: buttonText8.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText8
                    text: button8.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button8.number)
                    }
                }
            }
            Rectangle {
                id: button9
                property int number: 9
                // TODO: make physically square
                width: buttonText9.implicitHeight
                height: buttonText9.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText9
                    text: button9.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button9.number)
                    }
                }
            }
        }
        RowLayout {
            Rectangle {
                id: button0
                property int number: 0
                // TODO: make physically square
                width: buttonText0.implicitHeight
                height: buttonText0.implicitHeight
                color: "slategray"
                border.color: "darkgrey"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    id: buttonText0
                    text: button0.number.toString()
                    // TODO: how to get display size through
                    font.pixelSize: 36
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        buttonPressed(button0.number)
                    }
                }
            }
        }

    }

    // -- API --

    // Outgoing AppBoxMessages
    //   data: json object
    signal outgoingMessage(var data)

    property bool enabled: false

    // Incoming AppBoxMessages
    //   js: json object
    function incomingMessage(js) {
        console.debug("### MAINUI APPBOX MSG: " + js)

        //outgoingMessage("got appbox msg")

        if (!"action" in js) {
            console.error("Unknown message format: " + js)
            return
        }

        if (js["action"] === "DisableControls") {
            self.color = "green"
            self.enabled = false
        } else if (js["action"] === "EnableControls") {
            self.color = "blue"
            self.enabled = true
        }
    }

    function buttonPressed(number) {
        console.debug("Button pressed: " + number.toString())

        if (self.enabled) {
            // TODO: send
            var js = {action: "NumberPressed", number: number.toString()}
            console.debug("Sending message out from AppBox")
            outgoingMessage(js)
        }
    }
}

// TODO: try to use components defined in a pkg
