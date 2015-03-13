.pragma library

function createCloseQuestionMsg() {
    var js = {action: "CloseQuestion"}
    return JSON.stringify(js)
}

var CLOSE_QUESTION_MSG = createCloseQuestionMsg()




