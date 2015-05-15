import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
//import "ViewTitleBar.qml" as ViewTitleBar
import ".."
// TODO: how to get scaling combo and edit box

Item {
    id: self
    anchors.fill: parent

    signal viewClosed()
    signal login()

    onVisibleChanged: {
        if (self.visible) {
            userNameField.editText = UserModel.currentUserName
            guestCheckbox.checked = UserModel.currentIsGuest
            if (!guestCheckbox.checked) {
                passwordField.text = UserModel.currentPassword
            }
            rememberPasswordCheckbox.checked = UserModel.currentIsRememberPassword
        }
    }

    Rectangle {
        id: view
        anchors.centerIn: parent
        anchors.margins: border.width

        //width: Math.min(root.width - gdisplay.touchCellHeight(), mainColumn.width) + gdisplay.touchCellWidth()
        //height: mainColumn.height //+ column.anchors.margins

        width: mainColumn.width
        height: mainColumn.height
        radius: 10
        color: "snow"
        //color: "red" // for debug
        border.width: 1
        border.color: "lightgray"

        ColumnLayout {
            id: mainColumn
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0


            ViewTitleBar {
                id: titleBar
                titleText: qsTr("Login")
                Layout.fillWidth: true
                Layout.preferredHeight: preferredHeight

                onCloseSelected: viewClosed()
            }

            // divider between titlebar and other content
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                color: "darkgrey"
            }

            Item { // for margins
                id: content
                //color: "green"
                Layout.preferredWidth: column.width + gdisplay.touchCellHeight()
                Layout.preferredHeight: column.height + column.anchors.topMargin
                Layout.alignment: Qt.AlignHCenter

                ColumnLayout {
                    id: column
                    // there is fixed width&height
                    anchors.top: parent.top
                    anchors.topMargin: gdisplay.touchCellHeight() / 2
                    anchors.horizontalCenter: parent.horizontalCenter

                    //anchors.margins: gdisplay.smallSize * gdisplay.ppmText

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

                    Item {
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
