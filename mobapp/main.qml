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


    ToolBar {
        id: topbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Button {
            id: toggleLocalGeneralActionsButton
            text: qsTr("Menu")
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 5

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

        Text {
            id: currentPlayerLabel

            color: mobapp.loggedIn ? "black" : "grey"
            anchors.centerIn: parent
            font.pixelSize: toggleLocalGeneralActionsButton.height - 4
            text: UserModel.currentUserName
        }

        Button {
            id: toggleGeneralActionsButton
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 5
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

    MouseArea {
        id: mainMouseArea
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottombar.top

        onClicked: {
            console.debug("MOUSE CLICKED ON MAIN MOUSEAREA")

            // make sure dropdown menus are closed
            toggleGeneralActions(false)
            toggleLocalGeneralActions(false)
        }

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

        /*
        GConfirmationDialog {
            id: testConfirmationDialog
            visible: true // initial state
            //questionText: qsTr("Play another?")
            //option1Text: qsTr("Yes")
            //option2Text: qsTr("No")

            onOption1Selected: {
                // Yes
                Qt.quit()
            }

            onOption2Selected: {
                testConfirmationDialog.visible = false
            }

            Component.onCompleted:  {
                debug()
                questionText = "Play another? asd fasf as  sar easr eae "
                option1Text = "Yes"
                option2Text = "No"
                debug()
            }
        }
        */

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

    StatusBar {
        id: bottombar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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

    function onLocalGeneralActionSelected(actionId) {
        console.debug("LOCAL GENERAL ACTION: " + actionId)
        toggleLocalGeneralActions(false) // hide

        if (actionId === "Settings") {
            settingsView.visible = true
            // TODO: state?
            loginview.visible = false
            debugview.visible = false

        } else if (actionId === "Login") {
            loginview.visible = true
            settingsView.visible = false
            debugview.visible = false

        } else if (actionId === "Logout") {
            onLogout()

        } else if (actionId === "DebugInfo") {
            debugview.visible = true
            loginview.visible = false
            settingsView.visible = false

        } else if (actionId === "Home") {
            ui.hostNameToConnect = SettingsModel.consoleAddress()
            debugview.visible = false
            loginview.visible = false
            settingsView.visible = false
            // TODO: could we use stacked view?
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
        }
    }


    function updateTexts()
    {
        debugview.serverTextStr = "Server " + (app.serverConnectionOK ? "OK" : "NOK") + ": ok=" + app.serverPingOKCounter + " nok=" + app.serverPingFailureCounter
        debugview.consoleTextStr = "Console " + (app.consoleConnectionOK ? "OK" : "NOK") + ": ok=" + app.consolePingOKCounter + " nok=" + app.consolePingFailureCounter
    }

    function onLoginViewClosed()
    {
        loginview.visible = false
    }

    function onLogin()
    {
        if (mobapp.loggedIn) {
            onLogout()
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

        // TODO: if connection takes time, we should actually show some kind of status bar

        // TODO: widget should take care by itself a status
        //currentPlayerLabel.text = username

        // TODO: how to show login errors?
    }

    function onLoginFailed(errorMsg) {
        console.debug("Login failed: " + errorMsg)
        errorDialog.errorMessage = errorMsg
        errorDialog.visible = true
    }

    function onLogout() {
        mobapp.closeConsoleConnection()

        // TODO: we should have some kind of stacked view -> no matter what is open
        loginview.visible = false
        settingsView.visible = false
        debugview.visible = false
        basicControls.visible = false
        ui.visible = true
    }

    Component.onCompleted: {
        Log.initLog("main", Log.DEBUG_LEVEL)

        updateTexts()
        app.serverCounterChanged.connect(updateTexts)
        app.serverStatusChanged.connect(updateTexts)
        app.consoleCounterChanged.connect(updateTexts)
        app.consoleStatusChanged.connect(updateTexts)

        mobapp.playerMessageReceived.connect(onPlayerMessageReceived)
        mobapp.consoleConnectionOpenFailed.connect(onLoginFailed)

        generalActions.actionSelected.connect(onGeneralActionSelected)

        localGeneralActions.setActions(
            [{actionId: "Home", actionName: "Home"},
             {actionId: "Login", actionName: "Login"},
             {actionId: "Logout", actionName: "Logout"},
             {actionId: "Settings", actionName: "Settings"},
             {actionId: "DebugInfo", actionName: "Debug Info"}
        ])
        localGeneralActions.actionSelected.connect(onLocalGeneralActionSelected)


        loginview.viewClosed.connect(onLoginViewClosed)
        loginview.login.connect(onLogin)

        ui.connectToConsoleRequested.connect(onLogin)
        ui.hostNameToConnect = SettingsModel.consoleAddress()

        Log.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)

        Log.debug("screen.preferredWindowWidth: " + screen.preferredWindowWidth)
        Log.debug("screen.preferredWindowHeight: " + screen.preferredWindowHeight)

        // TODO: how to impl dynamic binding for devenv
        //root.width = screen.preferredWindowWidth
        //root.height = screen.preferredWindowHeight
        // TODO: more info

        if (UserModel.autoLoginEnabled) {
            Log.debug("Auto login enabled")
            onLogin()
        }
    }
}

// TODO: could camera be used to take user photo
