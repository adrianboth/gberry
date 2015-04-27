import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0

// TODO: how to get scaling combo and edit box

Rectangle {
    id: root
    width: Math.min(parent.width - gdisplay.touchCellHeight(), column.width) + gdisplay.touchCellWidth()
    //width: column.width
    //height: 4*60 // TODO: other calculation
    height: titlebar.height + column.height + column.anchors.margins //* 2// + gdisplay.touchCellHeight()// + divider.height + buttonArea.height
    anchors.centerIn: parent
    //color: "green"
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

            // TODO: cross
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
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.centerIn: parent

        anchors.margins: gdisplay.smallSize * gdisplay.ppmText
        //anchors.topMargin: gdisplay.smallSize * gdisplay.ppmText

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
                //anchors.right: parent.right
                //anchors.left: userNameRect.right

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
                //anchors.right: parent.right
                checked: false
                onCheckedChanged: {

                }
            }
        }

        RowLayout {
            id: passwordRow
            Layout.fillWidth: true
            Layout.preferredHeight: column.rowHeight
            spacing: gdisplay.touchCellWidth() / 2

            //anchors.left: parent.left
            //anchors.right: parent.right
            enabled: !guestCheckbox.checked

            Rectangle {
                id: passwordRect
                //color: "lightblue"
                Layout.preferredHeight: passwordLabel.implicitHeight
                Layout.preferredWidth: column.labelColumnWidth

                Label {
                    id: passwordLabel
                    text: qsTr("Password")
                }
            }

            // TODO: Problems to get TextInput (has password mode) to work (background)
            TextField {
                id: passwordField
                //anchors.right: parent.right
                //anchors.left: passwordRect.right
                Layout.preferredHeight: passwordField.implicitHeight
                Layout.fillWidth: true
                echoMode: TextInput.Password

                /*
                color: "red"
                border.width: 1
                border.color: "gray"
                */


                /*
                TextField {
                    //anchors.horizontalCenter: parent.horizontalCenter
                    anchors.fill: parent
                    text: "test"
                    echoMode: TextInput.Password


                }*/
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
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.bottom: buttonArea.top
            Layout.preferredWidth: parent.width - gdisplay.touchCellWidth()
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            id: buttonArea
            Layout.preferredHeight: loginButton.height + gdisplay.touchCellHeight() /2
            Layout.fillWidth: true
            //anchors.left: parent.left
            //anchors.right: parent.right
            //anchors.bottom: parent.bottom

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

        console.debug("column height: " + column.height)
        console.debug("column implicit height: " + column.implicitHeight)
    }
}


// TODO: where removing of remembered profiles
