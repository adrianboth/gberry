.pragma library

var _allQuestions
var _questions
var _currentQuestion
var _questionsMaxCount
var _questionsCount // number of question currently going to be answered
var _questionIdIndex = 0

var signals = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal gameEnded(); signal moveToNextQuestion(); }', Qt.application, 'QuestionsModelProperties');

var properties = Qt.createQmlObject('import QtQuick 2.0; QtObject { property int maxQuestionsCount: 0; property int currentQuestionIndex: 0; }', Qt.application, 'QuestionsModelProperties');

function initialize(questionsListJson) {
    // TODO: from disk
    _allQuestions = questionsListJson
    /*
    _allQuestions = [{"question": "Foo question", "a": "1aa", "b": "2b", "c": "3c", "d": "4d", "answer": "b"},
                     {"question": "Bar question", "a": "aa", "b": "b", "answer": "a"}
                    ]
    */
}

function setupGame(maxQuestionsCount) {
    // TODO: shuffle (both questions and answers)
    _questions = _allQuestions
    _questionsMaxCount = _questions.length
    _questionsCount = 0

    properties.maxQuestionsCount = _questions.length
    properties.currentQuestionIndex = _questionsCount + 1

    _questionIdIndex++

}

function currentQuestion() {
    // TODO: shuffle and store shuffled questio
    var q = _questions[_questionsCount]
    q["question_id"] = _questionIdIndex
    return q
}

function moveToNextQuestion() {
    _questionIdIndex++

    if (_questionsCount + 1 === _questions.length) {
        // TODO: signal game end
        console.debug("No more questions - game ended")
        signals.gameEnded()
    } else {
        _questionsCount++
        properties.currentQuestionIndex = _questionsCount + 1
        signals.moveToNextQuestion()
    }
}

// TODO: signal about game end
// TODO: how signal about player(s) scoring a point
