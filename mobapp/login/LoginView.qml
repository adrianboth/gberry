import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0

// TODO: how to get scaling combo and edit box

Rectangle {
    id: root
    width: Math.min(parent.width - gdisplay.touchCellHeight(), column.width) + gdisplay.touchCellWidth()
    height: titlebar.height + column.height + column.anchors.margins
    anchors.centerIn: parent
    //color: "green" // for debug
    border.width: 1
    border.color: "lightgray"

    property string titleText: qsTr("Login")

    signal viewClosed()
    signal login()

    onVisibleChanged: {
        if (root.visible) {
            userNameField.editText = UserModel.currentUserName
            guestCheckbox.checked = UserModel.currentIsGuest
            if (!guestCheckbox.checked) {
                passwordField.text = UserModel.currentPassword
            }
            rememberPasswordCheckbox.checked = UserModel.currentIsRememberPassword
        }
    }

    Rectangle {
        id: titlebar
        height: titleTextLabel.implicitHeight * 1.5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        color: "darkgrey"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "gray" }
            GradientStop { position: 1.0; color: "snow" }
        }

        border.width: 1
        border.color: "lightgray"

        Text {
            id: titleTextLabel
            text: titleText
            font.pointSize: gdisplay.smallSize * gdisplay.ppmText
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: font.pointSize
            color: "black"
        }

        Rectangle {
            id: closeButton
            width: closeButton.height
            height: parent.height * 0.65
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: parent.height * 0.25
            radius: 50
            border.width: 1
            border.color: "grey"
            color: "lightgrey"

            Canvas {
                id: cross
                // canvas size
                width: parent.width; height: parent.height
                // handler to override for drawing
                property real crossLineMarginX: 0.3 * cross.width
                property real crossLineMarginY: 0.3 * cross.height

                onPaint: {
                    // get context to draw with
                    var ctx = getContext("2d")
                    // setup the stroke
                    ctx.lineWidth = 2
                    ctx.strokeStyle = "black"
                    // setup the fill
                    ctx.fillStyle = "steelblue"
                    // begin a new path to draw
                    ctx.beginPath()

                    ctx.moveTo(0 + crossLineMarginX, 0 + crossLineMarginY)
                    ctx.lineTo(cross.width - crossLineMarginX, cross.height - crossLineMarginY)
                    ctx.stroke()

                    ctx.beginPath()
                    ctx.moveTo(cross.width - crossLineMarginX, 0 + crossLineMarginY)
                    ctx.lineTo(0 + crossLineMarginX, cross.height - crossLineMarginY)
                    ctx.stroke()
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.viewClosed()
                    // TODO: we are not storing possibly changed values
                }
            }
        }
    }

    ColumnLayout {
        id: column

        anchors.top: titlebar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: gdisplay.smallSize * gdisplay.ppmText

        // width to have all labels same size (filled with empty)
        property int labelColumnWidth: Math.max(userNameLabel.implicitWidth, passwordLabel.implicitWidth) + gdisplay.touchCellWidth()//100 // TODO: somekind of adjustment

        // each row should have same height
        property int rowHeight: userNameField.height

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: column.rowHeight
            spacing: gdisplay.touchCellWidth() / 2

            // adding rectable as label itself doesn't take width
            Rectangle {
                id: userNameRect
                //color: "lightblue"
                Layout.preferredHeight: userNameLabel.implicitHeight
                Layout.preferredWidth: column.labelColumnWidth

                Label {
                    id: userNameLabel
                    text: qsTr("User name")
                }
            }

            ComboBox {
                id: userNameField
                Layout.fillWidth: true

                editable: true
                model: profileModel

                onActivated: {
                    var item = profileModel.get(index)
                    passwordField.text = item.password
                    guestCheckbox.checked = item.guest
                    rememberPasswordCheckbox.checked = item.rememberPassword
                    loginButton.updateState()
                }

                onAccepted: {
                     if (find(currentText) === -1) {
                         model.append({text: editText})
                         currentIndex = find(editText)
                     }
                 }

                onEditTextChanged: {
                    loginButton.updateState()
                }
            }

            ListModel {
                id: profileModel
                // test data
                ListElement { text: "Banana"; password: "Yellow"; guest: false; rememberPassword: true }
                ListElement { text: "Apple"; password: ""; guest: true }
                ListElement { text: "Coconut"; password: ""; guest: false; rememberPassword: false }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: column.rowHeight
            spacing: gdisplay.touchCellWidth() / 2

            // empty slot
            Item {
                Layout.preferredWidth: column.labelColumnWidth
            }

            CheckBox {
                id: guestCheckbox
                text: qsTr("Guest")
                checked: false
                onCheckedChanged: {
                    // TODO
                }
            }
        }

        RowLayout {
            id: passwordRow
            Layout.fillWidth: true
            Layout.preferredHeight: column.rowHeight
            spacing: gdisplay.touchCellWidth() / 2
            enabled: !guestCheckbox.checked

            Rectangle {
                id: passwordRect
                //color: "lightblue" // for debugging
                Layout.preferredHeight: passwordLabel.implicitHeight
                Layout.preferredWidth: column.labelColumnWidth

                Label {
                    id: passwordLabel
                    text: qsTr("Password")
                }
            }

            TextField {
                id: passwordField
                Layout.preferredHeight: passwordField.implicitHeight
                Layout.fillWidth: true
                echoMode: TextInput.Password
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: column.rowHeight
            spacing: gdisplay.touchCellWidth() / 2

            // empty slot
            Item {
                Layout.preferredWidth: column.labelColumnWidth
            }

            CheckBox {
                id: rememberPasswordCheckbox
                text: qsTr("Remember password")
                //anchors.right: parent.right
                enabled: !guestCheckbox.checked
            }
        }

        Rectangle {
            id: divider
            Layout.preferredHeight: 2
            color: "gray"
            Layout.preferredWidth: parent.width - gdisplay.touchCellWidth()
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            id: buttonArea
            Layout.preferredHeight: loginButton.height + gdisplay.touchCellHeight() /2
            Layout.fillWidth: true

            GButton {
                id: loginButton
                label: qsTr("Login")
                enabled: false
                height: buttonHeight
                width: buttonWidth
                anchors.centerIn: parent
                onButtonClicked: {
                    UserModel.setCurrent(userNameField.editText,
                                         passwordField.text,
                                         guestCheckbox.checked,
                                         rememberPasswordCheckbox.checked)
                    login()
                }

                function updateState() {
                    loginButton.enabled = userNameField.editText.length > 0
                }
            }
        }
    }

    Component.onCompleted: {
        userNameField.currentIndex = 0
        var item = profileModel.get(userNameField.currentIndex)
        passwordField.text = item.password
        guestCheckbox.checked = item.guest
        rememberPasswordCheckbox.checked = item.rememberPassword

        loginButton.updateState()
    }
}


// TODO: where removing of remembered profiles
