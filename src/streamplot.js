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

        var w = 100;
        var h = 100;

        var m_canvas = document.createElement('canvas');
        m_canvas.width = w;
        m_canvas.height = h;
        var m_context = m_canvas.getContext('2d');
        var canvasData = m_context.getImageData(0, 0, w, h);

        return {
            'draw': function(x0, y0, x1, y1) {
                /*
                   http://stackoverflow.com/questions/4899799/whats-the-best-way-to-set-a-single-pixel-in-an-html5-canvas
                   http://jsperf.com/setting-canvas-pixel
                   http://stackoverflow.com/questions/7812514/drawing-a-dot-on-html5-canvas
                   https://hacks.mozilla.org/2009/06/pushing-pixels-with-canvas/
                   */
                /*
                   ctx.beginPath();
                   for(var i=0;i < 1000; i++) {
                   ctx.moveTo(x0, y0);
                   ctx.lineTo(x1, y1);
                   }
                   ctx.stroke();
                   ctx.closePath();
                   */
                /*
                   var i;
                   for(i = 0;i < 10000; i++) {
                   ctx.fillRect(x0, y0, 1, 1);
                   }
                   */
                /*
                   var canvasData = ctx.getImageData(0, 0, canvas.width, canvas.height);
                   var w = canvas.width;
                   var h = canvas.height;
                   for (var x = 0; x < 100; x++)  {
                   for (var y = 0; y < 100; y++)  {

                // Index of the pixel in the array
                var idx = (x + y * w) * 4;

                // If you want to update the values of the pixel
                canvasData.data[idx + 0] = 0; // Red channel
                canvasData.data[idx + 1] = 0; // Green channel
                canvasData.data[idx + 2] = 255; // Blue channel
                canvasData.data[idx + 3] = 255; // Alpha channel
                }
                }
                ctx.putImageData(canvasData, 0, 0);
                */

                // Off-screen rendering seems to help a lot
                for(var x = 0; x < w; x++) {
                    for(var y = 0; y < h; y++) {
                        // Index of the pixel in the array
                        var idx = (x + y * w) * 4;

                        // If you want to update the values of the pixel
                        canvasData.data[idx + 0] = 0; // Red channel
                        canvasData.data[idx + 1] = 0; // Green channel
                        canvasData.data[idx + 2] = 255; // Blue channel
                        canvasData.data[idx + 3] = 255; // Alpha channel
                    }
                }
                m_context.putImageData(canvasData, 0, 0);
                ctx.drawImage(m_canvas, x0, y0);

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

