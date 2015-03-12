import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2


Button {
    id: button

    signal buttonPressed()

    property string buttonGradientStartColor: "#eee"
    property string buttonGradientStopColor: "#ccc"
    property string buttonText: "-" // default

    width: 50 // TODO: how to adjust to text size
    height: 50

    style: ButtonStyle {
        background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    border.width: control.activeFocus ? 2 : 1
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
        text: button.buttonText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 16
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
