#include "pong.h"

void rect_set(SDL_Rect * r, int x, int y, int w, int h)
{
    r->x = x;
    r->y = y;
    if (w && h) {
        r->w = w;
        r->h = h;
    }
}


