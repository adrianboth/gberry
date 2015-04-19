.import GBerry 1.0 as GBerry

var _todaysFastest;

function hasTodaysFastest() {
    return typeof(_todaysFastest) != 'undefined'
}

function todaysFastest() {
    return _todaysFastest
}

function recordWinningResult(name, time) {
    if (typeof(_todaysFastest) == 'undefined' || _todaysFastest.time > time) {
        GBerry.Log.debug("New todays fastest: name=" + name + ", time=" + time.toString())
        _todaysFastest = new HighScore(name, time) // store time as real
    }
}

function HighScore(name, time) {
    this.name = name
    this.time = time
}
