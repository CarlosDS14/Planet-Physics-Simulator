#ifndef CIRCLERENDER_H
#define CIRCLERENDER_H

#include "inttypes.h"
#include <SDL2/SDL.h>

void RenderCircle(SDL_Renderer* renderer, int x, int y, float radius);
#ifdef OPENGL_RENDERER
void RenderCircleOpenGL()
#endif
void RenderRectangle(SDL_Renderer* renderer, int x, int y, int w, int h);

#endif // CIRCLERENDER_H