/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */
$(document).ready(function() {
    plot1 = streamplot.Plot("myCanvas", 
            { 
                'nChannels': 2,
                'grid': true 
            });

    function render() {
        plot1.clear();
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
