import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2

import "DeveloperLog.js" as Log


Window {
    visible: true
    width: 320
    height: 400

    ToolBar {
        id: topbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Button {
            id: bc
            text: "BC"
            onPressedChanged: {
                if (bc.pressed) {
                    basicControls.visible = !basicControls.visible
                    ui.visible = !ui.visible
                }

            }

        }
    }

    MainForm {
        id: ui
        visible: true
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottombar.top

        // TODO: not sure if this right way to configure ui side
        property string serverTextStr: "Server (INIT)"
        property string consoleTextStr: "Console (INIT)"
        mouseArea.onClicked: {
            Qt.quit();
        }

        connectButton.onClicked: { console.log("CONNECT"); connectToConsole() }


        Component.onCompleted: {
            connectButton1.connectButtonClicked.connect(sendMessage)
        }

        /*
        Connections {
            target: connectButton1
            onConnectButtonClicked: {
                console.log("SEND MESSAGE")
            }
        }
*/
    }

    function basicControlButtonPressed(button)
    {
        Log.debug("CLICKED: " + button)
        var js = {action: "SelectBasicControlAction",
                  id: button}
        var data = JSON.stringify(js)
        Log.debug("SEND: " + data)
        mobapp.sendMessage(data)
    }

    BasicControls {
        id: basicControls
        visible: false
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottombar.top

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


    StatusBar {
        id: bottombar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    function connectToConsole()
    {
        mobapp.loginGuest("Foobar")
        mobapp.openConsoleConnection("localhost")
        //mobapp.sendMessage()
    }

    function sendMessage()
    {
        console.log("SEND MESSAGE")
        mobapp.sendMessage("hello world")
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
        }
    }


    function updateTexts()
    {
        ui.serverTextStr = "Server " + (app.serverConnectionOK ? "OK" : "NOK") + ": ok=" + app.serverPingOKCounter + " nok=" + app.serverPingFailureCounter
        ui.consoleTextStr = "Console " + (app.consoleConnectionOK ? "OK" : "NOK") + ": ok=" + app.consolePingOKCounter + " nok=" + app.consolePingFailureCounter
    }

    Component.onCompleted: {
        Log.initLog("main", Log.DEBUG_LEVEL)

        updateTexts()
        app.serverCounterChanged.connect(updateTexts)
        app.serverStatusChanged.connect(updateTexts)
        app.consoleCounterChanged.connect(updateTexts)
        app.consoleStatusChanged.connect(updateTexts)

        mobapp.playerMessageReceived.connect(onPlayerMessageReceived)
    }
}
