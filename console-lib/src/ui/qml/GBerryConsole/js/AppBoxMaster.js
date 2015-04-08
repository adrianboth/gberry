.import GBerry.Log 1.0 as Log
//Log.initLog("AppBoxMaster", gsettings.logLevel)

var _appboxResourceString

function loadAppBoxResources(appboxMainQmlUrl) {
    Log.initLog("AppBoxMaster", gsettings.logLevel)

    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        //console.debug("### xhr.status: " + xhr.status)

        if (xhr.readyState === XMLHttpRequest.DONE) {
            Log.trace("XHR DONE")
            var obj = xhr.responseText.toString()
            if (obj.length > 0) {
                Log.trace("Loaded data: " + obj)
                _appboxResourceString = obj
            } else {
                Log.error("Got empty content")
            }

        }
    }


    xhr.open("GET", appboxMainQmlUrl)
    xhr.send()
}

function dataStr() {
    return _appboxResourceString
}
