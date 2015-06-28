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
    width: gdisplay.windowWidth
    height: gdisplay.windowHeight

    ApplicationSettings { id: gsettings }
    GDisplayProfile { id: gdisplay }

    Timer {
        id: devTimer
        repeat: false
        running: false
        property real startTime: 0

        function start() {
            startTime = new Date().valueOf()
        }

        function startWithEventQueueMarker() {
            start()
            running = true
        }

        function printTime() {
            var totalTime = new Date().valueOf() - startTime
            console.debug("###### DevTimer: current " + totalTime.toString() + " ms ######")
        }

        onTriggered: {
            var totalTime = new Date().valueOf() - startTime
            console.debug("###### DevTimer: final " + totalTime.toString() + " ms ######")
        }
    }

    FontLoader { id: defaultFont
                 //name: "FreeSans"
                 source: "file:/usr/share/fonts/truetype/freefont/FreeSans.ttf"}

    // invisible area that can be used during dev time to exit app
    MouseArea {
        id: topRightExitArea
        anchors.top: parent.top; anchors.right: parent.right
        width: 25; height: 25
        onClicked: Qt.quit()
    }

    Rectangle {
        id: mainarea
        anchors.fill: parent
        color: "steelblue"
        /* Gradient doesn't look good on Raspi
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }
        */

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

        js = {action: "DefineAppBox", data: AppBoxMaster.dataStr()}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        // in our case all actions happen in appbox -> we can show it all the time
        var js = {action: "ShowAppBox"}
        playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        if (mainarea.state === "WAITING_PLAYERS") {
            js = {action: "MoveToState", state: "WAITING_PLAYERS"}
            playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(js))

            js = {action: "DefineGeneralActions",
                      actions: [{actionId: "ExitGame", actionName: "Exit Game"}]}
            playersManager.sendPlayerMessage(pid, JSON.stringify(js))

        } else if (mainarea.state === "GAME") {
            js = {action: "MoveToState", state: "WAIT_GAME"}
            playersManager.sendPlayerMessage(pid, Messages.createCustomAppBoxMsg(js))

            js = {action: "DefineGeneralActions",
                      actions: [{actionId: "AbortGame", actionName: "Abort Game"}]}
            playersManager.sendPlayerMessage(pid, JSON.stringify(js))
        }

        // TODO: what about end game view
    }

    function onPlayerOut(pid)
    {
        // TODO:
        var msgToSend, msg
        // if waiting -> then drop that player out
        // TODO: it would be great to have temporary message what happened (just on main screen?)
        if (mainarea.state === "WAITING_PLAYERS" && PlayerWaitingModel.hasJoined(pid)) {
            resetWaitingPlayersState()

            msg = qsTr("Joined player left the game.")
            msgToSend = {action: "FeedbackMessage",
                         message: msg}
            playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
            feedbackDialog.show(msg)

        } else if (mainarea.state === "GAME" && GameModel.isPlaying(pid)) {
            mainarea.state = "WAITING_PLAYERS"
            msg = qsTr("Game abort because player left the game.")
            msgToSend = {action: "FeedbackMessage",
                         message: msg}
            playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
            feedbackDialog.show(msg)
        }

        // if player in game -> cancel the game -> to beginning (TODO: what kind of message?)

        console.log("Player left: id = " + pid)
    }

    function resetWaitingPlayersState() {
        PlayerWaitingModel.reset()
        joinview.reset()
        var msgToSend = {action: "MoveToState", state: "WAITING_PLAYERS"}
        playersManager.sendAllPlayersMessage(Messages.createCustomAppBoxMsg(msgToSend))
        msgToSend = {action: "DefineGeneralActions",
                     actions: [{actionId: "ExitGame", actionName: "Exit Game"}]}
        playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
    }

    function onPlayerMessageReceived(pid, data)
    {
        console.log("Player message: id = " + pid)
        var js  = JSON.parse(data)
        var msgToSend, msg

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

                var msg = qsTr("Player '" + playersManager.playerName(pid) + "' aborted the game.")
                msgToSend = {action: "FeedbackMessage",
                             message: msg}
                playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))

                feedbackDialog.show(msg)

            } else if (js["id"] === "ExitGame") {
                msgToSend = {action: "FeedbackMessage",
                             message: qsTr("Player '" + playersManager.playerName(pid) + "' selected Exit Game.")}
                playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
                Qt.quit()
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

                msg = qsTr("Player '" + playersManager.playerName(pid) + "' cancelled joining.")
                msgToSend = {action: "FeedbackMessage",
                             message: msg}
                playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
                feedbackDialog.show(msg)

            }
        }
    }

    // when animation starts mobapp states are changed to waiting
    function onSecondPlayerJoinedAnimationStarted() {
        var players = PlayerWaitingModel.joinedPlayers()

        // both joined player will wait turn (=game to start)
        var msgToSend = Messages.createCustomAppBoxMsg({action: "MoveToState", state: "PLAY_GAME_WAIT_TURN"})
        playersManager.sendPlayerMessage(players.player1.id, msgToSend)
        playersManager.sendPlayerMessage(players.player2.id, msgToSend)


        // to other players, wait game to finish
        var allIds = playersManager.playerIds()
        msgToSend = {action: "MoveToState", state: "WAIT_GAME"}
        var msg = Messages.createCustomAppBoxMsg(msgToSend)

        for (var i = 0; i < allIds.length; i++) {
            if (allIds[i] !== players.player1.id && allIds[i] !== players.player2.id) {
                playersManager.sendPlayerMessage(allIds[i], msg)
            }
        }

        msgToSend = {action: "DefineGeneralActions",
                     actions: [{actionId: "AbortGame", actionName: "Abort Game"}]}
        playersManager.sendAllPlayersMessage(JSON.stringify(msgToSend))
    }

    // once animation done, start playing
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
    }

    // hide inactive mouse
    MouseArea {
        id: hideMouseArea
        anchors.fill: parent
        cursorShape: Qt.BlankCursor
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            cursorShape = Qt.ArrowCursor
            hideMouseCursorTimer.restart()
        }

        Timer {
            id: hideMouseCursorTimer
            running: false
            interval: 10000 // 10 seconds
            repeat: false
            onTriggered: hideMouseArea.cursorShape = Qt.BlankCursor
        }

    }

    GFeedbackDialog {
        id: feedbackDialog
        visible: false
        //feedbackMessage: "This is a test message, quite long This is a test message, quite long This is a test message, quite long"
        showingTime: 3000 // ms
        height: preferredHeight
        textPixelSize: gdisplay.mediumSize * gdisplay.ppmText
        initialOpacity: 0.6
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Component.onCompleted: {
        Log.initLog("main", gsettings.logLevel)
        playersManager.playerIn.connect(onPlayerIn)
        playersManager.playerOut.connect(onPlayerOut)
        playersManager.playerMessageReceived.connect(onPlayerMessageReceived)

        GameModel.initialize(playersManager)
        PlayerWaitingModel.initialize(playersManager)

        joinview.secondPlayerJoinedAnimationStarted.connect(onSecondPlayerJoinedAnimationStarted)
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

        //feedbackDialog.show("test message.")
    }

}
