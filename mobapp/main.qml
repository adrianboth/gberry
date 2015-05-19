import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0

import "settings"
import "login"

import "js/MobileClientMessages.js" as Messages
import "js/AppBox.js" as AppBox
import "settings/SettingsModel.js" as SettingsModel

Window {
    id: root
    visible: true
    width: screen.preferredWindowWidth
    height: screen.preferredWindowHeight

    // global settings
    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay; scaleFactor: 0.5 }

    // for desktop development - easy test of scaling
    onHeightChanged: { gdisplay.adjust(width, height) }
    onWidthChanged: { gdisplay.adjust(width, height) }

    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        //propagateComposedEvents: true

        onClicked: {
            console.debug("MOUSE CLICKED ON MAIN MOUSEAREA")

            // make sure dropdown menus are closed
            toggleGeneralActions(false)
            toggleLocalGeneralActions(false)
        }
    }

    Rectangle {
        id: topbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        width: root.width
        height: topbarContainer.height * 1.15
        gradient: Gradient {
            GradientStop { position: 0.0; color: "gray" }
            GradientStop { position: 1.0; color: "snow" }
        }

        Item {
            id: topbarContainer
            anchors.centerIn: parent
            height: toggleLocalGeneralActionsButton.height
            width: root.width - gdisplay.touchCellWidth() / 2

            Button {
                id: toggleLocalGeneralActionsButton
                text: qsTr("Menu")
                anchors.left: parent.left
                anchors.top: parent.top

                onPressedChanged: {
                    if (toggleLocalGeneralActionsButton.pressed) {
                        //basicControls.visible = !basicControls.visible
                        //ui.visible = !basicControls.visible
                        //appbox.visible = false
                        toggleLocalGeneralActions()
                        toggleGeneralActions(false)
                    }

                }
            }

            Item {
                //color: "green"
                anchors.left: toggleLocalGeneralActionsButton.right
                anchors.right: toggleGeneralActionsButton.left
                anchors.leftMargin: gdisplay.touchCellWidth()/2
                anchors.rightMargin: gdisplay.touchCellWidth()/2
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Text {
                    id: currentPlayerLabel

                    color: mobapp.loggedIn ? "black" : "grey"
                    anchors.centerIn: parent
                    font.pixelSize: toggleLocalGeneralActionsButton.height - 4
                    text: UserModel.currentUserIsActive ? UserModel.currentUserName : " --- "
                }

                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onClicked: {
                        if (!anyGeneralActionsVisible()) {
                            onUserManagementSelected()
                        } else {
                            mouse.accepted = false // propagate
                        }
                    }
                }
            }

            Button {
                id: toggleGeneralActionsButton
                anchors.right: parent.right
                anchors.top: parent.top
                enabled: generalActions.hasActions

                text: "..."
                onPressedChanged: {
                    if (toggleGeneralActionsButton.pressed) {
                        toggleGeneralActions()
                        toggleLocalGeneralActions(false)
                    }
                }
            }
        }
    }

    Item {
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        DefaultMainArea {
            id: ui
            visible: true
            anchors.fill: parent
        }

        BasicControls {
            id: basicControls
            visible: false
            anchors.fill: parent

            Component.onCompleted: {
                basicControls.buttonPressed.connect(function (buttonID) {basicControlButtonPressed(buttonID)})
            }
        }

        GConfirmationDialog {
            id: msgDiag
            visible: false

            onOption1Selected: {
                msgDiag.visible = false
                var js = { action: "ConfirmationQuestionResponse",
                           questionId: msgDiag.questionId,
                           ref: msgDiag.option1Id }
                mobapp.sendMessage(JSON.stringify(js))
            }
            onOption2Selected: {
                msgDiag.visible = false
                var js = { action: "ConfirmationQuestionResponse",
                           questionId: msgDiag.questionId,
                           ref: msgDiag.option2Id }
                mobapp.sendMessage(JSON.stringify(js))
            }

        }


        GConfirmationDialog {
            id: disconnectConsoleDialog
            visible: false
            questionText: qsTr("You are logged in. Disconnect?")
            option1Text: qsTr("Yes")
            option2Text: qsTr("No")

            onOption1Selected: {
                visible = false
                onDisconnectRequested()
            }

            onOption2Selected: {
                visible = false
            }
        }


        GErrorDialog {
            id: errorDialog
            visible: false

            /*
            Component.onCompleted: {
                // testing. To test dynamic size adjustment we can set 'text' property
                // directly
                errorMessage = "Error occurred"
            }
            */
        }

        GFeedbackDialog {
            id: feedbackDialog
            visible: false
            //feedbackMessage: "This is a test message, quite long This is a test message, quite long This is a test message, quite long"
            showingTime: 2000 // ms
            height: preferredHeight
            initialOpacity: 0.6 // show partly what is behind
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }

        /*
        Window {
            id: testWindow
            visible: true
            flags: Qt.Dialog | Qt.FramelessWindowHint
            modality: Qt.WindowModal
            width: 100
            height: 100


            Rectangle {
                color: "red"
                anchors.fill: parent
            }
        }
        */

        Rectangle {
            id: appbox
            visible: false
            color: "pink"
            anchors.fill: parent

            Component.onCompleted: {
                AppBox.initialiaze(appbox)
                // make connection between dynamic content and our function
                // (this is not direct connect, we just define function that should
                //  be connected when dynamic content is in place)
                AppBox.connectOutgoingMessageTo(receiveFromAppBox)
            }

            function receiveFromAppBox(message) {
                console.debug("### RECEIVED FROM APPBOX: " + message)
                var js = Messages.createCustomAppBoxMsg(message)
                mobapp.sendMessage(js)
            }
        }

        Settings {
            id: settingsView
            visible: false // initial state
        }

        LoginView {
            id: loginview
            visible: false // initial state
        }

        DebugView {
            id: debugview
            visible: false // initial state

            onViewClosed: visible = false
        }
    }

    GeneralActions {
        id: generalActions
        visible: false
        anchors.top: topbar.bottom
        anchors.right: parent.right
    }

    GeneralActions {
        id: localGeneralActions
        visible: false
        anchors.top: topbar.bottom
        anchors.left: parent.left
    }

    /*
    StatusBar {
        id: bottombar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }*/

    Item {
        id: modalDialog
        anchors.fill: parent
        visible: false

        Rectangle {
            id: fadeBackground
            anchors.fill: parent
            opacity: 0.2
        }

        MouseArea {
            anchors.fill: parent
            // steal all events
            onClicked: console.debug("robber of events!")
        }

        Rectangle {
            color: "red"
            opacity: 1
            width: 100
            height: 100
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 50

            MouseArea {
                anchors.fill: parent
                onClicked: console.debug("red rectangle mousearea")
            }
        }
    }

    // -------------------- FUNCTIONS -------------------

    function basicControlButtonPressed(button)
    {
        Log.debug("CLICKED: " + button)
        var js = {action: "SelectBasicControlAction",
                  id: button}
        var data = JSON.stringify(js)
        Log.debug("SEND: " + data)
        mobapp.sendMessage(data)
    }

    // 'show' is optional, forces visibility to true/false
    function toggleGeneralActions(show) {
        if (typeof show !== 'undefined')
            generalActions.visible = show
        else
            generalActions.visible = !generalActions.visible
    }

    function onGeneralActionSelected(actionId) {
        mobapp.sendMessage(Messages.createGeneralActionMessage(actionId))

        // TODO: action button should have a feedback
        // TODO: and because of that dropdown menu shouldn't close immediately
        generalActions.visible = false
    }

    // 'show' is optional, forces visibility to true/false
    function toggleLocalGeneralActions(show) {
        if (typeof show !== 'undefined')
            localGeneralActions.visible = show
        else
            localGeneralActions.visible = !localGeneralActions.visible
    }

    function anyGeneralActionsVisible() {
        return generalActions.visible || localGeneralActions.visible
    }

    function onUserManagementSelected(show) {
        if (typeof show === 'undefined' || show === true) {
            // TODO: if already logged in then show logout dialog
            loginview.visible = true
            settingsView.visible = false
            debugview.visible = false
        } else {
            loginview.visible = false
            // TODO: view model not good one, we don't know what to show
        }
    }

    function onLocalGeneralActionSelected(actionId) {
        console.debug("LOCAL GENERAL ACTION: " + actionId)
        toggleLocalGeneralActions(false) // hide

        if (actionId === "Settings") {
            settingsView.visible = true
            // TODO: state?
            loginview.visible = false
            debugview.visible = false

        } else if (actionId === "Logout") {
            // TODO: remove
            onDisconnectRequested()

        } else if (actionId === "DebugInfo") {
            if (!debugview.visible) {
                debugview.visible = true

                // we keep other view as it is on the background
                //loginview.visible = false
                //settingsView.visible = false
            }

        } else if (actionId === "Home") {
            // first put to unvisible to that hooks may set model
            debugview.visible = false
            loginview.visible = false
            settingsView.visible = false

            ui.hostNameToConnect = SettingsModel.consoleAddress()
            console.debug("UPDATING CONSOLE ADDRES TO " + ui.hostNameToConnect)

            // TODO: could we use stacked view?

        } else if (actionId === "Console") {
            if (mobapp.loggedIn) {
                disconnectConsoleDialog.visible = true
            }

            // first put to unvisible to that hooks may set model
            /*
            debugview.visible = false
            loginview.visible = false
            settingsView.visible = false
            ui.visible = true
            appbox.visible = false
            */
        }


        // TODO: other kind of list, now always need to add if
    }

    function onPlayerMessageReceived(data) {
        console.log("PLAYER MESSAGE: " + data)

        var js = JSON.parse(data)
        if (js["action"] === "ConfirmationQuestion") {
            msgDiag.titleText = js["title"]
            msgDiag.questionText = js["text"]
            msgDiag.questionId = js["questionId"]

            // we expect exactly two options (might be dangerous)
            msgDiag.option1Id = js["options"][0]["id"]
            msgDiag.option1Text = js["options"][0]["text"]

            msgDiag.option2Id = js["options"][1]["id"]
            msgDiag.option2Text = js["options"][1]["text"]

            msgDiag.visible = true

        } else if (js["action"] === "CloseQuestion") {
            // currently we may have only one dialog instanceof
            msgDiag.visible = false

        } else if (js["action"] === "DefineAppBox") {
            AppBox.createContent(js["data"])

        } else if (js["action"] === "ShowAppBox") {
            appbox.visible = true
            AppBox.showContent() // TODO: should appbox be also set visible here??
            basicControls.visible = false
            ui.visible = false
            msgDiag.visible = false

        } else if (js["action"] === "AppBoxMessage") {
            AppBox.sendMessage(js["data"])

        } else if (js["action"] === "ShowBasicControls") {
            appbox.visible = false
            basicControls.visible = true
            console.debug("### " + js['enable'].toString())
            if (js.hasOwnProperty("enable")) {
                basicControls.enable(js['enable']) // list
            } else {
                basicControls.enable([]) // all buttons
            }

            ui.visible = false
            msgDiag.visible = false

        } else if (js["action"] === "DefineGeneralActions") {
            generalActions.setActions(js["actions"])

        } else if (js["action"] === "FeedbackMessage") {
            feedbackDialog.show(js["message"])
        }
    }


    function updateTexts()
    {
        debugview.serverTextStr = "Server " + (app.serverConnectionOK ? "OK" : "NOK") + ": ok=" + app.serverPingOKCounter + " nok=" + app.serverPingFailureCounter
        debugview.consoleTextStr = "Console " + (app.consoleConnectionOK ? "OK" : "NOK") + ": ok=" + app.consolePingOKCounter + " nok=" + app.consolePingFailureCounter
    }

    function onLoginViewClosed()
    {
        // no actions, really a cancel operation of showing login view
        loginview.visible = false
    }
    // TODO: how case when already connection but logging out and loggin in

    function onLogin(userName)
    {
        loginview.visible = false

        if (mobapp.loggedIn) {
            onDisconnectRequested()

            // if valid user -> reconnect
            if (UserModel.currentUserIsActive) {
                UserModel.selectCurrentUser(userName)
                onConnectRequested()
            }

        } else {
            UserModel.selectCurrentUser(userName)
        }

        // no really actions
        //   - name on title bar should update automatically
     }

     function onConnectRequested() {
         if (!UserModel.currentUserIsActive) {
             errorDialog.errorMessage = qsTr("No user selected")
             errorDialog.visible = true

             return
         }

        var username = UserModel.currentUserName
        var password = UserModel.currentPassword
        var guest = UserModel.currentIsGuest
        var rememberPassword = UserModel.currentIsRememberPassword

        console.debug("LOGIN: " + username + ", " + password + ", " + (guest ? "GUEST" : "NORMAL") + ", " + (rememberPassword ? "REMEMBER" : "-"))
        // TODO: so far only guest login supported
        mobapp.loginGuest(username)

        console.debug("USING CONSOLE ADDRESS: " + SettingsModel.consoleAddress())
        mobapp.openConsoleConnection(SettingsModel.consoleAddress()) // if opening fails then signal is emitted
        loginview.visible = false
        ui.state = "CONNECTING"


        // TODO: if connection takes time, we should actually show some kind of status bar

        // TODO: widget should take care by itself a status
        //currentPlayerLabel.text = username

        // TODO: how to show login errors?
    }

    function onDisconnectRequested() {
        mobapp.closeConsoleConnection()

        // TODO: we should have some kind of stacked view -> no matter what is open
        loginview.visible = false
        settingsView.visible = false
        debugview.visible = false
        basicControls.visible = false
        appbox.visible = false

        ui.visible = true
        ui.state = "DISCONNECTED"

        generalActions.clearActions()
    }


    function onLoginFailed(errorMsg) {
        console.debug("Login failed: " + errorMsg)
        errorDialog.errorMessage = errorMsg
        errorDialog.visible = true

        ui.state = "DISCONNECTED"
    }

    function onLogout(userName) {
        loginview.visible = false

        if (mobapp.loggedIn) {
            onDisconnectRequested()
        }
        UserModel.unselectCurrentUser()
    }

    // TODO: we should handle connection closed

    Component.onCompleted: {
        Log.initLog("main", Log.DEBUG_LEVEL)

        updateTexts()
        app.serverCounterChanged.connect(updateTexts)
        app.serverStatusChanged.connect(updateTexts)
        app.consoleCounterChanged.connect(updateTexts)
        app.consoleStatusChanged.connect(updateTexts)

        mobapp.playerMessageReceived.connect(onPlayerMessageReceived)
        mobapp.consoleConnectionOpenFailed.connect(onLoginFailed)
        mobapp.consoleConnectionClosed.connect(onDisconnectRequested) // TODO: should we have some kind of info for user what happened

        generalActions.actionSelected.connect(onGeneralActionSelected)

        localGeneralActions.setActions(
            [{actionId: "Home", actionName: "Home"},
             {actionId: "Console", actionName: "Console"},
             {actionId: "Logout", actionName: "Logout"},
             {actionId: "Settings", actionName: "Settings"},
             {actionId: "DebugInfo", actionName: "About"}
        ])
        localGeneralActions.actionSelected.connect(onLocalGeneralActionSelected)


        loginview.viewClosed.connect(onLoginViewClosed)
        loginview.login.connect(onLogin)
        loginview.logout.connect(onLogout)

        ui.connectToConsoleRequested.connect(onConnectRequested)
        ui.hostNameToConnect = SettingsModel.consoleAddress()

        Log.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)

        Log.debug("screen.preferredWindowWidth: " + screen.preferredWindowWidth)
        Log.debug("screen.preferredWindowHeight: " + screen.preferredWindowHeight)

        // TODO: how to impl dynamic binding for devenv
        //root.width = screen.preferredWindowWidth
        //root.height = screen.preferredWindowHeight
        // TODO: more info

        // TODO: should change name of prop!!!
        if (UserModel.autoLoginEnabled) {
            Log.debug("Auto login enabled")
            onConnectedRequest()
        }

        // debug
        console.debug("CurrentUserActive: " + UserModel.currentUserIsActive.toString())
        console.debug("Current user name: '" + UserModel.currentUserName + "'")

        // testing
        //feedbackDialog.show("jjadadadasdafdafa")
    }
}

// TODO: could camera be used to take user photo
