#ifndef FORMAT_H
#define FORMAT_H

#include <stdlib.h>

int handle_player_connect(int *message, struct object ***map, int *slots);
int handle_object_properties(int *message, struct object ***map, int *slots);

#endif
