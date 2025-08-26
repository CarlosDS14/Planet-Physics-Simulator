#include "headers/globals.h"

float UpdateDeltaTime = 0.00f;
SDL_mutex* UpdateDeltaTime_mutex = NULL;

bool running = true;
SDL_mutex* running_mutex = NULL;