#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

struct game {
    SDL_Window *window; 
    SDL_Renderer *renderer;

    int up; 
    int down; 
    int left; 
    int right;
};

struct object {
    SDL_Texture *texture; 
    int resolution; // of the texture/each tiles

    int x; 
    int y;
    int scale;

    // animation 
    int state;
};

#endif 
