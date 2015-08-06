/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 .import GBerry.Log 1.0 as Log
//Log.initLog("AppBoxMaster", gsettings.logLevel)

var _appboxResourceString

function loadAppBoxResources(appboxMainQmlUrl) {
    Log.initLog("AppBoxMaster", Log.DEBUG)

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
