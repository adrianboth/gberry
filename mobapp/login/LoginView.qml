import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1


Rectangle {
    id: root
    width: parent.width - 50
    height: 4*60 // TODO: other calculation
    anchors.centerIn: parent
    color: "lightblue"

    property string titleText: "Login"

    signal viewClosed()
    //signal login(var username, var password, var guest, var rememberPassword)
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
        height: titleTextLabel.implicitHeight + 4
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "darkgrey"

        Text {
            id: titleTextLabel
            text: titleText
            font.pointSize: 14
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 2
            color: "white"
        }

        Rectangle {
            id: closeButton
            width: closeButton.height
            height: parent.height - 6
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
                    // TODO: we are not storing possibly changed values
                }
            }
        }
    }

    ColumnLayout {
        id: column

        anchors.top: titlebar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        property int labelColumnWidth: 100 // TODO: somekind of adjustment

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            // adding rectable as label itself doesn't take width
            Rectangle {
                id: userNameRect
                //color: "yellow"
                color: "lightblue"
                height: userNameLabel.implicitHeight
                width: column.labelColumnWidth

                Label {
                    id: userNameLabel
                    text: "User name"
                    width: 200; //column.labelColumnWidth
                }
            }

            ComboBox {
                id: userNameField
                anchors.right: parent.right
                anchors.left: userNameRect.right

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
            Item {
                width: column.labelColumnWidth
            }

            CheckBox {
                id: guestCheckbox
                text: qsTr("Guest")
                anchors.right: parent.right
                checked: false
                onCheckedChanged: {

                }
            }
        }

        RowLayout {
            id: passwordRow
            //anchors.left: parent.left
            //anchors.right: parent.right
            enabled: !guestCheckbox.checked

            Rectangle {
                id: passwordRect
                //color: "yellow"
                color: "lightblue"
                height: passwordLabel.implicitHeight
                width: column.labelColumnWidth

                Label {
                    id: passwordLabel
                    text: "Password"
                }
            }

            // TODO: Problems to get TextInput (has password mode) to work (background)
            TextField {
                id: passwordField
                anchors.right: parent.right
                anchors.left: passwordRect.right
                height: passwordLabel.implicitHeight
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
            Item {
                width: column.labelColumnWidth
            }

            CheckBox {
                id: rememberPasswordCheckbox
                text: qsTr("Remember password")
                anchors.right: parent.right
                enabled: !guestCheckbox.checked
            }
        }
    }

    Rectangle {
        id: buttonArea
        height: loginButton.implicitHeight + 2*5 // + margin
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "steelblue"

        RowLayout {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 5

            Button {
                id: loginButton
                text: qsTr("Login")
                enabled: false
                onClicked: {
                    console.debug("LOGIN PRESSED")

                    // TODO: set current userinfo
                    UserModel.setCurrent(userNameField.editText,
                                         passwordField.text,
                                         guestCheckbox.checked,
                                         rememberPasswordCheckbox.checked)

                    /*
                    login(userNameField.editText,
                          passwordField.text,
                          guestCheckbox.checked,
                          rememberPasswordCheckbox.checked)
                    */
                    login()
                }
                function updateState() {
                    console.debug("CONDITION " + userNameField.editText.length > 0)
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
