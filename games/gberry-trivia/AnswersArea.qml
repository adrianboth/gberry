import QtQuick 2.4
import QtQuick.Layouts 1.1

Item {
    id: self

    signal devAnswerClicked(var answerId)

    function showAnswers(answers) {
        // TODO: put text and hide rows if not needed
    }

    ColumnLayout {
        anchors.fill: parent

        AnswerRow {
            id: answerRowA
            answerId: "a"
            answerText: currentQuestion.answer1
            showAnswer: currentQuestion.showAnswer1

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: self.answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowB
            answerId: "b"
            answerText: currentQuestion.answer2
            showAnswer: currentQuestion.showAnswer2

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: self.answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowC
            answerId: "c"
            answerText: currentQuestion.answer3
            showAnswer: currentQuestion.showAnswer3

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: self.answerClicked(answerId)
        }

        AnswerRow {
            id: answerRowD
            answerId: "d"
            answerText: currentQuestion.answer4
            showAnswer: currentQuestion.showAnswer4

            Layout.fillWidth: true
            Layout.preferredHeight: preferredHeight

            onAnswerClicked: self.answerClicked(answerId)
        }
    }

    // -------------------------------------------------------------------------

    function answerClicked(answerId) {
        console.debug("Answer " + answerId + " was selected by mouse click")
        devAnswerClicked(answerId)
    }
}
