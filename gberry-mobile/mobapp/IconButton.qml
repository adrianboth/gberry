import QtQuick 2.4

Item {
    id: self
    //color: "yellow"
    property url imageSource: ""
    property real imageSizeRatio: 0.9
    property color backgroundColor: "black"
    property real backgroundOpacity: 0

    property int preferredHeight: targetHeight !== 0 ? targetHeight : image.implicitHeight
    property int preferredWidth: targetWidth !== 0 ? targetWidth : image.implicitWidth

    property int targetHeight: 0
    property int targetWidth: 0

    property int rotation: 0

    onTargetHeightChanged: {
        if (targetHeight !== 0)
            image.sourceSize.height = targetHeight * imageSizeRatio
    }

    signal buttonClicked()

    Rectangle {
        id: imageBackground
        color: backgroundColor
        opacity: backgroundOpacity
        height: image.paintedHeight / imageSizeRatio
        width: image.paintedWidth / imageSizeRatio
        anchors.centerIn: parent
    }

    Image {
        visible: true
        id: image
        anchors.centerIn: parent
        source: imageSource
        fillMode: Image.PreserveAspectFit
        rotation: self.rotation
        opacity: self.enabled ? 1 : 0.5
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (self.enabled) {
                image.opacity = 0.5
                imageBackground.opacity = backgroundOpacity + 0.1
                buttonFeedbackTimer.running = true
                buttonClicked()
            }
        }
    }
    Timer {
        id: buttonFeedbackTimer
        running: false; repeat: false; interval: 200
        onTriggered: {
            image.opacity = 1
            imageBackground.opacity = backgroundOpacity
        }
    }
}
