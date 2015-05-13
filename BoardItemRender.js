.pragma library

// This library prerenders board item images ('x' and 'o') and later draws them.

var _crossImage
var _circleImage

// prerender needs to be called once and only once before any draw*() call
function prerender(canvas) {
    console.debug("### PRERENDER")
    //var ctx = canvas.getContext("2d")
    prerenderCrossImage(canvas)
    prerenderCircleImage(canvas)
}

function drawCross(canvas) {
    var ctx = canvas.getContext("2d")
    ctx.drawImage(_crossImage, 0, 0)
}

function prerenderCrossImage(canvas) {
    var ctx = canvas.getContext('2d')
    ctx.save()
    ctx.clearRect ( 0 , 0 , canvas.width, canvas.height );

    // setup the stroke
    ctx.lineWidth = 5
    ctx.strokeStyle = "blue"

    ctx.beginPath()
    ctx.moveTo(0 + canvas.crossMarginX, 0 + canvas.crossMarginY)
    ctx.lineTo(canvas.width - canvas.crossMarginX, canvas.height - canvas.crossMarginY)
    ctx.stroke()

    ctx.beginPath()
    ctx.moveTo(canvas.width - canvas.crossMarginY, 0 + canvas.crossMarginY)
    ctx.lineTo(0 + canvas.crossMarginX, canvas.height - canvas.crossMarginY)
    ctx.stroke()

    ctx.restore()

    _crossImage = ctx.createImageData(canvas.width, canvas.height)
}

function prerenderCircleImage(canvas) {

}
