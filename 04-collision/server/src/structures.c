#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "defs.h"

int map_find_empty_slot(void **map, int slots) {
    int slot = 0; 

    if (slots <= 0) 
        return slot;

    if (map == NULL) 
        return slot;

    while (slot < slots) {
        if (map[slot] == NULL)
            break;
        
        slot++;
    }

    return slot;
}

// If there's Nx fewer used slots than total slots in the map, find the first possible one.
// Else reallocate the map and use the end slot 
// Then allocate an object of size BYTES and add it into the map.
int map_allocate_value(void ***map, int *total_slots, int *used_slots, int bytes) {
    int half_slots = (*total_slots) / 2;
    int empty_slot = 0; 
    
    if (*used_slots < half_slots) {
        empty_slot = map_find_empty_slot(*map, *used_slots); 
        goto jump;   
    } 

    empty_slot = *total_slots;
    (*total_slots)++;
    *map = realloc(*map, sizeof(void *)*(*total_slots));

jump: 
    if (*map == NULL) 
        return MEMERR; 

    void *new_value = calloc(1, bytes); 
    if (new_value == NULL) 
        return MEMERR; 

    (*used_slots)++;
    (*map)[empty_slot] = new_value;

    return empty_slot;
}


