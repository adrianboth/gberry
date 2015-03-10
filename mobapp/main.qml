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
    }

    MainForm {
        id: ui
        visible: false
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
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottombar.top

        Component.onCompleted: {
            basicControls.upPressed.connect(function () {basicControlButtonPressed("Up")})
            basicControls.rightPressed.connect(function () {basicControlButtonPressed("Right")})
            basicControls.downPressed.connect(function () {basicControlButtonPressed("Down")})
            basicControls.leftPressed.connect(function () {basicControlButtonPressed("Left")})
            basicControls.okPressed.connect(function () {basicControlButtonPressed("OK")})
        }
    }

    /*
    MessageDialog {
        id: msgDiag
        title: "May I have your attention please"
        text: "It's so cool that you are using Qt Quick."
        Component.onCompleted: visible = true
    }
    */
/*
    Rectangle {
        id: bottombar
        opacity: 0.5
        color: "blue"
        height: 25
        anchors.left: parent.left
        anchors.right: parent. right
        anchors.bottom: parent.bottom
    }
*/
    StatusBar {
        id: bottombar
        anchors.left: parent.left
        anchors.right: parent. right
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
    }
}
