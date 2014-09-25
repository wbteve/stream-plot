/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */

var streamplot = (function() {
    var Plot = function(canvasId) {
        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext("2d");

        var w = canvas.width;
        var h = canvas.height;

        var bufCanvas = document.createElement('canvas');
        bufCanvas.width = w;
        bufCanvas.height = h;
        var bufCtx = bufCanvas.getContext("2d");

        var t = 1;

        return {
            'draw': function() {
                bufCtx.clearRect(0, 0, w, h);
                bufCtx.beginPath();
                for(var i=0; i < 100; i++) {
                    bufCtx.moveTo(50, 50);
                    bufCtx.lineTo(100, 100);
                }
                bufCtx.stroke();
                ctx.drawImage(bufCanvas, t, 0, w-t, h, 0, 0, w-t, h);
                ctx.drawImage(bufCanvas, 0, 0, t, h, w-t, 0, t, h);

                t++;
                if(t >= w)
                    t = 1;
            },
            'clear': function() {
                ctx.clearRect(0, 0, w, h);
            }
        }
    }
    return {
        'Plot': Plot
    }
})();

