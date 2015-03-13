import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1


Rectangle {
    id: root
    width: parent.width - 100
    height: 300 // TODO: other calculation
    anchors.centerIn: parent
    color: "lightblue"

    property string titleText: "Login"

    signal viewClosed()
    signal login(var username, var password, var guest, var rememberPassword)

    Rectangle {
        id: titlebar
        height: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "darkgrey"

        Text {
            text: titleText
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 2
            color: "white"
        }

        Rectangle {
            id: closeButton
            width: 15
            height: 15
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 2
            color: "black"
            // TODO: cross

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug("CLOSED PRESSED")
                    root.viewClosed()
                }
            }
        }
    }

    ColumnLayout {
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: "User name"
            }
            ComboBox {
                id: userNameField
                width: 200
                editable: true
                model: profileModel
            }

            ListModel {
                id: profileModel
                ListElement { text: "Banana"; color: "Yellow" }
                ListElement { text: "Apple"; color: "Green" }
                ListElement { text: "Coconut"; color: "Brown" }
            }
        }

        CheckBox {
            id: guestCheckbox
            text: qsTr("Guest")
            anchors.right: parent.right
            checked: false
            onCheckedChanged: {

            }
        }

        RowLayout {
            id: passwordRow
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: !guestCheckbox.checked

            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                // TODO: how password field
                //displayText: "<password>"
            }
        }

        CheckBox {
            id: rememberPasswordCheckbox
            text: qsTr("Remember password")
            anchors.right: parent.right
            enabled: !guestCheckbox.checked
        }

        Rectangle {
            id: buttonArea
            height: 100 // TODO

            RowLayout {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: qsTr("Login")
                    onClicked: {
                        console.debug("LOGIN PRESSED")
                        login(userNameField.currentText,
                              passwordField.text,
                              guestCheckbox.checked,
                              rememberPasswordCheckbox.checked)
                    }
                }
            }
        }
    }
}


// TODO: where removing of remembered profiles
