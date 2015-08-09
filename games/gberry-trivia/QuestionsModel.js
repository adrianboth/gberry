.pragma library

var _allQuestions
var _questions
var _currentQuestion
var _questionsMaxCount
var _questionsCount // number of question currently going to be answered

var signals = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal gameEnded(); signal moveToNextQuestion(); }', Qt.application, 'QuestionsModelProperties');

var properties = Qt.createQmlObject('import QtQuick 2.0; QtObject { property int maxQuestionsCount: 0; property int currentQuestionIndex: 0; }', Qt.application, 'QuestionsModelProperties');

function initialize() {
    // TODO: from disk
    _allQuestions = [{"question": "Foo question", "a": "1aa", "b": "2b", "c": "3c", "d": "4d", "answer": "b"},
                     {"question": "Bar question", "a": "aa", "b": "b", "answer": "a"}
                    ]
}

function setupGame(maxQuestionsCount) {
    // TODO: shuffle (both questions and answers)
    _questions = _allQuestions
    _questionsMaxCount = _questions.length
    _questionsCount = 0

    properties.maxQuestionsCount = _questions.length
    properties.currentQuestionIndex = _questionsCount + 1

}

function currentQuestion() {
    // TODO: shuffle and store shuffled questio
    return _questions[_questionsCount]
}

function moveToNextQuestion() {
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
