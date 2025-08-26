#include "headers/define.h"
#include "headers/globals.h"
#include "headers/inttypes.h"
#include "SDL2/SDL.h"

int UpdateThread(void* data)
{
    UpdateDeltaTime_mutex = SDL_CreateMutex();

    u32 updateDelay = 1000 / TARGET_UPS;
    u32 lastUpdate = SDL_GetTicks();

    while (executingSimulation)
    {
        
    }
}