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

typedef struct {
    SDL_mutex* lock;
    SDL_cond* cond;
    SDL_bool condition;
} SP_WaitCondition;

static void SP_PostEvent(int code, void* data1, void* data2);
static int SP_PltThreadFunc(void *arg);

static SP_WaitCondition* SP_InitWaitCondition();
static void SP_WaitForCondition(SP_WaitCondition* w);
static void SP_SignalCondition(SP_WaitCondition* w);

static SDL_Thread* pltThread;
static int initDone;
static SDL_mutex* SP_sync;

static SP_WaitCondition* SP_Quit_Condition;

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Private functions ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

static SP_WaitCondition* SP_InitWaitCondition() {
    SP_WaitCondition* w = (SP_WaitCondition *) malloc(sizeof(SP_WaitCondition));
    if (w == NULL)
        return NULL;
    w->condition = SDL_FALSE;
    w->lock = SDL_CreateMutex();
    if (w->lock == NULL) {
        free(w);
        return NULL;
    }
    w->cond = SDL_CreateCond();
    if (w->cond == NULL) {
        SDL_DestroyCond(w->cond);
        free(w);
        return NULL;
    }
    return w;
}
static void SP_DestroyWaitCondition(SP_WaitCondition* w) {
    SDL_DestroyMutex(w->lock);
    SDL_DestroyCond(w->cond);
    free(w);
}
static void SP_WaitForCondition(SP_WaitCondition* w) {
    SDL_LockMutex(w->lock);
    while (!w->condition) {
        SDL_CondWait(w->cond, w->lock);
    }
    w->condition = SDL_FALSE;
    SDL_UnlockMutex(w->lock);
}
static void SP_SignalCondition(SP_WaitCondition* w) {
    SDL_LockMutex(w->lock);
    w->condition = SDL_TRUE;
    SDL_CondBroadcast(w->cond);
    SDL_UnlockMutex(w->lock);
}

static int SP_PltThreadFunc(void *arg) {
    int quit = 0;
    SDL_Event evt;
    SP_Plot* plt;

    while (!quit) {
        while (SDL_PollEvent(&evt) != 0) {
            switch (evt.type) {
            case SDL_QUIT:
                SP_SignalCondition(SP_Quit_Condition);
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
    SP_sync = SDL_CreateMutex();
    if (SP_sync == NULL) {
        SDL_Quit();
        return 2;
    }
    SP_Quit_Condition = SP_InitWaitCondition();
    if (SP_Quit_Condition == NULL) {
        SDL_DestroyMutex(SP_sync);
        SDL_Quit();
        return 3;
    }
    pltThread = SDL_CreateThread(SP_PltThreadFunc, "PltThread", NULL);
    if (pltThread == NULL) {
        SDL_Quit();
        return 2;
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
    SP_DestroyWaitCondition(SP_Quit_Condition);
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
    SP_WaitForCondition(SP_Quit_Condition);
}

