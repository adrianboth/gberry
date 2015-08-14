import QtQuick 2.4

import GBerry 1.0

GConfirmationDialog {
    id: self
    visible: false // initial state

    property string questionId: "exitgame"

    questionText: qsTr("Are you sure to exit this game?")
    option1Text: qsTr("Yes")
    option2Text: qsTr("No")

    signal cancelSelected()

    onOption1Selected: {
        // Yes
        Qt.quit()
    }

    onOption2Selected: {
        // No
        self.visible = false
        cancelSelected()
    }

    function show() {
        var js = {action: "ConfirmationQuestion",
                  questionId: "exitgame",
                  title: self.titleText,
                  text: self.questionText,
                  options: [{id:   self.option1Id,
                             text: self.option1Text},
                            {id:   self.option2Id,
                             text: self.option2Text}]
                 }

        // TODO: should be send only for controlling player but that is not yet implemented)
        //    * Now for first player

        // TODO: gets really strange player id list (not list at all??)
        //playersManager.sendPlayerMessage(playersManager.playerIds(), JSON.stringify(js))
        playersManager.sendAllPlayersMessage(JSON.stringify(js))

        // show also on big screen the question
        self.visible = true
        self.focus = true

    }

    function processResponse(ref) {
        if (self.visible) {
            selectOption(ref)

            // just to make sure everyones dialogs are closed
            playersManager.sendAllPlayersMessage(MessagesJS.CLOSE_QUESTION_MSG)
        }
    }

    Keys.onPressed: {
        console.debug("######## KEY PRESSED (ExitConfirmationDialog)")
        if (event.key === Qt.Key_Return)
            selectOption(option1Id)
        if (event.key === Qt.Key_Escape)
            selectOption(option2Id)
    }
}
