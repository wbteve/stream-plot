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

#include "streamplot.h"
#include <SDL2/SDL.h>

typedef struct SP_Plot {
    int nChannels;
    const char* plotStyle;
    const char* color;
    SDL_Window* win;
} SP_Plot;

int SP_Init() {
    return SDL_Init(SDL_INIT_VIDEO);
}

SP_Plot* SP_CreatePlot_base(SP_CreatePlot_args args) {
    // In a future version, have option for line/point thickness, grid and gridbg, legend string
    // Set-up default arguements
    args.windowTitle = args.windowTitle ? args.windowTitle : "Stream Plot";
    args.nChannels = args.nChannels ? args.nChannels : 1; // Minimum 1 channel
    args.bufferSize = args.bufferSize ? args.bufferSize : 1024*1024;

    SP_Plot* newPlot = malloc(sizeof(SP_Plot));
    newPlot->win = SDL_CreateWindow(args.windowTitle, 100, 100, 320, 240,
            SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    if(newPlot->win == NULL)
        return NULL;
    // Return pointer to new plot structure
    return newPlot;
}

void SP_DestroyPlot(SP_Plot* plot) {
    SDL_DestroyWindow(plot->win);
    free(plot);
}

void SP_Quit() {
    SDL_Quit();
}
