.pragma library

function createGeneralActionMessage(actionId) {
    var js = {action: "GeneralAction", id: actionId}
    return JSON.stringify(js)
}

