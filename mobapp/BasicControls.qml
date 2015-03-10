import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    width: 100
    height: 62

    signal upPressed()
    signal leftPressed()
    signal rightPressed()
    signal downPressed()
    signal okPressed()

    Button {
        id: ok
        width: 50 // TODO: how to adjust to text size
        height:50
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: "OK"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                okPressed()
            }
        }
    }

    Button {
        id: up
        property alias control: up
        property string buttonGradientStartColor: "#eee"
        property string buttonGradientStopColor: "#ccc"

        //GradientStop { position: 0 ; color: up.pressed ? "#ccc" : "#eee" }
        //GradientStop { position: 1 ; color: up.pressed ? "#aaa" : "#ccc" }
        width: 50 // TODO: how to adjust to text size
        height: 50
        anchors.bottom: ok.top
        anchors.horizontalCenter: ok.horizontalCenter
        anchors.bottomMargin: 15
        style: ButtonStyle {
            background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 25
                        border.width: control.activeFocus ? 2 : 1
                        border.color: "#888"
                        radius: 4
                        gradient: Gradient {
                            GradientStop { position: 0 ; color: up.buttonGradientStartColor }
                            GradientStop { position: 1 ; color: up.buttonGradientStopColor }
                        }
                    }
        }
        //GradientStop { position: 0 ; color: up.pressed ? "#ccc" : "#eee" }
        //GradientStop { position: 1 ; color: up.pressed ? "#aaa" : "#ccc" }
        Text {
            text: "U"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                upPressed()
                up.buttonGradientStartColor = "#ccc"
                up.buttonGradientStopColor = "#aaa"
                upPressedTimer.running = true
            }
        }
        Timer {
            id: upPressedTimer
            interval: 100
            running: false
            repeat: false
            onTriggered: { up.buttonGradientStartColor = "#eee"
                           up.buttonGradientStopColor = "#ccc"
                         }
        }

        /*
        style: ButtonStyle {
            background: Rectangle {

            }
        }*/

        // TODO: press animation (feedback)

    }

    Button {
        id: right
        width: 50 // TODO: how to adjust to text size
        height:50
        anchors.left: ok.right
        anchors.verticalCenter: ok.verticalCenter
        anchors.leftMargin: 15

        Text {
            text: "R"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rightPressed()
            }
        }
    }

    Button {
        id: left
        width: 50 // TODO: how to adjust to text size
        height:50
        anchors.right: ok.left
        anchors.verticalCenter: ok.verticalCenter
        anchors.rightMargin: 15

        Text {
            text: "L"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                leftPressed()
            }
        }

    }

    Button {
        id: down
        width: 50 // TODO: how to adjust to text size
        height:50
        anchors.top: ok.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 15

        Text {
            text: "D"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                downPressed()
            }
        }
    }

}

