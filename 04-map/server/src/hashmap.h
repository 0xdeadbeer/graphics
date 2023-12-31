#ifndef HASHMAP_H
#define HASHMAP_H

#include "structs.h"

int find_empty_slot(void **map, int slots);
int allocate_value(void ***map, int *total_slots, int *used_slots, int bytes);

#endif
