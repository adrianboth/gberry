import QtQuick 2.3
import GBerry 1.0

GConfirmationDialog {
    id: self
    visible: false
    questionText: "xxx"
    option1Id: "Yes"
    option1Text: qsTr("Yes")
    option2Id: "No"
    option2Text: qsTr("No")


    border.color: "black"
    border.width: 2

    signal newGameRequested()
    signal menuRequested()

    // time: how much time it was used to complete game
    function declarePlayerWon(highestScore, maxPoints, time) {
        //var name = playersManager.playerName(pid)
        var name = highestScore["name"]
        var pointsStr = highestScore["points"].toString() + "/" + maxPoints.toString()
        // TODO: localization of this is screwed!!!FIX
        self.questionText = qsTr("Player " + name + " won!\n\nPoints " + pointsStr + "\n\nPlay another?")
        self.visible = true

        // note that we are not showing exactly same info in question that
        // on client side
        sendConfirmationToPlayers()

    }

    function declareDraw(highestScores, maxPoints) {
        var playersStr = "Draw between "
        for (var i = 0; i < highestScores.length; i++) {
            playersStr += highestScores[i]["name"]

            if (i + 2 < highestScores.length) { // this is second last, or earlier
                playersStr += ","
            } else if (i + 1 < highestScores.length) { // this last
                playersStr += " and "
            }
        }
        var pointsStr = highestScores[0]["points"].toString() + "/" + maxPoints.toString()
        // TODO: localization of text
        self.questionText = playersStr + ". Points " + pointsStr + "\n\nPlay another?"
        self.visible = true

        // note that we are not showing exactly same info in question that
        // on client side
        sendConfirmationToPlayers()

    }

    function sendConfirmationToPlayers() {
        var clientQuestionText = qsTr("Play another?")
        // TODO: GConfirmationDialog could have a helper ...
        var js = {action: "ConfirmationQuestion",
                  questionId: "playanother", // TODO: client needs to return this
                  title: self.titleText,
                  text: clientQuestionText,
                  options: [{id:   self.option1Id,
                             text: self.option1Text},
                            {id:   self.option2Id,
                             text: self.option2Text}]
                 }

        playersManager.sendAllPlayersMessage(JSON.stringify(js))
    }

    onOption1Selected: {
        // yes
        self.visible = false
        console.debug("New game requested")
        newGameRequested()
    }

    onOption2Selected: {
        // no
        self.visible = false
        console.debug("Menu requested")
        menuRequested()
    }
}
