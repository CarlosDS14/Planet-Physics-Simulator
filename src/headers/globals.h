#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <SDL2/SDL_mutex.h>

extern float UpdateDeltaTime;
extern SDL_mutex* UpdateDeltaTime_mutex;

extern bool executingSimulation;
extern SDL_mutex* executingSimulation_mutex;

#endif