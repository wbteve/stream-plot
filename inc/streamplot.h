/*
 Copyright (c) 2014, Sagar G V <sagar.writeme@gmail.com>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __STREAMPLOT_H__
#define __STREAMPLOT_H__

#define STREAMPLOT_API_VERSION 1

typedef struct SP_Plot SP_Plot;


/* Call once to initialize StreamPlot
 * Returns 0 on success. Other return values indicate failure
 */
int SP_Init();

/* Create a streamplot.
 * Returns a SP_Plot structure if successful. Returns NULL on failure.
 * Example usage: SP_Plot* plot1 = SP_CreatePlot(.nChannels=2, .windowTitle="Test Plot");
 */
typedef struct {
    int version;
    int nChannels;
    const char* plotStyle; // 'l' for line, 'p' for point
    const char* color; // "rgbk" ('k' for black)
    const char* xLabel;
    const char* yLabel;
    const char* windowTitle;
    long bufferSize;
} SP_CreatePlot_args;
#define SP_CreatePlot(...) SP_CreatePlot_base((SP_CreatePlot_args){__VA_ARGS__});
SP_Plot* SP_CreatePlot_base(SP_CreatePlot_args args);

/* Destroys a plot
 * Call to remove the plot window and destroy associated data structure
 */
void SP_DestroyPlot(SP_Plot* plot);

/* Call once before program terminates
 * This closes all streamplots.
 */
void SP_Quit();

/*
 * Block until all windows have been closed
 */
void SP_WaitForAllWindowsAndQuit();

#endif
