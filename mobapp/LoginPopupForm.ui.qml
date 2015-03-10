import QtQuick 2.4

Item {
    width: 400
    height: 400

    Text {
        id: userNameLabel
        x: 106
        y: 129
        text: qsTr("User name:")
        font.pixelSize: 12
    }

    TextInput {
        id: userNameInput
        x: 181
        y: 129
        width: 80
        height: 20
        text: "foobar"
        font.pixelSize: 12
    }
}

