/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */

dd = null;

var streamplot = (function() {
    var Plot = function(canvasId) {
        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext("2d");
        return {
            'draw': function(x0, y0, x1, y1) {
                ctx.beginPath();
                ctx.moveTo(x0, y0);
                ctx.lineTo(x1, y1);
                ctx.stroke();
            },
            'clear': function() {
                ctx.clearRect(0, 0, canvas.width, canvas.height);
            }
        }
    }
    return {
        'Plot': Plot
    }
})();

