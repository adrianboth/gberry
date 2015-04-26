.pragma library

function createGeneralActionMessage(actionId) {
    var js = {action: "GeneralAction", id: actionId}
    return JSON.stringify(js)
}

// CustomAppBox message carries whatever json message to appbox on client side
function createCustomAppBoxMsg(appboxJS) {
    var js = {action: "AppBoxMessage", data: appboxJS}
    return JSON.stringify(js)
}
