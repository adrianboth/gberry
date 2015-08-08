import QtQuick 2.4
import QtQuick.Layouts 1.1

Item {
    id: self

    function showAnswers(answers) {
        // TODO: put text and hide rows if not needed
    }

    ColumnLayout {
        anchors.fill: parent

        AnswerRow {
            id: answerRowA
            answerId: "A"
            answerText: "Test A"

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowB
            answerId: "B"
            answerText: "Test B"

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowC
            answerId: "C"
            answerText: "Test C"

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowD
            answerId: "D"
            answerText: "Test D"

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: answerClicked(answerId)
        }
    }

    // -------------------------------------------------------------------------

    function answerClicked(answerId) {
        console.debug("Answer " + answerId + " was selected by mouse click")
    }
}
