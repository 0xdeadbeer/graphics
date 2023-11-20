#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

struct game {
    SDL_Window *window; 
    SDL_Renderer *renderer;

    int left; 
    int right;
    int jump;
};

struct animation {
    SDL_Texture *tilemap;   
    int resolution;

    int skippable;
};

struct object {
    SDL_Texture *texture;
    int resolution;                 // of the texture/every tile
    int id;

    int x; 
    int y;
    int scale;
    Uint32 flip;

    // animation 
    double animation_clock;         // everytime it reaches 1, the texture switches to the next slide
    double animation_speed;         // how fast will the clock reach 1 with respect to delta_time
    int animation_slide;            // the current slide of the animation
    
    uint8_t state;

    int colliding;
    int force;
};

struct object *create_object(SDL_Texture *texture, int scale, int resolution);
void draw_object(struct game *game, struct object *object);
void switch_animation(struct object *object, SDL_Texture *animation); 

#endif 
