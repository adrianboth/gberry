//.pragma library

/* Player object */
function Player(pid, pname) {
    this.id = pid
    this.name = pname

    this.isValid = function() {
        return true
    }
}

function UndefinedPlayer() {
    this.id = 0
    this.name = "<undefined>"

    this.isValid = function() {
        return false
    }
}

/* Composition: Players of the game */
function Players(__player1, __player2) {
    this.player1 = __player1
    this.player2 = __player2
}
