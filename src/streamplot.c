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

#define SP_QUIT_ALL_EVENT 0
#define SP_CREATE_WINDOW_EVENT 1
#define SP_DESTROY_WINDOW_EVENT 2
#define SP_QUIT_WHEN_ALL_WINDOWS_CLOSE_EVENT 3

typedef struct SP_Plot {
    int nChannels;
    const char* plotStyle;
    const char* color;
    const char* windowTitle;
    SDL_Window* win;
} SP_Plot;

void SP_PostEvent(int code, void* data1, void* data2);

static SDL_Thread* pltThread;
static int nWindows;

static int PltThreadFunc(void *arg) {
    int quit = 0, waitAndQuit = 0;
    SDL_Event evt;
    while (!quit) {
        while (SDL_PollEvent(&evt) != 0) {
            switch (evt.type) {
            case SDL_QUIT:
                nWindows--;
                if(nWindows == 0 && waitAndQuit == 1)
                    quit = 1;
                break;
            case SDL_USEREVENT:
                switch (evt.user.code) {
                case SP_QUIT_ALL_EVENT:
                    quit = 1;
                    break;
                case SP_CREATE_WINDOW_EVENT:
                    nWindows++;
                    SP_Plot* newPlt = evt.user.data2;
                    newPlt->win = SDL_CreateWindow(newPlt->windowTitle, 100,
                            100, 320, 240,
                            SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
                    break;
                case SP_QUIT_WHEN_ALL_WINDOWS_CLOSE_EVENT:
                    waitAndQuit = 1;
                    if(nWindows == 0)
                        quit = 1;
                }
                break;
            }
        }
        SDL_Delay(20);
    }
    SDL_Quit();
    return 0;
}

int SP_Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }
    pltThread = SDL_CreateThread(PltThreadFunc, "PltThread", NULL);
    if (pltThread == NULL) {
        return 2;
    }
    return 0;
}

SP_Plot* SP_CreatePlot_base(SP_CreatePlot_args args) {
    // In a future version, have option for line/point thickness, grid and gridbg, legend string
    // Set-up default arguements
    args.windowTitle = args.windowTitle ? args.windowTitle : "Stream Plot";
    args.nChannels = args.nChannels ? args.nChannels : 1; // Minimum 1 channel
    args.bufferSize = args.bufferSize ? args.bufferSize : 1024 * 1024;

    SP_Plot* newPlot = malloc(sizeof(SP_Plot));
    newPlot->windowTitle = args.windowTitle;

    SP_PostEvent(SP_CREATE_WINDOW_EVENT, NULL, newPlot);

    return newPlot;
}

void SP_DestroyPlot(SP_Plot* plot) {
    SDL_DestroyWindow(plot->win);
    free(plot);
}
void SP_WaitForAllWindowsAndQuit() {
    SP_PostEvent(SP_QUIT_WHEN_ALL_WINDOWS_CLOSE_EVENT, NULL, NULL);
    int status;
    SDL_WaitThread(pltThread, &status);
}
void SP_Quit() {
    SP_PostEvent(SP_QUIT_ALL_EVENT, NULL, NULL);
    int status;
    SDL_WaitThread(pltThread, &status);
}

void SP_PostEvent(int code, void* data1, void* data2) {
    SDL_Event evt;
    SDL_zero(evt);
    evt.type = SDL_USEREVENT;
    evt.user.code = code;
    evt.user.data1 = data1;
    evt.user.data2 = data2;
    SDL_PushEvent(&evt);
}
