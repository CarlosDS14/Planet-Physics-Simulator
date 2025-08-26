// For loading with saves, compile with parameter = -D SAVE_LOADER
// For checking data, compile with parameter = -D BODY_CHECKER
// To check current FPS in terminal, compile with parameter = -D TSHOW_FPS
#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "headers/inttypes.h"
#include "headers/GeometryRender.h"
#include "headers/structs.h"
#include "headers/ApplyGravity.h"
#include "headers/LoadSave.h"
#include "headers/MemoryAlloc.h"
#include "headers/define.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#ifdef PLEASEDONOTTOUCHDISPLZ
#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <limits.h>
#else
#error "Unknown OS to compile"
#endif
#endif

/*
    Hardcoded bodies for testing and demonstration.
    TODO: Replace this with acbody_t interactive GUI that permits to customize the
    body to whatever the user wants.
*/
const cbody_t PrecreatedBodies[AMOUNTPRECREATEDBODIES] = {
    {0, 0, 0, 0, 1, 0.00234212f, 0xff, 0xff, 0xff, 0xff, 0b00000000}, // Dust
    {0, 0, 30, 0, 20, 70, 0x5d, 0x6d, 0xe8, 0xff, 0b00000000}, // Moving planet
    {0, 0, 0, 0, 20, 70, 0xff, 0x6b, 0x6b, 0xff, 0b00000000}, // Static planet
    {0, 0, 0, 0, 20, 70, 0x6B, 0xff, 0xa1, 0xff, 0b00000001} // Locked planet
};

