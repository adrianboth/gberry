import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import GBerry 1.0
import GBerryConsole 1.0

import "GameModel.js" as GameModel
import "PlayerWaitingModel.js" as PlayerWaitingModel

Window {
    id: root
    visible: true
    // TODO: how to have different sizes for dev and real
    width: 800
    height: 600

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }


    Rectangle {
        id: mainarea
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }

        // --- alternative view


        JoinView {
            id: joinview
            visible: true
            anchors.fill: parent
            z: 100 // on top of gameview
            opacity: 1

            OpacityAnimator {
                id: opacityAnimator
                target: joinview
                from: 1
                to: 0
                duration: 1000
                running: false
                onStopped: {
                    console.debug("opacity animator finished")
                    joinview.visible = false
                    joinview.opacity = 1
                }
            }
        }

        GameView {
            id: gameview
            visible: false
            anchors.fill: parent
            z: 50 // below join view
            opacity: 0 // initial

            OpacityAnimator {
                id: opacityAnimator2
                target: gameview
                from: 0
                to: 1
                duration: 2000
                running: false
                onStarted: {
                    console.debug("opacity animator started")
                    gameview.opacity = 0
                    gameview.visible = true
                }

                onStopped: {
                    console.debug("opacity animator finished")
                    gameview.player1Turn()
                    GameModel.enableTurns()
                }
            }

            onGameClosed: {
                mainarea.state = "WAITING_PLAYERS"
                // this handler is mainly for development time, handling mouse clicks ..
            }
        }

        // ---

        state: "WAITING_PLAYERS"
        // TODO: temporary
        //state: "GAME"

        states: [
                State {
                    name: "WAITING_PLAYERS"

                    StateChangeScript { script: resetWaitingPlayersState() }
                    PropertyChanges { target: joinview; visible: true }
                    PropertyChanges { target: gameview; visible: false }
                },

                State {
                    name: "GAME"
                    StateChangeScript { script: GameModel.initGame(PlayerWaitingModel.joinedPlayers()) }
                    PropertyChanges { target: gameview; visible: true}
                    PropertyChanges { target: opacityAnimator2; running: true}
                    PropertyChanges { target: opacityAnimator; running: true}
                },

                State {
                    // game ended state
                    name: "END"
                    // keep still game area visible
                    PropertyChanges { target: gameview; visible: true }
                    PropertyChanges { target: joinview; visible: false }
                }
            ]
    }


    function onPlayerIn(pid)
    {
        console.log("Player in: id = " + pid)

        var js = {action: "DefineGeneralActions",
                  actions: [{actionId: "AbortGame", actionName: "Abort Game"}]}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        js = {action: "DefineAppBox", data: AppBoxMaster.dataStr()}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        // in our case all actions happen in appbox -> we can show it all the time
        js = {action: "ShowAppBox"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (mainarea.state === "WAITING_PLAYERS") {
            js = {action: "MoveToState", state: "WAITING_PLAYERS"}
            playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(js))

        } else if (mainarea.state === "GAME") {
            js = {action: "MoveToState", state: "WAIT_GAME"}
            playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(js))
        }

        // TODO: what about end game view
    }

    function onPlayerOut(pid)
    {
        // TODO:

        // if waiting -> then drop that player out
        // TODO: it would be great to have temporary message what happened (just on main screen?)
        if (mainarea.state === "WAITING_PLAYERS" && PlayerWaitingModel.hasJoined(pid)) {
            resetWaitingPlayersState()
        } else if (mainarea.state === "GAME" && GameModel.isPlaying(pid)) {
            mainarea.state = "WAITING_PLAYERS"
        }

        // if player in game -> cancel the game -> to beginning (TODO: what kind of message?)

        console.log("Player left: id = " + pid)
    }

    function resetWaitingPlayersState() {
        PlayerWaitingModel.reset()
        joinview.reset()
        var msgToSend = {action: "MoveToState", state: "WAITING_PLAYERS"}
        playersManager.sendAllPlayersMessage(Messages.createCustomAppBoxMsg(msgToSend))
    }

    //js = {action: "ShowAppBox"}
    //playersManager.sendAllPlayersMessage(JSON.stringify(js))

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        var js  = JSON.parse(data)
        var msgToSend

        if (js["action"] === "GeneralAction") {
            if (js["id"] === "AbortGame") {
                // TODO: only playing players can abort

                // TODO: but otherwise
                //   - if no real player is not active
                //   - and abort is selected second time in 10s
                //   (there is room for sabotage, but is not really big issue as everyone are in a same room)
                if (mainarea.state === "WAITING_PLAYERS") {
                    resetWaitingPlayersState()
                }
                else {
                    mainarea.state = "WAITING_PLAYERS"
                }
            }

        } else if (js["action"] === "AppBoxMessage") {
            console.debug("AppBoxMessage")
            if (GameModel.playerMessageReceived(pid, js["data"])) return true
            if (PlayerWaitingModel.playerMessageReceived(pid, js["data"])) return true

            console.debug("AppBoxMessage: not model")

            // TODO: not sure if these are in right place

            if (js["data"]["action"] === "NextGame") {
                console.debug("AppBoxMessage: NextGame")
                mainarea.state = "WAITING_PLAYERS"

            } else if (js["data"]["action"] === "CancelJoin" && mainarea.state === "WAITING_PLAYERS") {
                // if one player cancel -> return to initial joining phase
                resetWaitingPlayersState()
            }
        }
    }

    function onSecondPlayerJoinedAnimationFinished() {
        gameview.visible = true
        var players = PlayerWaitingModel.joinedPlayers()

        gameview.player1Name = players.player1.name
        gameview.player2Name = players.player2.name
        mainarea.state = "GAME"

        var msgToSend = {action: "MoveToState", state: "PLAY_GAME_MY_TURN"}
        playersManager.sendPlayerMessage(players.player1.id, Messages.createCustomAppBoxMsg(msgToSend))

        msgToSend = {action: "MoveToState", state: "PLAY_GAME_WAIT_TURN"}
        playersManager.sendPlayerMessage(players.player2.id, Messages.createCustomAppBoxMsg(msgToSend))

        // to other players
        var allIds = playersManager.playerIds()
        msgToSend = {action: "MoveToState", state: "WAIT_GAME"}
        var msg = Messages.createCustomAppBoxMsg(msgToSend)

        for (var i = 0; i < allIds.length; i++) {
            if (allIds[i] !== players.player1.id && allIds[i] !== players.player2.id) {
                playersManager.sendPlayerMessage(allIds[i], msg)
            }
        }
    }

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        GameModel.initialize(playersManager)
        PlayerWaitingModel.initialize(playersManager)

        joinview.secondPlayerJoinedAnimationFinished.connect(onSecondPlayerJoinedAnimationFinished)

        //GaneModel.callbacks.playerWon.connect(onPlayerWon)
        //GaneModel.callbacks.playerCorrectNumber.connect(onPlayerCorrectNumber)
        //GaneModel.callbacks.playerInvalidNumber.connect(onPlayerInvalidNumber)

        AppBoxMaster.loadAppBoxResources("qrc:/appbox/AppBox.qml")

        // testing dialog
        //playedwonDialog.declarePlayerWon(0, 0)

        //joinview.joinFirstPlayer("abc")
        //joinview.joinSecondPlayer("foobar")
        //GameModel.initGame(1, 2)
    }
}
