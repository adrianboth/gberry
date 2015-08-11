/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
//import "ViewTitleBar.qml" as ViewTitleBar
import ".."
// TODO: how to get scaling combo and edit box

import "../settings/SettingsModel.js" as SettingsModel

/*
  This views makes possible for user
   - select user profile
   - edit/create user profile (TODO: now simple as no online registration)

  Logic:
   a) user can enter all data  / edit currently visible
      - when close pressed: no login but data is remembered
      - when login is pressed: data is saved and login is made
   b) if user selects other profile from dropbox
      - currently edited data is saved
      - new data for profile is shown

*/
Item {
    id: self
    anchors.fill: parent

    signal viewClosed()
    signal login(string userName)
    signal logout(string userName)

    property bool currentUserDirty: false

    onVisibleChanged: {
        if (self.visible) {
            refreshComboboxModel()

            var index = userNameField.find(UserModel.currentUserName)

            if (UserModel.currentUserIsActive && index > -1) {
                userNameField.currentIndex = index
                refreshFields(index)

            } else {
                // zero is "no selection"
                userNameField.currentIndex = 0
                clearFields()
            }
        }
    }

    Rectangle {
        id: view
        anchors.centerIn: parent
        anchors.margins: border.width

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

                onCloseSelected: onClose()
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
                Layout.preferredWidth: column.width + gdisplay.touchCellWidth()
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
                    // we need to recalculate width of view to avoid binding loops
                    // (NOTE: not counting padding itself when calculating real width: just too complex)
                    property int firstColumPadding: Math.max(0, Math.min((root.width - fullViewWidth) * 0.10, 1.5* maxLabelWidth))
                    property int secondColumnPadding: Math.max(0, Math.min((root.width - fullViewWidth) * 0.10, 1.5* rememberPasswordCheckbox.implicitWidth))

                    // formulate: 1st column, 2nd column, view margins, spacing
                    property int fullViewWidth: maxLabelWidth + rememberPasswordCheckbox.implicitWidth + gdisplay.touchCellWidth() + columnSpacing
                    property int maxLabelWidth: Math.max(userNameLabel.implicitWidth, passwordLabel.implicitWidth)

                    // this real width that should be used
                    property int labelColumnWidth: Math.max(userNameLabel.implicitWidth, passwordLabel.implicitWidth) + firstColumPadding// + root.width / 10//+ gdisplay.touchCellWidth()//100 // TODO: somekind of adjustment

                    // each row should have same height
                    property int rowHeight: userNameField.height

                    property int columnSpacing: 5 //gdisplay.touchCellWidth() / 2

                    //onFirstColumPaddingChanged: debugCalc()

                    function debugCalc() {
                        console.debug("userNameLabel.implicitWidth:" + userNameLabel.implicitWidth)
                        console.debug("emailLabel.implicitWidth:" + emailLabel.implicitWidth)
                        console.debug("passwordLabel.implicitWidth:" + passwordLabel.implicitWidth)
                        console.debug("rememberPasswordCheckbox.implicitWidth:" + rememberPasswordCheckbox.implicitWidth)
                        console.debug("gdisplay.touchCellWidth()" + gdisplay.touchCellWidth())
                        console.debug("fullViewWidth: " + fullViewWidth)
                        console.debug("root.width: " + root.width)
                        console.debug("firstColumPadding: " + firstColumPadding)
                        console.debug("secondColumnPadding: " + secondColumnPadding)
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: column.rowHeight
                        spacing: column.columnSpacing

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

                            property bool validSelection: currentIndex !== 0
                            property bool invalidSelection: currentIndex === 0

                            // user selected a profile
                            onActivated: {
                                // currentIndex still contains old index
                                saveCurrent(currentIndex)
                            }

                            // on new selection update all fields on dialog
                            onCurrentIndexChanged: {
                                console.debug("User name currentIndex changed: " + currentIndex)
                                // first there comes event with -1 as model doesn't have entry
                                //  -> skip it
                                if (currentIndex > -1) {

                                    var item = profileModel.get(currentIndex)
                                    refreshFields(currentIndex)
                                }
                            }

                            onFocusChanged: {
                                console.debug("on pressed")
                                if (currentIndex === 0) {
                                    // on edit clear default text
                                    editText = ""
                                }
                            }

                            // new user name entered via editing
                            onAccepted: {
                                 if (currentText === "") {
                                     currentIndex = 0

                                 }

                                 // is this new user?
                                  else if (find(currentText) === -1) {
                                     model.append({text: editText})
                                     // append goes always as last
                                     saveCurrent(model.count - 1)
                                     currentIndex = find(editText)
                                 } else {

                                     // existing user (written)
                                     //  -> look other data of that user

                                     currentIndex = find(editText)
                                     // does this trigger activated or should we update by our selves
                                 }

                             }

                            /*
                            onEditTextChanged: {
                                loginButton.updateState()
                            }
                            */

                            ListModel {
                                id: profileModel
                                // very initial data to avoid creation time errors
                                ListElement {text: "foobar"; email: "foo@mail"; password: ""; guest: false; rememberPassword: false}
                            }
                        }

                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: column.rowHeight
                        spacing: column.columnSpacing


                        // empty slot
                        Item {
                            Layout.preferredWidth: column.labelColumnWidth
                        }

                        CheckBox {
                            id: guestCheckbox
                            text: qsTr("Guest")
                            enabled: userNameField.validSelection
                            checked: false
                            onCheckedChanged: {
                                if (UserModel.currentUserName == userNameField.currentText)
                                    self.currentUserDirty = true
                            }
                        }
                    }

                    RowLayout {
                        id: emailRow
                        Layout.fillWidth: true
                        Layout.preferredHeight: column.rowHeight
                        spacing: column.columnSpacing

                        enabled: !guestCheckbox.checked && userNameField.validSelection

                        Rectangle {
                            id: emailRect
                            //color: "lightblue" // for debugging
                            Layout.preferredHeight: emailLabel.implicitHeight
                            Layout.preferredWidth: column.labelColumnWidth

                            Label {
                                id: emailLabel
                                text: qsTr("Email")
                            }
                        }

                        TextField {
                            id: emailField
                            Layout.preferredHeight: emailField.implicitHeight
                            Layout.fillWidth: true

                            onTextChanged: {
                                if (UserModel.currentUserName == userNameField.currentText)
                                    self.currentUserDirty = true
                            }
                        }
                    }

                    RowLayout {
                        id: passwordRow
                        Layout.fillWidth: true
                        Layout.preferredHeight: column.rowHeight
                        spacing: column.columnSpacing

                        enabled: !guestCheckbox.checked && userNameField.validSelection

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

                            onTextChanged: {
                                if (UserModel.currentUserName == userNameField.currentText)
                                    self.currentUserDirty = true
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: column.rowHeight
                        spacing: column.columnSpacing


                        // empty slot
                        Item {
                            Layout.preferredWidth: column.labelColumnWidth
                        }

                        CheckBox {
                            id: rememberPasswordCheckbox
                            // this is longest text on second column (under text fields)
                            // so this defines the width of second column
                            Layout.preferredWidth: implicitWidth + column.secondColumnPadding
                            text: qsTr("Remember password")
                            //anchors.right: parent.right
                            enabled: !guestCheckbox.checked && userNameField.validSelection
                        }
                    }

                    Rectangle {
                        id: divider
                        Layout.preferredHeight: 2
                        color: "gray"
                        Layout.preferredWidth: parent.width - gdisplay.touchCellWidth() / 2
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Item {
                        id: buttonArea
                        Layout.preferredHeight: loginButton.height + gdisplay.touchCellHeight() /2
                        Layout.fillWidth: true


                        Item {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: loginButton.left
                            anchors.rightMargin: gdisplay.touchCellWidth() / 3

                            width: signUpLink.implicitWidth * 1.2
                            height: signUpLink.implicitHeight * 1.2

                            Text {
                                id: signUpLink
                                color: "blue"
                                font.pixelSize: gdisplay.smallSizeText * 0.7
                                font.underline: true
                                text: qsTr("Sign up")
                                anchors.centerIn: parent
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    var url = "http://" + SettingsModel.serverAddress() +"/idm/users/sign_up"
                                    console.debug("### sign up clicked!")
                                    Qt.openUrlExternally(url)
                                }
                            }
                        }
                        /*
                        Item {
                            id: middle
                            anchors.centerIn: parent
                            width: gdisplay.touchCellWidth() / 4
                            height: gdisplay.touchCellHeight()
                        }
                        */

                        GButton {
                            id: loginButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: +gdisplay.touchCellWidth()/2
                            // login possible when:
                            //   a) current user is not active

                            property bool loginModeEnabled: !LoginModel.isLoggedIn && ((UserModel.currentUserName != userNameField.currentText) || self.currentUserDirty)
                            label: {
                                if (loginModeEnabled) {
                                    return qsTr("Login")
                                } else {
                                    return qsTr("Logout")
                                }
                            }
                            labelTextPixelSize: gdisplay.smallSizeText


                            enabled: (userNameField.editText.length > 0 && userNameField.validSelection) || (UserModel.currentUserIsActive && userNameField.invalidSelection)
                            height: buttonHeight
                            width: buttonWidth
                            anchors.verticalCenter: parent.verticalCenter
                            //anchors.left: middle.right
                            onButtonClicked: {
                                var userName = profileModel.get(userNameField.currentIndex).text
                                if (loginModeEnabled) {
                                    saveCurrent(userNameField.currentIndex)
                                    login(userName)
                                } else {
                                    logout(userName)
                                }
                            }
                        }
                    }
                }
            }
        }
    }



    function refreshComboboxModel() {
        var users = UserModel.userNames()
        profileModel.clear() // remove all previous
        // "no selection" is always first
        profileModel.append({ text: "<no selection>",
                              email: "",
                              password: "",
                              guest: false,
                              rememberPassword: false })
        for (var i = 0; i < users.length; i++) {
            profileModel.append({text: users[i],
                                 email: UserModel.email(users[i]),
                                 password: UserModel.password(users[i]),
                                 guest: UserModel.isGuest(users[i]),
                                 rememberPassword: UserModel.isRememberPassword(users[i])
                                })
        }
    }

    function saveCurrent(index) {
        if (index === 0) {
            console.debug("Not saving <no selection>")
            return
        }

        var newData = {text: userNameField.editText,
                       email: emailField.text,
                       password: passwordField.text,
                       guest: guestCheckbox.checked,
                       rememberPassword: rememberPasswordCheckbox.checked}

        profileModel.set(index, newData)
        UserModel.setUser(newData.text, newData.email, newData.password, newData.guest, newData.rememberPassword)
        self.currentUserDirty = false
    }

    function refreshFields(index) {
        var item = profileModel.get(index)
        emailField.text = item.email
        guestCheckbox.checked = item.guest
        rememberPasswordCheckbox.checked = item.rememberPassword

        if (item.rememberPassword) {
            passwordField.text = item.password
        } else {
            passwordField.text = ""
        }

        self.currentUserDirty = false
        console.debug("User name changed, guest: " + item.guest)
    }

    function clearFields() {
        emailField.text = ""
        passwordField.text = ""
        guestCheckbox.text = ""
        rememberPasswordCheckbox.checked = true
    }

    // actions when view is about to close
    function onClose() {
        /*
        if (userNameField.currentIndex !== 0) {
            saveCurrent(userNameField.currentIndex)
        }
        */
        viewClosed() // signal
    }

    Component.onCompleted: {
        /*
        userNameField.currentIndex = 0
        var item = profileModel.get(userNameField.currentIndex)
        passwordField.text = item.password
        guestCheckbox.checked = item.guest
        rememberPasswordCheckbox.checked = item.rememberPassword
        */
        //column.debugCalc() // debug
    }
}


// TODO: where removing of remembered profiles
