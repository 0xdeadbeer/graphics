#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

struct game {
    SDL_Window *window; 
    SDL_Renderer *renderer;

    int left; 
    int right;
};

struct animation {
    SDL_Texture *tilemap;   
    int resolution;

    int skippable;
};

struct object {
    SDL_Texture *texture;
    int resolution;                 // of the texture/every tile

    int x; 
    int y;
    int scale;
    Uint32 flip;

    // animation 
    double animation_clock;         // everytime it reaches 1, the texture switches to the next slide
    double animation_speed;         // how fast will the clock reach 1 with respect to delta_time
    int animation_slide;            // the current slide of the animation
};

#endif 
