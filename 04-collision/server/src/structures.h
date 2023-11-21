#ifndef STRUCTURES_H
#define STRUCTURES_H 

#include "structs.h"

int map_find_empty_slot(void **map, int slots);
int map_allocate_value(void ***map, int *total_slots, int *used_slots, int bytes);

#endif
