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

/**
 * Returns a random integer between min (inclusive) and max (inclusive)
 * Using Math.round() will give you a non-uniform distribution!
 */
function _getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

/**
 * Returns shuffled list of a, b ,c ,d
 */
function _randomAnswerOrder(answersCount) {
    var all = ["a", "b", "c", "d"].slice(0, answersCount)
    var newOrder = []
    var i
    while (all.length > 0) {
        i = _getRandomInt(0, all.length -1)
        newOrder.push(all[i])
        all.splice(i, 1)
    }

    return newOrder
}

function _defined(obj) {
    return typeof(obj) !== "undefined"
}

function _answersCount(q) {
    if (_defined(q.d))
        return 4
    if (_defined(q.c))
        return 3
    if (_defined(q.b))
        return 2
    if (_defined(q.a))
        return 1

    return 0
}

function setupGame(maxQuestionsCount) {
    // TODO: shuffle (both questions and answers)
    var tempQ = _allQuestions.slice(0)
    _questions = []
    var i
    var answerIndex
    var answerShuffleList
    var questionOld
    var questionNew
    var refAnswers = ["a", "b", "c", "d"]

    while (_questions.length < _allQuestions.length && _questions.length < maxQuestionsCount) {

        i = _getRandomInt(0, tempQ.length -1)
        questionOld = tempQ[i]
        answerShuffleList = _randomAnswerOrder(_answersCount(questionOld))
        questionOld = tempQ[i]
        questionNew = {}
        questionNew.question = questionOld.question
        if (_defined(questionOld.image))
            questionNew.image = questionOld.image

        if (_defined(answerShuffleList.length > 0))
            questionNew.a = questionOld[answerShuffleList[0]]

        if (_defined(answerShuffleList.length > 1))
            questionNew.b = questionOld[answerShuffleList[1]]

        if (_defined(answerShuffleList.length > 2))
            questionNew.c = questionOld[answerShuffleList[2]]

        if (_defined(answerShuffleList.length > 3))
            questionNew.d = questionOld[answerShuffleList[3]]

        // adjust answer
        answerIndex = answerShuffleList.indexOf(questionOld.answer)
        questionNew.answer = refAnswers[answerIndex]

        _questions.push(questionNew)
        tempQ.splice(i, 1)

        //console.debug("### new question: " + questionNew.toString())
    }

    console.debug("### questions: " + JSON.stringify(_questions))

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
