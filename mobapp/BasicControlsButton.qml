import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2


Button {
    id: button

    signal buttonPressed()

    property string buttonGradientStartColor: "#eee"
    property string buttonGradientStopColor: "#ccc"
    property string buttonText: "-" // default

    function enable() {
        button.enabled = true
        button.opacity = 1.0
    }
    function disable() {
        button.enabled = false
        button.opacity = 0.5
    }

    width: gdisplay.largeSize * gdisplay.ppmX // TODO: how to adjust to text size
    height: gdisplay.largeSize * gdisplay.ppmY

    style: ButtonStyle {
        background: Rectangle {
                    //border.width: button.enabled ? 2 : 1
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