int main(int argc, char** argv)
{
    // Window
    SDL_Window* MainWindow = NULL;
    SDL_Renderer* MainRenderer = NULL;
    SDL_Event EventHandler;
    u16 windowSize[] = {800, 620}; // {width, height} original = 800, 620
    #ifdef SAVE_LOADER
    const u8 FileVersionR
    const u8 FileVersionRequired = 1; // File system version
    #endif
    float cameraPosX = 0; // This acts like an offset for all objects in scene
    float cameraPosY = 0;
    int cursorX, cursorY;
    u8 PlanetTypeSelector = 0;
    u8 FPS = 60;
    double desiredDelta = 1000 / FPS;
    bool executingSimulation = true;
    bool load_custom_save = false;
    #ifndef BUILD_WITHOUT_COLLITION
    bool disable_collition = false;
    #endif
    bool put_default_planets = false;
    bool pause_physics = true;
    bool vsync = false;
    bool fatal_error = false;
    cbody_t* BodysLoaded[MAXBODYSINSCENARIO] = {NULL};
    GUI_t* UILoaded[MAXUICOUNTALLOWED] = {NULL};
    // TTF_Pair* FontsLoaded[MAXFONTSLOADED] = {NULL};

    // Flags that can be set 
    if (argc >= 2)
    {
        for (int i = 2; i < argc; i++)
        {
            #ifndef BUILD_WITHOUT_COLLITION
            if (!strcmp(argv[i], "--disable-collition")) disable_collition = true;
            #endif // BUILD_WITHOUT_COLLITION
            if (!strcmp(argv[i], "--set-default-planets")) put_default_planets = true;

        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error: failed to init video\n");
        return 1;
    }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        printf("error: failed to init img formats");
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0)
    {
        printf("error: failed to init ttf\n");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    MainWindow = SDL_CreateWindow(pause_physics ? PAUSEDTITLE : DEFAULTTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize[0], windowSize[1], SDL_WINDOW_SHOWN);
    if (!MainWindow)
    {
        printf("error: failed to create window\n");
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!MainRenderer)
    {
        printf("error: failed to generate renderer\n");
        SDL_DestroyWindow(MainWindow);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    #ifdef SAVE_LOADER
    load_bodies(BodysLoaded, MAXBODYSINSCENARIO, "saves/PPSSavetest.sav", FileVersionRequired);
    load_custom_save = true;
    #endif

    if (load_custom_save == false && put_default_planets == true) default_bodies(BodysLoaded);
    // default_bodies(BodysLoaded);
    #ifdef BODY_CHECKER
    for (int i = 0; i < MAXBODYSINSCENARIO; i++)
    {
        if (BodysLoaded[i] == NULL) continue;
        printf("X: %f\n", BodysLoaded[i]->x);
        printf("Y: %f\n", BodysLoaded[i]->y);
        printf("X vel: %f\n", BodysLoaded[i]->vx);
        printf("Y vel: %f\n", BodysLoaded[i]->vy);
        printf("Radius: %f\n", BodysLoaded[i]->radius);
        printf("Mass: %f\n", BodysLoaded[i]->mass);
        printf("Red: %d\n", (int)BodysLoaded[i]->ColorRed);
        printf("Green: %d\n", (int)BodysLoaded[i]->ColorGreen);
        printf("Blue: %d\n",(int) BodysLoaded[i]->ColorBlue);
        printf("Alpha: %d\n", (int)BodysLoaded[i]->AlphaChannel);
        printf("Bitfield settings: %d\n", (int)BodysLoaded[i]->Bitfield);
        printf("\n");
        objectCountInScenario++;
    }
    printf("object count: %d\n", objectCountInScenario);
    #endif

    // Creating FPS Counter:
    
    // UILoaded[0] = TrackMalloc(sizeof(GUI_t));
    // UILoaded[0]->x = 0;
    // UILoaded[0]->y = 0;
    // UILoaded[0]->w = 100;
    // UILoaded[0]->h = 70;
    // UILoaded[0]->ColorRed = 0x42;
    // UILoaded[0]->ColorGreen = 0x72;
    // UILoaded[0]->ColorBlue = 0xf5;
    // UILoaded[0]->AlphaChannel = 0x80;
    // UILoaded[0]->Font = TTF_OpenFont("MartianMono_Condensed-Regular.ttf", 25);

    // Pause/Unpause button
    UILoaded[0] = TrackMalloc(sizeof(GUI_t));
    UILoaded[0]->x = 0;
    UILoaded[0]->y = 0;
    UILoaded[0]->w = 17;
    UILoaded[0]->h = 18;
    SDL_Surface* scene_control_buttons = IMG_Load("asset/scene_control_buttons.png");
    UILoaded[0]->Texture = SDL_CreateTextureFromSurface(MainRenderer, scene_control_buttons);
    SDL_FreeSurface(scene_control_buttons);

    while (executingSimulation)
    {
        u32 frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&EventHandler))
        {
            if (EventHandler.type == SDL_QUIT)
            {
                executingSimulation = false;
            }
            if (EventHandler.key.keysym.sym == SDLK_d && !EventHandler.key.repeat)
            {
                // TODO: Make a delete keybind, that deletes bodies
            }
            if (EventHandler.type == SDL_KEYDOWN)
            {
                if (EventHandler.key.keysym.sym == SDLK_ESCAPE && !EventHandler.key.repeat)
                {
                    pause_physics = !pause_physics;
                    pause_physics == true ? SDL_SetWindowTitle(MainWindow, PAUSEDTITLE) : SDL_SetWindowTitle(MainWindow, DEFAULTTITLE);
                }
                if (EventHandler.key.keysym.sym == SDLK_e && !EventHandler.key.repeat)
                {
                    PlanetTypeSelector++;
                    if (PlanetTypeSelector >= AMOUNTPRECREATEDBODIES) PlanetTypeSelector = 0;
                }
                if (EventHandler.key.keysym.sym == SDLK_r && !EventHandler.key.repeat)
                {
                    printf("Reseting scenario...\n");
                    for (int i = 0; i < MAXBODYSINSCENARIO; i++)
                    {
                        if (BodysLoaded[i] == NULL) continue;
                        printf("Reseting scenario, freeing body %d...\n", i);
                        TrackFree(BodysLoaded[i]);
                        BodysLoaded[i] = NULL;
                    }
                }
                if (EventHandler.key.keysym.sym == SDLK_c && !EventHandler.key.repeat)
                {
                    // TODO: Make a create keybind, that create bodies with specific parameters, but with customized position
                    bool bodyCreated = false;
                    SDL_GetMouseState(&cursorX, &cursorY);
                    for (int i = 0; i < MAXBODYSINSCENARIO; i++)
                    {
                        if (BodysLoaded[i] != NULL) continue;
                        // BodysLoaded[i] = MallocCBody((float)cursorX, (float)cursorY, 0, 0, 20, 180, 0xff, 0xff, 0xff, 0xff, 0);
                        BodysLoaded[i] = TrackMalloc(sizeof(cbody_t));

                        BodysLoaded[i]->x = (float)cursorX;
                        BodysLoaded[i]->y = (float)cursorY;
                        BodysLoaded[i]->vx = PrecreatedBodies[PlanetTypeSelector].vx;
                        BodysLoaded[i]->vy = PrecreatedBodies[PlanetTypeSelector].vy;
                        BodysLoaded[i]->radius = PrecreatedBodies[PlanetTypeSelector].radius; // 20
                        BodysLoaded[i]->mass = PrecreatedBodies[PlanetTypeSelector].mass; // 180
                        BodysLoaded[i]->ColorRed = PrecreatedBodies[PlanetTypeSelector].ColorRed;
                        BodysLoaded[i]->ColorGreen = PrecreatedBodies[PlanetTypeSelector].ColorGreen;
                        BodysLoaded[i]->ColorBlue = PrecreatedBodies[PlanetTypeSelector].ColorBlue;
                        BodysLoaded[i]->AlphaChannel = PrecreatedBodies[PlanetTypeSelector].AlphaChannel;
                        BodysLoaded[i]->Bitfield = PrecreatedBodies[PlanetTypeSelector].Bitfield;

                        bodyCreated = true;
                        
                        break;
                    }
                    if (bodyCreated == false) printf("error: cannot create anymore objects\n");
                }
            }
        }
        // Rendering

        SDL_SetRenderDrawBlendMode(MainRenderer, SDL_BLENDMODE_BLEND);
        // Render background
        SDL_SetRenderDrawColor(MainRenderer, 0x00, 0x03, 0x00, 0); // #000300
        SDL_RenderClear(MainRenderer);

        // Render bodies
        for (int i = 0; i < MAXBODYSINSCENARIO; i++)
        {
            if (BodysLoaded[i] == NULL) continue;
            float ViewportX = BodysLoaded[i]->x + cameraPosX;
            float ViewportY = BodysLoaded[i]->y + cameraPosY;
            // Highlight
            SDL_SetRenderDrawColor(MainRenderer, 0xff, 0xff, 0xff, 0xff); // #FFFFFF
            RenderCircle(MainRenderer, (i32)ViewportX, (i32)ViewportY, BodysLoaded[i]->radius + 2);
            // Planet
            SDL_SetRenderDrawColor(MainRenderer, BodysLoaded[i]->ColorRed, BodysLoaded[i]->ColorGreen, BodysLoaded[i]->ColorBlue, BodysLoaded[i]->AlphaChannel);
            RenderCircle(MainRenderer, (i32)ViewportX, (i32)ViewportY, BodysLoaded[i]->radius);
        }
        
        // Render UI
        for (int i = 0; i < MAXUICOUNTALLOWED; i++)
        {
            if (UILoaded[i] == NULL) continue;
            SDL_SetRenderDrawColor(MainRenderer, UILoaded[i]->ColorRed, UILoaded[i]->ColorGreen, UILoaded[i]->ColorBlue, UILoaded[i]->AlphaChannel);
        }

        SDL_RenderPresent(MainRenderer);
        // Updating position
        if (!pause_physics)
        {
            for (int i = 0; i < MAXBODYSINSCENARIO; i++)
            {
                if (BodysLoaded[i] == NULL) continue;
                if (BodysLoaded[i]->Bitfield & 0x01) continue;
                BodysLoaded[i]->x += BodysLoaded[i]->vx / 60; // Sum the X velocity of the body
                BodysLoaded[i]->y += BodysLoaded[i]->vy / 60; // Sum the Y velocity of the body
                if (BodysLoaded[i]->x >= (windowSize[0] + 100) || BodysLoaded[i]->x <= -100)
                {
                    printf("Body %d in scenario is out of bounds in X axis, removing...\n", i);
                    TrackFree(BodysLoaded[i]);
                    BodysLoaded[i] = NULL;
                    continue;
                }
                else if (BodysLoaded[i]->y >= (windowSize[1] + 100) || BodysLoaded[i]->y <= -100)
                {
                    printf("Body %d in scenario is out of bounds in Y axis, removing...\n", i);
                    TrackFree(BodysLoaded[i]);
                    BodysLoaded[i] = NULL;
                    continue;
                }
            }
        }
        // Apply gravity and 
        if (pause_physics == false)
        {
            ApplyGravity(BodysLoaded);
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
        printf("Exiting program, freeing body %d...\n", i);
        TrackFree(BodysLoaded[i]);
    }

    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    if (fatal_error != true) return 0;
    else return 1;
}