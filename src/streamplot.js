/*
   stream-plot

   Copyright (c) 2014 Sagar Gubbi <sagar.writeme@gmail.com>

   This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
   See http://creativecommons.org/licenses/by-nc/4.0/ for more details.
   */

var streamplot = (function() {
    var defaultColors = ['#7cb5ec', '#434348', '#90ed7d', '#f7a35c', '#8085e9', '#f15c80', '#e4d354', '#8085e8', '#8d4653', '#91e8e1'];
    var Plot = function(canvasId, params) {
        /* params->
         { 'bufSize': number, nChannels: num, 
           'styles': [], 'colors': [], 'thickness': []
           'title': "", 'xLabel': "", 'yLabel': "", 'grid': bool, 
           'zoomMode': hash, 'XInterval': num, 'viewArea': hash }
        */
        // Default values for unspecified params
        var params = params || {};
        var nChannels = params['nChannels'] || 1;
        var bufSize = params['bufSize'] || 10000;

        var title = params['title'] || "";
        var xLabel = params['xLabel'] || "";
        var yLabel = params['yLabel'] || "";
        
        var grid = params['grid'] || false;
        var zoomMode = params['zoomMode'] || { 'xZoomMode': "auto", 'yZoomMode': "auto" };
        var xInterval = params['xInterval'] || 10;
        var viewArea = params['viewArea'] || { 'xStart': 0, 'xStop': 10, 'yStart': 0, 'yStop': 1024 };

        var styles = params['styles'] || Array.apply(null, new Array(nChannels)).map(function(ele) { return 'l'; }); // 'l' or 'p'
        var thickness = params['thickness'] || Array.apply(null, new Array(nChannels)).map(function(ele) { return 1.0; });
        var colors = params['colors'] || Array.apply(null, new Array(nChannels)).map(function(e, ind) { return defaultColors[ind % defaultColors.length]; }); 
        
        // Get context of given canvas
        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext("2d");
        var w = canvas.width;
        var h = canvas.height;
        
        // Create an off-screan buffer context
        var bufCanvas = document.createElement('canvas');
        bufCanvas.width = w;
        bufCanvas.height = h;
        var bufCtx = bufCanvas.getContext("2d");

        return {
            'addData': function(t, vals) {
                // t is time, and vals is an array of length nChannel
            },
            'clear': function() {
                // Clears the plot
                ctx.clearRect(0, 0, w, h);
            },

            //////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////// setter functions ///////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            'setZoomMode': function(zMode) {
                // zoomMode = { 'xZoomMode': 'manual' or 'auto' or 'roll' or 'overwrite',
                //              'yZoomMode': 'manual' or 'auto' }
                zoomMode = zMode;
            },
            'setXInterval': function(tDiff) {
                // Works only when xZoom mode is 'overwrite' or 'roll'.
                xInterval = tDiff;
            },
            'setViewArea': function(vArea) {
                // set the current view area {'xStart': num, 'xStop': num, 'yStart': num, 'yStop': num }. 
                // This works when 'manual' zoom is enabled for either axis.
                viewArea = vArea;
            },
            'setGrid': function(en) {
                // en==true enables the grid, otherwise the grid is hidden
                grid = en;
            },
            'setLabels': function(params) {
                // params = {'xLabel': "" , 'yLabel': "", 'title': "" }
                xLabel = params['xLabel'] || "";
                title = params['title'] || "";
                yLabel = params['yLabel'] || "";

            },
            'setColors': function(colorArr) {
                // colors is an array
                colors = colorArr;
            },

            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////// getter functions ////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            
            'getZoomMode': function() {
                // returns zoomMode
                return zoomMode;
            },
            'getXInterval': function() {
                // returns X-interval. Applicable only when xZoomMode is 'roll' or 'overwrite'.
                return XInterval;
            },
            'getViewArea' : function() {
                // returns view area
                return viewArea;
            },
            'getBufferSize': function() {
                // return current buffer size
                return bufSize;
            },
            'getColors': function() {
                // returns array of colors
                return colors;
            },
            'getGrid': function() {
                // returns True if grid is enabled
                return grid;
            }
        };
    };
    return {
        'Plot': Plot
    };
})();

