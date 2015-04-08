import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0


Window {
    id: root
    visible: true
    width: 800
    height: 600

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    // testing
    //GBerry.GButton { label: "Test2"; anchors.centerIn: parent; z: 1000 }

    // TODO: column layout
    InfoBar {
        id: infobar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        z: 100

        gradient: Gradient {
            GradientStop { position: 0.0; color: "snow" }
            GradientStop { position: 1.0; color: "gray" }
        }
    }

    Rectangle {
        id: mainarea

        anchors.top: infobar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        border.color: "slategray"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }

        MainMenu {
            id: mainmenu
            anchors.centerIn: parent

            items: [
                MainMenuItem {
                    text: qsTr("Play Game")
                    onSelected: playGameSelected()
                },
                MainMenuItem {
                    text: qsTr("Exit")
                    onSelected: exitGameSelected()
                }
            ]
        }

        AppBoxUI {
            id: appboxui
            color: "orange"
            anchors.fill: parent
        }

        state: "MENU"
        states: [
                State {
                    name: "MENU"
                    PropertyChanges { target: appboxui; visible: false }
                    PropertyChanges { target: mainmenu; visible: true }
                    StateChangeScript {
                            name: "myScript1"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO MENU")
                                var js = {action: "ShowBasicControls"}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))
                            }
                        }
                },
                State {
                    name: "GAME"
                    PropertyChanges { target: appboxui; visible: true}
                    PropertyChanges { target: mainmenu; visible: false}

                    StateChangeScript {
                            name: "myScript"
                            script: {
                                console.debug("STATE CHANGE SCRIPT: INTO GAME")
                                var js = {action: "ShowAppBox"}
                                playersManager.sendAllPlayersMessage(JSON.stringify(js))
                            }
                        }
                }
            ]
    }

    function onPlayerIn(pid)
    {
        console.log("New player in: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "New player")

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "GameMenu", actionName: "Menu"}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (mainarea.state === "MENU") {
            js = {action: "ShowBasicControls"}
            playersManager.sendPlayerMessage(pid, JSON.stringify(js))
        } else if (mainarea.state === "GAME") {
            js = {action: "DefineAppBox",
                      data: AppBoxMaster.dataStr()}

            playersManager.sendAllPlayersMessage(JSON.stringify(js))

            js = {action: "ShowAppBox"}
            playersManager.sendAllPlayersMessage(JSON.stringify(js))
        }
    }
    function onPlayerOut(pid)
    {
        console.log("Player left: id = " + pid)
        messageBoard.insertPlayerMessage(pid, "Player left")
    }
    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)

        var js  = JSON.parse(data)
        if (js["action"] === "SelectBasicControlAction")
        {
            if (js["id"] === "Up")
                mainmenu.moveFocusToNext()
            else if (js["id"] === "Down")
                mainmenu.moveFocusToPrevious()
            else if (js["id"] === "OK")
                mainmenu.selectCurrent()

        } else if (js["action"] === "ConfirmationQuestionResponse") {
            // TODO: case when multiple possible confirmations
            if (exitConfirmationDialog.visible) {
                exitConfirmationDialog.selectOption(js["ref"])

                // just to make sure everyones dialogs are closed
                playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
            }

        } else if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION")
                mainarea.state = "MENU"
            }
        }
    }

    MessageBoard {
        id: messageBoard
        opacity: 0.5
        visible: gsettings.developmentMode

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
    }

    GConfirmationDialog {
        id: exitConfirmationDialog
        visible: false // initial state
        //text: "Are you sure to exit this game?"
        questionText: qsTr("Exit?") // "Are you sure to exit the game?"
        option1Text: qsTr("Yes")
        option2Text: qsTr("No")

        onOption1Selected: {
            // Yes
            Qt.quit()
        }

        onOption2Selected: {
            exitConfirmationDialog.visible = false
        }
    }

    function playGameSelected() {
        console.debug("Play selected")
        // TODO: appbox version handshaking at some point

        var js = {action: "DefineAppBox",
                  data: AppBoxMaster.dataStr()}

        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        js = {action: "ShowAppBox"}
        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        // TODO: waiting response from player (every one ready)

        // TODO: create timer to enable buttons
        // TODO: wait button presses -> !inform whose came first
        // TODO:   - ?? how to move to next

        mainarea.state = "GAME"

    }
    function exitGameSelected() {
        console.debug("Exit selected")

        // as demo send confirmation to clients

        // TODO: how localization of these texts would go?

        var js = {action: "ConfirmationQuestion",
                  title: exitConfirmationDialog.titleText,
                  text: exitConfirmationDialog.questionText,
                  options: [{id:   exitConfirmationDialog.option1Id,
                             text: exitConfirmationDialog.option1Text},
                            {id:   exitConfirmationDialog.option2Id,
                             text: exitConfirmationDialog.option2Text}]
                 }

        // TODO: should be send only for controlling player but that is not yet implemented)
        //    * Now for first player

        // TODO: gets really strange player id list (not list at all??)
        //playersManager.sendPlayerMessage(playersManager.playerIds(), JSON.stringify(js))
        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        // show also on big screen the question
        exitConfirmationDialog.visible = true

        // TODO: disable everything else -> record state
    }

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")
    }
}
