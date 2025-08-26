#ifndef STRUCTS_H
#define STRUCTS_H
#include "inttypes.h"
#include <SDL2/SDL_ttf.h>

typedef struct _cbody
{
    float x, y; // Position of the body
    float vx, vy; // Velocity of the body
    float radius; // Radius of the body
    float mass; // Mass of the body
    u8 ColorRed;
    u8 ColorGreen;
    u8 ColorBlue;
    u8 AlphaChannel;
    u8 Bitfield; // Custom configs like "render enabled", "update enabled", etc
} cbody_t;

typedef struct _photon
{
    float x, y;
    float vx, vy;
} Photon_t;

typedef struct _gui
{
    u16 x, y;
    u16 w, h; // For interaction
    u32 Tw, Th; // For getting the texture
    u8 ColorRed;
    u8 ColorGreen;
    u8 ColorBlue;
    u8 AlphaChannel;
    SDL_Texture* Texture;
    uint8_t bitfieldconfig;
} GUI_t;

typedef struct _ttf_pair
{
    char FontKey[50];
    TTF_Font* FontValue;
} TTF_Pair;

#endif // STRUCTS_H