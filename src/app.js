/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */
$(document).ready(function() {
    plot1 = streamplot.Plot("myCanvas");

    var x0 = y0 = 100;
    var x1 = y1 = 200;
    function render() {
        plot1.clear();
        plot1.draw(x0, y0, x1, y1);

        x0 += 1;
        x1 += 1;
        if(x0 == 500) {
            x0 = 100;
            x1 = 200;
        }
    }

    window.requestAnimFrame = (function(){
        return  window.requestAnimationFrame       ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame    ||
        function( callback ){
            window.setTimeout(callback, 1000 / 60);
        };
    })();

    (function animloop(){
        requestAnimFrame(animloop);
        render();
    })();

});
