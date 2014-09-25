/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   Contact the author if you want to use this work under a different license.
*/

$(document).ready(function() {
    plot1 = streamplot.Plot("myCanvas", 
            { 
                'nChannels': 2,
                'grid': true,
                'zoomMode': 
                {
                    'xZoomMode': "roll",
                    'yZoomMode': "auto"
                } 
            });

    plot1.addData(0, [1, 3]);
    plot1.addData(100, [200, 500]);
    plot1.addData(200, [300, 400]);

});
