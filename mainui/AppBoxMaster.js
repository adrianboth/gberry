var _appboxResourceString

function loadAppBoxResources() {
    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        console.debug("### xhr.status: " + xhr.status)

        if (xhr.readyState === XMLHttpRequest.DONE) {
            console.debug("XHR DONE")
            var obj = xhr.responseText.toString()
            if (obj.length > 0) {
                console.debug("### LOCAL FILE: " + obj)
                _appboxResourceString = obj
            }
        }
    }

    xhr.open("GET", "appbox/AppBox.qml")
    xhr.send()
}

function dataStr() {
    return _appboxResourceString
}
