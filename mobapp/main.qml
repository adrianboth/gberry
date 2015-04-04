import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import "settings"
import "login"

import "ui/gberry-lib/js/DeveloperLog.js" as Log

import "AppBox.js" as AppBox
import "js/MobileClientMessages.js" as Messages

import "ui/gberry-lib/qml" as GBerry
//import "ui/gberry-lib/qml"

Window {
    id: mainwindow
    visible: true
    width: screen.preferredWindowWidth
    height: screen.preferredWindowHeight

    GBerry.GButton {}
    //GButton {}

    ToolBar {
        id: topbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Button {
            id: toggleLocalGeneralActionsButton
            text: "M"
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


        MainForm {
            id: ui
            visible: true
            anchors.fill: parent

            // TODO: not sure if this right way to configure ui side
            property string serverTextStr: "Server (INIT)"
            property string consoleTextStr: "Console (INIT)"
            mouseArea.onClicked: {
                Qt.quit();
            }

            connectButton.onClicked: { console.log("CONNECT"); connectToConsole() }
        }



        BasicControls {
            id: basicControls
            visible: false
            anchors.fill: parent

            Component.onCompleted: {
                basicControls.buttonPressed.connect(function (buttonID) {basicControlButtonPressed(buttonID)})
            }
        }


        MessageDialog {
            id: msgDiag
            visible: false

            standardButtons: StandardButton.Yes | StandardButton.No

            onYes: {
                msgDiag.visible = false
                var js = { action: "ConfirmationQuestionResponse",
                           ref: "Yes" }
                mobapp.sendMessage(JSON.stringify(js))
            }
            onNo: {
                msgDiag.visible = false
                var js = { action: "ConfirmationQuestionResponse",
                           ref: "No" }
                mobapp.sendMessage(JSON.stringify(js))
            }
        }

        Rectangle {
            id: appbox
            visible: false
            color: "pink"
            anchors.fill: parent

            Component.onCompleted: {
                AppBox.initialiaze(appbox)
                AppBox.connectOutgoingMessageTo(receiveFromAppBox)
            }

            function receiveFromAppBox(message) {
                console.debug("### RECEIVED FROM APPBOX: " + message)
                var js = {action: "AppBoxMessage",
                          data: message}
                mobapp.sendMessage(JSON.stringify(js))
            }
        }

        Settings {
            id: settings
            visible: false // initial state
        }

        LoginView {
            id: loginview
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
            settings.visible = true
            // TODO: state?
            loginview.visible = false

        } else if (actionId === "Login") {
            loginview.visible = true
            settings.visible = false

        } else if (actionId === "Reconnect") {
            mobapp.closeConsoleConnection()
            mobapp.openConsoleConnection(settings.consoleAddress()) // TODO: use defined profiles
        }
    }

    function connectToConsole()
    {
        mobapp.loginGuest("Foobar")
        //mobapp.openConsoleConnection("192.168.1.248")
        mobapp.openConsoleConnection("localhost")
    }

    function onPlayerMessageReceived(data) {
        console.log("PLAYER MESSAGE: " + data)

        var js = JSON.parse(data)
        if (js["action"] === "ConfirmationQuestion") {
            msgDiag.title = js["title"]
            msgDiag.text = js["text"]
            // TODO: Use StandardButtons or custom impl for handling option (Yes/No),
            //       now using built in
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
            ui.visible = false
            msgDiag.visible = false

        } else if (js["action"] === "DefineGeneralActions") {
            generalActions.setActions(js["actions"])
        }
    }


    function updateTexts()
    {
        ui.serverTextStr = "Server " + (app.serverConnectionOK ? "OK" : "NOK") + ": ok=" + app.serverPingOKCounter + " nok=" + app.serverPingFailureCounter
        ui.consoleTextStr = "Console " + (app.consoleConnectionOK ? "OK" : "NOK") + ": ok=" + app.consolePingOKCounter + " nok=" + app.consolePingFailureCounter
    }

    function onLoginViewClosed()
    {
        loginview.visible = false
    }
    function onLogin(username, password, guest, rememberPassword)
    {
        console.debug("LOGIN: " + username + ", " + password + ", " + (guest ? "GUEST" : "NORMAL") + ", " + (rememberPassword ? "REMEMBER" : "-"))
        mobapp.loginGuest(username)
        console.debug("USING CONSOLE ADDRESS: " + settings.consoleAddress())
        mobapp.openConsoleConnection(settings.consoleAddress())
        loginview.visible = false

        // TODO: how to show login errors?
    }

    Component.onCompleted: {
        Log.initLog("main", Log.DEBUG_LEVEL)

        updateTexts()
        app.serverCounterChanged.connect(updateTexts)
        app.serverStatusChanged.connect(updateTexts)
        app.consoleCounterChanged.connect(updateTexts)
        app.consoleStatusChanged.connect(updateTexts)

        mobapp.playerMessageReceived.connect(onPlayerMessageReceived)

        generalActions.actionSelected.connect(onGeneralActionSelected)

        localGeneralActions.setActions(
            [{actionId: "Login", actionName: "Login"},
             {actionId: "Settings", actionName: "Settings"},
             {actionId: "Reconnect", actionName: "Reconnect"}
        ])
        localGeneralActions.actionSelected.connect(onLocalGeneralActionSelected)


        loginview.viewClosed.connect(onLoginViewClosed)
        loginview.login.connect(onLogin)

        Log.debug("TEST: " + screen.name)
        Log.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)

        Log.debug("screen.preferredWindowWidth: " + screen.preferredWindowWidth)
        Log.debug("screen.preferredWindowHeight: " + screen.preferredWindowHeight)

        // TODO: how to impl dynamic binding for devenv
        //mainwindow.width = screen.preferredWindowWidth
        //mainwindow.height = screen.preferredWindowHeight
        // TODO: more info
    }
}
