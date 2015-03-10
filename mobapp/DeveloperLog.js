/*
 * DeveloperLog provides logging for tracking QML & Javascript execution.
 *
 * DeveloperLog supports
 *  - log levels
 *  - logging areas (component name)
 *
 * This is statefull library and it needs to be initialized everywhere
 * where it is included.
 *
 * It is recommended to define log level once in your application. Because
 * DeveloperLog.js is statefull you need to provide log level for your
 * *.js modules.
 *
 * For example:
 *     // usefull in *.js files
 *     var log
 *
 *     function init(logLevel) {
 *         log = new DeveloperLog("GameLogic", logLevel)
 *     }
 *
 * For *.qml files following suits better:
 *     import "DeveloperLogic.js" as Log
 *
 *     ...
 *     Component.onCompleted: {
 *         Log.initLog("MainView", logLevel)
 *     }
 *
 *     // usage
 *     Log.debug("This is a message")
 *
 */
var TRACE_LEVEL = 0
var DEBUG_LEVEL = 1
var INFO_LEVEL = 2
var WARNING_LEVEL = 3
var ERROR_LEVEL = 4

var _startDate = new Date()
var _log // yet undefined


function initLog(componentName, logLevel) {
    if (logLevel < TRACE_LEVEL || logLevel > ERROR_LEVEL) {
        console.error("ERROR: Invalid log level " + level)
    }

    //console.debug("Log level: " + logLevel)
    _log = new DeveloperLog(componentName, logLevel)
    return _log
}

function trace(msg) { _log.trace(msg) }
function debug(msg) { _log.debug(msg) }
function info(msg) { _log.info(msg) }

// If you create DeveloperLog directly you can use arguments more freely.
// They are concatenated with space.
//
function DeveloperLog(componentName, logLevel) {
    this.componentName = componentName
    this.logLevel = logLevel

    this.trace = function() {
        if (this.logLevel <= TRACE_LEVEL) {
            console.debug(this._formatMsg(this.componentName, "TRACE", this._concatMsg(arguments)))
        }
    }

    this.debug = function() {
        if (this.logLevel <= DEBUG_LEVEL) {
            console.debug(this._formatMsg(this.componentName, "DEBUG", this._concatMsg(arguments)))
        }
    }

    this.info = function() {
        if (this.logLevel <= INFO_LEVEL) {
            console.info(this._formatMsg(this.componentName, "INFO", this._concatMsg(arguments)))
        }
    }

    this._concatMsg = function(args) {
        var msg = ""
        for (var i =0; i < args.length; i++) {
            if (msg.length > 0) {msg += " "}
            msg += args[i]
        }
        return msg
    }

    this._formatMsg = function(component, level, msg) {
        var now = new Date()
        var sinceStartMs = now.getTime() - _startDate.getTime()
        var secs = Math.floor(sinceStartMs / 1000)
        var ms = sinceStartMs % 1000
        return secs.toString() + "." + ms + " " + level +" [" + component + "] " + msg
    }
}
