/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */

var streamplot = (function() {
    var Plot = function(canvasId, params) {
        // params->{ 'bufSize': 100, 'styles': [], 'colors': [], nChannels: 2, title:"", xLabel:"", yLabel:"", grid: True, 'autoZoom': True }

        var defaultColors = ["#ff0000", "#00ff00", "#0000ff"];
        // TODO: default colors, styles

        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext("2d");
        var w = canvas.width;
        var h = canvas.height;

        var bufCanvas = document.createElement('canvas');
        bufCanvas.width = w;
        bufCanvas.height = h;
        var bufCtx = bufCanvas.getContext("2d");

        var t = 1;

        return 
        {
            'addData': function(t, vals) {
                // t is time, and vals is an array of length nChannel
            },
            'zoomX': function(trange) {
                // set the range along x-axis (10 secs or 1 sec or whatever). Applicable only when auto zoom is enabled.
            },
            'clear': function() {
                // Clears the plot
                ctx.clearRect(0, 0, w, h);
            },

            //////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////// setter functions ///////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            'setRollMode': function(en) {
                // en==true puts plot into rolling mode. Need to enable auto-zoom for it to work.
            }
            'setAutoZoom': function(enX, enY) {
                // en==true enables auto zoom along that axis
            }
            'setViewArea': function(xStart, xStop, yStart, yStop) {
                // set the current view area. These params are ignored when autoZoom is enabled
            },
            'setGrid': function(en) {
                // en==true enables the grid, otherwise the grid is hidden
            },
            'setLabels': function(params) {
                // params = {'xLabel': "" , 'yLabel': "", 'title': "" }
            },
            'setColors': function(colors) {
                // colors is an array
            },
            'setBufferSize': function(size) {
                // set the buffer size to size
            },

            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////// getter functions ////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            
            'getAutoZoom': function() {
                // returns true or false
            },
            'getViewArea' : function() {
                // returns view area
            },
            'getBufferSize': function() {
                // return current buffer size
            },
            'getColors': function() {
                // returns array of colors
            },
            'getGrid': function() {
                // returns True if grid is enabled
            }
        }
    }
}
return {
    'Plot': Plot
}
})();

