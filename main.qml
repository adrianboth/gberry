import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0

import "GameModel.js" as GameModel
import "ProgressFeedback.js" as ProgressFeedback

Window {
    id: root
    visible: true
    // TODO: how to have different sizes for dev and real
    width: 800
    height: 600

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

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


        Rectangle {
            id: setupview // TODO: menuview
            anchors.fill: parent

            border.color: "slategray"
            gradient: Gradient {
                GradientStop { position: 0.0; color: "lightsteelblue" }
                GradientStop { position: 1.0; color: "slategray" }
            }

            Item {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                //anchors.bottom: mainmenu.top
                //color: "red"
                //opacity: 0

                // TODO: need to position correctly
                Text {
                    text: "TicTacToe"
                    font.pixelSize: gdisplay.text_mm(25)
                    anchors.centerIn: parent
                }
            }

        }

        // --- alternative view

        GameView {
            id: gameview
            visible: true
            anchors.fill: parent
        }

        state: "WAITING_PLAYERS"
        // TODO: temporary
        //state: "GAME"

        states: [
                State {
                    name: "WAITING_PLAYERS"
                    //PropertyChanges { target: gameview; visible: false }
                    PropertyChanges { target: setupview; visible: true }
                    StateChangeScript {
                            name: "myScript1"
                            script: {

                            }
                        }
                },

                State {
                    name: "GAME"
                    PropertyChanges { target: gameview; visible: true}
                    PropertyChanges { target: setupview; visible: false}

                    StateChangeScript {
                            name: "myScript"
                            script: {

                            }
                        }
                },
                State {
                    // game ended state
                    name: "END"
                    // keep still game area visible
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
            // TODO: we should ask from view what basic controls should be enabled
            js = {action: "ShowBasicControls",
                  enable: ["Up", "Down", "OK"]}
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

        if (playersManager.numberOfPlayers === 0) {
            // last player left

            // if in game -> return to menu
            if (mainarea.state === "GAME") {
                messageBoard.insertMessage("Last player left -> back to menu")
                mainarea.state = "WAITING_PLAYERS"
            }
        }
    }

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        messageBoard.insertPlayerMessage(pid, data)

        var js  = JSON.parse(data)

        if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION")
                mainarea.state = "WAITING_PLAYERS"
            }
        } else if (js["action"] === "AppBoxMessage") {
            GameModel.playerMessageReceived(pid, js["data"])
        }
    }

    function onPlayerWon(pid) {
        mainarea.state = "END"
        gametimelabel.stop()
        appboxui.stop()
        HighScore.recordWinningResult(playersManager.playerName(pid), gametimelabel.finalTime())
        playedwonDialog.declarePlayerWon(pid, gametimelabel.finalTime())
    }

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        //GaneModel.callbacks.playerWon.connect(onPlayerWon)
        //GaneModel.callbacks.playerCorrectNumber.connect(onPlayerCorrectNumber)
        //GaneModel.callbacks.playerInvalidNumber.connect(onPlayerInvalidNumber)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        // testing dialog
        //playedwonDialog.declarePlayerWon(0, 0)
    }
}
