.pragma library

function createCloseQuestionMsg() {
    var js = {action: "CloseQuestion"}
    return JSON.stringify(js)
}

function createShowAppBoxMsg() {
    return JSON.stringify({action: "ShowAppBox"})
}

// CustomAppBox message carries whatever json message to appbox on client side
function createCustomAppBoxMsg(appboxJS) {
    var js = {action: "AppBoxMessage", data: appboxJS}
    return JSON.stringify(js)
}

var CLOSE_QUESTION_MSG = createCloseQuestionMsg()
