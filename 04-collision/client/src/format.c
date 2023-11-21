#include <SDL2/SDL.h>
#include "defs.h"
#include "structs.h"
#include "format.h"

extern SDL_Texture *idle_down_animation;
extern struct object *create_object(SDL_Texture *texture, int scale, int resolution);

int handle_player_connect(int *message, struct object ***map, int *slots) {
    int object_id = message[1]; 
    
    if (object_id >= *slots) {
        *slots = object_id+1;
        *map = realloc(*map, sizeof(struct object *)*(*slots));
    }

    if (*map == NULL) 
        return MEMERR; 

    struct object *new_object = create_object(idle_down_animation, 4, 64);
    if (new_object == NULL) 
        return MEMERR; 

    new_object->id = object_id;
    new_object->x = message[2]; 
    new_object->y = message[3]; 
    new_object->colliding = message[4]; 
    new_object->force = message[5]; 
    new_object->state = message[6];
    (*map)[object_id] = new_object;

    return STDOK;
}

int handle_object_properties(int *message, struct object ***map, int *slots) {
    int object_id = message[1]; 

    if (object_id >= *slots) {
        *slots = object_id+1;
        *map = realloc(*map, sizeof(struct object *)*(*slots));
    }

    if (*map == NULL)
        return MEMERR;
    
    if ((*map)[object_id] == NULL) {
        struct object *new_object = create_object(idle_down_animation, 4, 64);
        if (new_object == NULL) 
            return MEMERR;

        new_object->id = object_id;
        (*map)[object_id] = new_object;
    }

    (*map)[object_id]->x = message[2];
    (*map)[object_id]->y = message[3];
    (*map)[object_id]->colliding = message[4];
    (*map)[object_id]->force = message[5];
    (*map)[object_id]->state = message[6];

    return STDOK;
}
