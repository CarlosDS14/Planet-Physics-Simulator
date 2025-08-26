#include "headers/GeometryRender.h"
#include <math.h>

void RenderCircle(SDL_Renderer* renderer, int x, int y, float radius)
{
    // Code that makes a circle
    SDL_Point center = {.x = x, .y = y};
    const float diameter = radius * radius;
    for (int y = -radius; y <= radius; y++)
    {
        int dx = (int)sqrtf(diameter - y * y);
        SDL_RenderDrawLine(renderer, center.x - dx, center.y + y, center.x + dx, center.y + y);
    }
}

void RenderRectangle(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    SDL_Rect tempRect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &tempRect);
}