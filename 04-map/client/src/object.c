#include "structs.h"

extern double delta_time;

struct object *create_object(SDL_Texture *texture, int scale, int resolution) {
    struct object *object = (struct object *) calloc(1, sizeof(struct object));

    if (object == NULL) 
        return NULL;

    object->texture = texture;
    object->resolution = resolution;
    object->animation_speed = 13;
    object->scale = scale;

    return object;
}

void frame_draw_object(struct game *game, struct object *object) {
    int texture_width; 
    SDL_Rect src;
    SDL_Rect dest;

    src.x = object->animation_slide * object->resolution;
    src.y = 0;

    src.w = object->resolution;
    src.h = object->resolution;

    dest.x = object->x; 
    dest.y = object->y; 
    dest.w = object->resolution * object->scale;
    dest.h = object->resolution * object->scale;

    SDL_RenderCopyEx(game->renderer, object->texture, &src, &dest, 0, NULL, object->flip);

    // update animation slide
    SDL_QueryTexture(object->texture, NULL, NULL, &texture_width, NULL);

    object->animation_clock += object->animation_speed*(delta_time/1000);

    if (object->animation_clock >= 1) {
        object->animation_clock = 0;    
        object->animation_slide = (object->animation_slide+1) % (texture_width / object->resolution); // clock arithmetic: jump back to first animation slide 
    }
}

void draw_object(struct game *game, struct object *object) {
    SDL_Rect src; 
    SDL_Rect dest; 

    src.x = 0; 
    src.y = 0; 
    src.w = object->resolution;
    src.h = object->resolution;

    dest.x = object->x; 
    dest.y = object->y; 
    dest.w = object->resolution * object->scale; 
    dest.h = object->resolution * object->scale;

    SDL_RenderCopyEx(game->renderer, object->texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
}


void switch_animation(struct object *object, SDL_Texture *animation) {
    if (object->texture == animation)
        return;

    object->animation_slide = 0; 
    object->texture = animation;
}
