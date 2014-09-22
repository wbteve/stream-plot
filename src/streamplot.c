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

typedef struct SP_Plot {
    int nChannels;
    const char* plotStyle;
    const char* color;
    const char* windowTitle;
    SDL_Window* win;
} SP_Plot;

static void SP_PostEvent(int code, void* data1, void* data2);
static int PltThreadFunc(void *arg);

static SDL_Thread* pltThread;
static int initDone;
static SDL_mutex* SP_sync;

static SDL_mutex* SP_quit_lock;
static SDL_cond* SP_quit_cond;
static SDL_bool SP_quit_condition = SDL_FALSE;

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Private functions ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

static int PltThreadFunc(void *arg) {
    int quit = 0;
    SDL_Event evt;
    SP_Plot* plt;

    while (!quit) {
        while (SDL_PollEvent(&evt) != 0) {
            switch (evt.type) {
            case SDL_QUIT:
                SDL_LockMutex(SP_quit_lock);
                SP_quit_condition = SDL_TRUE;
                SDL_CondBroadcast(SP_quit_cond);
                SDL_UnlockMutex(SP_quit_lock);
                break;
            case SDL_WINDOWEVENT:
                switch (evt.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    plt = SDL_GetWindowData(
                            SDL_GetWindowFromID(evt.window.windowID),
                            "SP_Plot");
                    SDL_DestroyWindow(plt->win);
                    plt->win = NULL;
                    break;
                }
                break;
            case SDL_USEREVENT:
                switch (evt.user.code) {
                case SP_QUIT_ALL_EVENT:
                    quit = 1;
                    break;
                case SP_CREATE_WINDOW_EVENT:
                    plt = evt.user.data2;
                    plt->win = SDL_CreateWindow(plt->windowTitle,
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240,
                            SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
                    SDL_SetWindowData(plt->win, "SP_Plot", plt);
                    break;
                case SP_DESTROY_WINDOW_EVENT:
                    plt = evt.user.data2;
                    if (plt->win) {
                        SDL_DestroyWindow(plt->win);
                        plt->win = NULL;
                    }
                    break;
                }
                break;
            }
        }
        SDL_Delay(20);
    }
    return 0;
}

static void SP_PostEvent(int code, void* data1, void* data2) {
    SDL_Event evt;
    SDL_zero(evt);
    evt.type = SDL_USEREVENT;
    evt.user.code = code;
    evt.user.data1 = data1;
    evt.user.data2 = data2;
    SDL_PushEvent(&evt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Public functions ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

int SP_Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        return 1;
    }
    pltThread = SDL_CreateThread(PltThreadFunc, "PltThread", NULL);
    if (pltThread == NULL) {
        SDL_Quit();
        return 2;
    }
    SP_sync = SDL_CreateMutex();
    SP_quit_lock = SDL_CreateMutex();
    SP_quit_cond = SDL_CreateCond();
    if (SP_sync == NULL || SP_quit_lock == NULL || SP_quit_cond == NULL) {
        SDL_Quit();
        return 3;
    }
    initDone = 1;
    return 0;
}

void SP_Quit() {
    int status;
    if (!initDone) {
        return;
    }
    SP_PostEvent(SP_QUIT_ALL_EVENT, NULL, NULL);
    SDL_WaitThread(pltThread, &status);
    SDL_DestroyMutex(SP_sync);
    SDL_Quit();
}

SP_Plot* SP_CreatePlot_base(SP_CreatePlot_args args) {
    if (!initDone) {
        return NULL;
    }
    if (SDL_LockMutex(SP_sync) != 0) {
        SDL_Log("Error acquiring sync lock.\n");
        return NULL;
    }
    // Set-up default arguements
    args.windowTitle = args.windowTitle ? args.windowTitle : "Stream Plot";
    args.nChannels = args.nChannels ? args.nChannels : 1; // Minimum 1 channel
    args.bufferSize = args.bufferSize ? args.bufferSize : 1024 * 1024;

    SP_Plot* newPlot = malloc(sizeof(SP_Plot));
    newPlot->windowTitle = args.windowTitle;

    SP_PostEvent(SP_CREATE_WINDOW_EVENT, NULL, newPlot);
    SDL_UnlockMutex(SP_sync);
    return newPlot;
}

void SP_DestroyPlot(SP_Plot* plot) {
    if (!initDone) {
        return;
    }
    if (SDL_LockMutex(SP_sync) != 0) {
        SDL_Log("Error acquiring sync lock.\n");
        return;
    }
    SP_PostEvent(SP_DESTROY_WINDOW_EVENT, NULL, plot);
    free(plot);
    SDL_UnlockMutex(SP_sync);
}
void SP_WaitForAllWindowsToClose() {
    if (!initDone) {
        return;
    }
    SDL_LockMutex(SP_quit_lock);
    while (!SP_quit_condition) {
        SDL_CondWait(SP_quit_cond, SP_quit_lock);
    }
    SP_quit_condition = SDL_FALSE;
    SDL_UnlockMutex(SP_quit_lock);

}

