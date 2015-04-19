import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0

import "ReactGameModel.js" as ReactGameModel
import "ProgressFeedback.js" as ProgressFeedback

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

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: mainmenu.top
            color: "red"

            Text {
                text: "ReactGame"
                font.pixelSize: gdisplay.text_mm(25)
                anchors.centerIn: parent
            }
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

        GameView {
            id: appboxui
            anchors.fill: parent

            onVisibleChanged: {
                if (appboxui.visible) {
                    // TODO: now just fixed, can we even get coords before gameview visible
                    var topY = appboxui.yForBoxLine()
                    var initialX = appboxui.xForLeftMostBox()
                    var stepX = appboxui.stepSize()
                    Log.debug("appboxui visible: topY=" + topY + ", initialX=" + initialX + ", stepX=" + stepX)
                    ProgressFeedback.init(ReactGameModel, topY, initialX, stepX)
                } else {
                    ProgressFeedback.teardown()
                }
            }

        }

        CountDown {
            id: startcountdown
            visible: false
            anchors.centerIn: parent

            onFinished: {
                ReactGameModel.start()
            }
        }

        state: "MENU"
        // TODO: temporary
        //state: "GAME"

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

                                // TODO: could this be embedded to setup() but what is UI effect
                                ProgressFeedback.teardown()
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
                                //var js = {action: "ShowAppBox"}
                                //playersManager.sendAllPlayersMessage(JSON.stringify(js))
                                ReactGameModel.initializePlayers()
                                ProgressFeedback.setup()
                                appboxui.setNumbers(ReactGameModel.numbers())
                                startcountdown.start()
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

        if (playersManager.numberOfPlayers === 0) {
            // last player left

            // if in game -> return to menu
            if (mainarea.state === "GAME") {
                messageBoard.insertMessage("Last player left -> back to menu")
                mainarea.state = "MENU"
            }
        }
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
            // TODO: could it be possible to have more object oriented questions
            //       (i.e. if visible it has been registered as listener)

            if (js["questionId"] === "exitgame") {
                if (exitConfirmationDialog.visible) {
                    exitConfirmationDialog.selectOption(js["ref"])

                    // just to make sure everyones dialogs are closed
                    playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
                }
            } else if (js["questionId"] === "playanother") {
                playedwonDialog.selectOption(js["ref"])
            }

        } else if (js["action"] === "GeneralAction") {
            if (js["id"] === "GameMenu") {
                console.debug("GAME MENU GENERAL ACTION")
                mainarea.state = "MENU"
            }
        } else if (js["action"] === "AppBoxMessage") {
            ReactGameModel.playerMessageReceived(pid, js["data"])
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
        questionText: qsTr("Are you sure to exit this game?")
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
                  questionId: "exitgame",
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



    GConfirmationDialog {
        id: playedwonDialog
        questionText: "xxx"
        option1Id: "Yes"
        option1Text: qsTr("Yes")
        option2Id: "No"
        option2Text: qsTr("No")

        function declarePlayerWon(pid) {
            var name = playersManager.playerName(pid)
            playedwonDialog.questionText = qsTr("Player " + name + " won!\n\nPlay another?")
            playedwonDialog.visible = true

            // TODO: GConfirmationDialog could have a helper ...
            var js = {action: "ConfirmationQuestion",
                      questionId: "playanother", // TODO: client needs to return this
                      title: playedwonDialog.titleText,
                      text: playedwonDialog.questionText,
                      options: [{id:   playedwonDialog.option1Id,
                                 text: playedwonDialog.option1Text},
                                {id:   playedwonDialog.option2Id,
                                 text: playedwonDialog.option2Text}]
                     }

            playersManager.sendAllPlayersMessage(JSON.stringify(js))
        }

        onOption1Selected: {
            // yes
            playedwonDialog.visible = false
            mainarea.state = "GAME"
        }

        onOption2Selected: {
            // no
            mainarea.state = "MENU"
            playedwonDialog.visible = false
        }
    }

    function onPlayerWon(pid) {
        mainarea.state = "END"
        playedwonDialog.declarePlayerWon(pid)
    }
    function onPlayerCorrectNumber(pid) {
        messageBoard.insertPlayerMessage(pid, "Correct number!")
        ProgressFeedback.updatePlayer(pid)
        var msg = {action: "CorrectNumberFeedback"}
        playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(msg))
    }
    function onPlayerInvalidNumber(pid) {
        messageBoard.insertPlayerMessage(pid, "Invalid number!")
        var msg = {action: "InvalidNumberFeedback"}
        playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(msg))
    }

    // TODO: sound when game starts
    // TODO: sound when timer ticks
    // TODO: sound when game ends

    Component.onCompleted: {
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        ReactGameModel.callbacks.playerWon.connect(onPlayerWon)
        ReactGameModel.callbacks.playerCorrectNumber.connect(onPlayerCorrectNumber)
        ReactGameModel.callbacks.playerInvalidNumber.connect(onPlayerInvalidNumber)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")
    }
}

// TODO: credits main page
// TODO: why timer text not center

// TODO: UI candy
//   - timer number size changing, going smaller
//   - at least zero fading
//
// some indicator who has type what

// TODO: confirmation dialog doesn't work; why ? I thought ready; check mainui

// TODO: get fancy background image (1920x1080)

// TODO: record running time: stop clock when winner
