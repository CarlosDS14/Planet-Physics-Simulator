#ifndef STRUCTS_H
#define STRUCTS_H
#include "inttypes.h"

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
    u8 Bitfield; 
} cbody_t;

typedef struct _gui
{
    u16 x, y;
    u16 w, h;
    u8 ColorRed;
    u8 ColorGreen;
    u8 ColorBlue;
    u8 AlphaChannel;
} GUI_t;


#endif // STRUCTS_H