.pragma library

var _allQuestions
var _questions
var _currentQuestion
var _questionsMaxCount
var _questionsCount // number of question currently going to be answered

function initialize() {
    // TODO: from disk
    _questions = [{"question": "Foo question", "a": "aa", "b": "b"},
                  {"question": "Bar question", "a": "aa", "b": "b"}
                 ]
}

function setupGame(maxQuestionsCount) {
    // TODO: shuffle
    _questions = _allQuestions
    _questionsMaxCount = maxQuestionsCount
    _questionsCount = 0

}

function currentQuestion() {
    return _questions[_questionsCount]
}

function moveToNextQuestion() {
    if (_questionsCount + 1 === _questions.length) {
        // TODO: signal game end
        console.debug("No more questions - game ended")
    } else {
        _questionsCount++
    }
}

// TODO: signal about game end
// TODO: how signal about player(s) scoring a point
