#include <SDL2/SDL.h>
#include "defs.h"
#include "structs.h"
#include "format.h"

extern SDL_Texture *grass_texture;
extern SDL_Texture *cobble_texture;
extern SDL_Texture *lava_texture;
extern struct object *create_object(SDL_Texture *texture, int scale, int resolution);

int handle_block_creation(int id, int type, struct object ***map, int *slots) {
    (*slots)++;
    *map = realloc(*map, sizeof(struct object *)*(*slots)); 
    if (*map == NULL) 
        return MEMERR;

    SDL_Texture *texture; 
    switch (type) {
        case 0: 
            texture = grass_texture; 
            break; 
        case 1: 
            texture = cobble_texture; 
            break; 
        case 2: 
            texture = lava_texture; 
            break;
        default: 
            texture = grass_texture; 
            break;
    }

    struct object *new_block = create_object(texture, 1, 32);
    if (new_block == NULL)
        return MEMERR; 

    (*map)[id] = new_block;

    return STDOK;
}
