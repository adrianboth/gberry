import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import "DeveloperLog.js" as Log
import "AppBox.js" as AppBox


Window {
    visible: true
    width: 320
    height: 400

    ToolBar {
        id: topbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            spacing: 2

            Button {
                id: bc
                text: "BC"
                onPressedChanged: {
                    if (bc.pressed) {
                        basicControls.visible = !basicControls.visible
                        ui.visible = !basicControls.visible
                        appbox.visible = false
                    }

                }
            }

            Button {
                id: test
                text: "T1"
                onPressedChanged: {
                    if (test.pressed) {
                        appbox.visible = !appbox.visible
                    }
                }
            }
            Button {
                id: test2
                text: "T2"
                onPressedChanged: {
                    if (test2.pressed) {
                        var qmlContent2 = 'import QtQuick 2.0;\
            Rectangle {\
            color: "lightgrey";\
            border.width: 1;\
            border.color: "black";\
            width: 200;\
            height: 50;\
            anchors.centerIn: parent;\
            Text {\
              text: "CONTENT";\
              anchors.centerIn: parent;\
            }\
            signal outgoingMessage(var data);\
            function incomingMessage(data) {\n\
            console.debug("### APPBOX MSG55555: " + data)\n\
            outgoingMessage("got appbox msg")\n\
            }\n\
            }'
                        AppBox.createContent(qmlContent2) // for testing
                        AppBox.showContent()
                    }
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

    Rectangle {
        id: appbox
        visible: false
        color: "pink"
        anchors.top: topbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottombar.top

        Component.onCompleted: {
            AppBox.initialiaze(appbox)

            var qmlContent = 'import QtQuick 2.0; Rectangle {color: "red"; width: 20; height: 20}'
            var qmlContent2 = 'import QtQuick 2.0;\
Rectangle {\
color: "lightgrey";\
border.width: 1;\
border.color: "black";\
width: 200;\
height: 50;\
anchors.centerIn: parent;\
Text {\
  text: "CONTENT";\
  anchors.centerIn: parent;\
}\
signal outgoingMessage(var data);\
function incomingMessage(data) {\n\
console.debug("### APPBOX MSG1111: " + data)\n\
outgoingMessage("got appbox msg")\n\
}\n\
}'
            AppBox.connectOutgoingMessageTo(receiveFromAppBox)
            //AppBox.createContent(qmlContent2) // for testing

            //AppBox.showContent()
            //AppBox.releaseContent()
            //AppBox.showContent()
            //AppBox.connectOutgoingMessageTo(receiveFromAppBox)
            //AppBox.sendMessage("hello appbox")
        }

        function receiveFromAppBox(message) {
            console.debug("### RECEIVED FROM APPBOX: " + message)
            var js = {action: "AppBoxMessage",
                      data: message}
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

        if (js["action"] === "DefineAppBox") {
            // TODO: naming not best
            console.debug("#### CREATING CONTENT")
            var qmlContent2 = 'import QtQuick 2.0;\
Rectangle {\
color: "lightgrey";\
border.width: 1;\
border.color: "black";\
width: 200;\
height: 50;\
anchors.centerIn: parent;\
Text {\
  text: "CONTENT";\
  anchors.centerIn: parent;\
}\
signal outgoingMessage(var data);\
function incomingMessage(data) {\n\
console.debug("### APPBOX MSG3333: " + data)\n\
outgoingMessage("got appbox msg")\n\
}\n\
}'
            //AppBox.createContent(qmlContent2) // testing
            AppBox.createContent(js["data"])
            var qmlContent3 = 'import QtQuick 2.0;\
Rectangle {\
color: "lightgrey";\
border.width: 1;\
border.color: "black";\
width: 200;\
height: 50;\
anchors.centerIn: parent;\
Text {\
  text: "CONTENT";\
  anchors.centerIn: parent;\
}\
signal outgoingMessage(var data);\
function incomingMessage(data) {\n\
console.debug("### APPBOX MSG9999: " + data)\n\
outgoingMessage("got appbox msg")\n\
}\n\
}'
            //AppBox.createContent(qmlContent3) // for testing
            //AppBox.showContent()
        }

        if (js["action"] === "ShowAppBox") {
            appbox.visible = true
            AppBox.showContent() // TODO: should appbox be also set visible here??
            basicControls.visible = false
            ui.visible = false
            msgDiag.visible = false
        }

        if (js["action"] === "AppBoxMessage") {
            AppBox.sendMessage(js["data"])
        }

        if (js["action"] === "ShowBasicControls") {
            appbox.visible = false
            basicControls.visible = true
            ui.visible = false
            msgDiag.visible = false
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
