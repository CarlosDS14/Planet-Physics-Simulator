// For loading with saves, compile with parameter = -D SAVE_LOADER
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "headers/inttypes.h"
#include "headers/CircleRender.h"
#include "headers/structs.h"
#include "headers/ApplyGravity.h"
#include "headers/LoadSave.h"
#include <SDL2/SDL.h>

#define MAXBODYSINSCENARIO 10
#define MAXTHREADSALLOWED 10
#define MAXUICOUNTALLOWED 16

int main(int argc, char** argv)
{
    // Window
    SDL_Window* MainWindow = NULL;
    SDL_Renderer* MainRenderer = NULL;
    SDL_Event EventHandler;
    u16 windowSize[] = {800, 620}; // {width, height} original = 800, 620
    float cameraPosX = 0; // This acts like an offset for all objects in scene
    float cameraPosY = 0;
    u8 FPS = 60;
    double desiredDelta = 1000 / FPS;
    u8 frameCount = 0;
    float realFPS = 0.0f;
    bool executingSimulation = true;
    bool load_custom_save = false;
    cbody_t* BodysLoaded[MAXBODYSINSCENARIO] = {NULL};

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error: failed to init video\n");
        return 1;
    }
    MainWindow = SDL_CreateWindow("Planet physics simulator - SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize[0], windowSize[1], SDL_WINDOW_SHOWN);
    if (!MainWindow)
    {
        printf("error: failed to create window\n");
        SDL_Quit();
        return 1;
    }
    MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!MainRenderer)
    {
        printf("error: failed to generate renderer\n");
        SDL_DestroyWindow(MainWindow);
        SDL_Quit();
        return 1;
    }
    #ifdef SAVE_LOADER
    load_bodies(BodysLoaded, MAXBODYSINSCENARIO, "PPSSavetest.sav");
    load_custom_save = true;
    #endif

    if (load_custom_save == false) default_bodies(BodysLoaded);

    // #ifdef SAVE_LOADER
    // u8 objectCountInScenario;
    // for (int i = 0; i < MAXBODYSINSCENARIO; i++)
    // {
    //     if (BodysLoaded[i] == NULL)
    //     printf("X: %.1f\n", BodysLoaded[i]->x);
    //     printf("Y: %.1f\n", BodysLoaded[i]->y);
    //     printf("X vel: %.1f\n", BodysLoaded[i]->vx);
    //     printf("Y vel: %.1f\n", BodysLoaded[i]->vy);
    //     printf("Radius: %.1f\n", BodysLoaded[i]->radius);
    //     printf("Mass: %.1f\n", BodysLoaded[i]->mass);
    //     printf("Red: %d\n", (int)BodysLoaded[i]->ColorRed);
    //     printf("Green: %d\n", (int)BodysLoaded[i]->ColorGreen);
    //     printf("Blue: %d\n",(int) BodysLoaded[i]->ColorBlue);
    //     printf("Alpha: %d\n", (int)BodysLoaded[i]->AlphaChannel);
    //     printf("Bitfield settings: %d\n", (int)BodysLoaded[i]->Bitfield);
    //     objectCountInScenario++;
    // }
    // printf("object count: %d\n", objectCountInScenario);
    // #endif

    u32 fpsTimer = SDL_GetTicks();
    while (executingSimulation)
    {
        u32 frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&EventHandler))
        {
            if (EventHandler.type == SDL_QUIT)
            {
                executingSimulation = false;
            }
            if (EventHandler.type == SDL_KEYDOWN)
            {
                
            }
        }
        // Rendering
        SDL_SetRenderDrawBlendMode(MainRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(MainRenderer, 0, 0, 0, 255);
        SDL_RenderClear(MainRenderer);
        for (int i = 0; i < MAXBODYSINSCENARIO; i++)
        {
            if (BodysLoaded[i] == NULL) continue;
            SDL_SetRenderDrawColor(MainRenderer, BodysLoaded[i]->ColorRed, BodysLoaded[i]->ColorGreen, BodysLoaded[i]->ColorBlue, BodysLoaded[i]->AlphaChannel);
            float ViewportX = BodysLoaded[i]->x + cameraPosX;
            float ViewportY = BodysLoaded[i]->y + cameraPosY;
            RenderCircle(MainRenderer, (i32)ViewportX, (i32)ViewportY, BodysLoaded[i]->radius);
        }
        SDL_SetRenderDrawColor(MainRenderer, 0x00, 0x00, 0x00, 0x80);

        SDL_RenderPresent(MainRenderer);
        // Updating position
        for (int i = 0; i < MAXBODYSINSCENARIO; i++)
        {
            if (BodysLoaded[i] == NULL) continue;
            if (BodysLoaded[i]->Bitfield & 0x01) continue;
            BodysLoaded[i]->x += BodysLoaded[i]->vx; // Sum the X velocity of the body
            BodysLoaded[i]->y += BodysLoaded[i]->vy; // Sum the Y velocity of the body
            if (BodysLoaded[i]->x >= (windowSize[0] + 100) || BodysLoaded[i]->x <= -100)
            {
                printf("Body %d in scenario is out of bounds in X axis, removing...\n", i);
                free(BodysLoaded[i]);
                BodysLoaded[i] = NULL;
                continue;
            }
            else if (BodysLoaded[i]->y >= (windowSize[1] + 100) || BodysLoaded[i]->y <= -100)
            {
                printf("Body %d in scenario is out of bounds in Y axis, removing...\n", i);
                free(BodysLoaded[i]);
                BodysLoaded[i] = NULL;
                continue;
            }
        }
        // Apply gravity
        ApplyGravity(BodysLoaded, (u16)MAXBODYSINSCENARIO);

        frameCount++;
        if (SDL_GetTicks() - fpsTimer >= 1000)
        {
            realFPS = frameCount * 1000.0f / (SDL_GetTicks() - fpsTimer);
            frameCount = 0;
            fpsTimer = SDL_GetTicks();
            printf("\rFPS: %.1f", realFPS);
        }

        u32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < desiredDelta)
        {
            SDL_Delay(desiredDelta - frameTime);
        }
    }

    for (int i = 0; i < MAXBODYSINSCENARIO; i++)
    {
        if (BodysLoaded[i] == NULL) continue;
        free(BodysLoaded[i]);
    }

    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);
    SDL_Quit();
    return 0;
}