import QtQuick 2.0

Rectangle {
    anchors.fill: parent

    border.color: "slategray"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }
}

