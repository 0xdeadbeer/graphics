#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "defs.h"

int find_empty_slot(void **map, int slots) {
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

int allocate_value(void ***map, int *slots, int bytes) {
    int empty_slot = find_empty_slot(*map, *slots); 

    if (empty_slot == *slots) {
        (*slots)++;
        *map = realloc(*map, sizeof(void *)*(*slots));
    }

    if (*map == NULL)
        return MEMERR; 

    void *new_value = calloc(1, bytes);
    if (new_value == NULL) 
        return MEMERR;

    (*map)[empty_slot] = new_value;

    return empty_slot;
}
